/* Notes by Allan 2/1/2025:

This code reads an analog signal from an ADC channel (Connect to Pin P0.02 of nrf52840dk) 
and applies a derivative filter to the signal to detect edges. The code calculates 
the BPM (beats per minute) based on the time intervals between peaks.

Git pushed on 2/1/2025
*/ 


#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <stdio.h>
#include <zephyr/sys/printk.h> // For printk

#define ADC_REF_VOLTAGE_MV 600   // Internal reference in mV
#define ADC_GAIN (1.0 / 6.0)     // Gain of 1/6
#define ADC_RESOLUTION 4096      // 12-bit resolution
#define THRESHOLD 2600           // Adjustable threshold for pulse detection

// Define ADC channel
static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

static int16_t buf;
static struct adc_sequence sequence = {
    .buffer = &buf,
    .buffer_size = sizeof(buf),
};


// Convert raw ADC value to millivolts
int32_t convert_to_mv(int16_t raw_value) {
    return (int32_t)((raw_value * ADC_REF_VOLTAGE_MV * (1.0 / ADC_GAIN)) / ADC_RESOLUTION);
}


// -----------------Filtering ------------------
#include <math.h>  // Include for exponential calculations if needed

// Derivative filter
int32_t derivative_filter(int32_t new_sample, int32_t prev_sample) {
    return new_sample - prev_sample;
}
// -----------------Filtering ------------------



// ------- Heart Rate Calculation ------------------------------
#define BEAT_THRESHOLD 100   // Minimum change to detect a breath
#define MAX_PEAKS 15            // Circular buffer for peak timestamps
#define MIN_PEAK_INTERVAL_MS 600 // Minimum interval between peaks for valid BPM calculation (at least 60 BPM)

// Detect peaks
bool detect_peak(int32_t current_value, int32_t prev_value, bool *rising) {
    if (*rising && current_value < prev_value) {
        *rising = false;
        return true; // Peak detected
    } else if (!*rising && current_value > prev_value + BEAT_THRESHOLD) {
        *rising = true;
        // printf("Rising True\n"); // for debugging
    }
    return false;
}

// Circular buffer to store timestamps of peaks
uint32_t peak_timestamps[MAX_PEAKS];
int peak_index = 0;

void add_peak_timestamp(uint32_t timestamp) {
    peak_timestamps[peak_index] = timestamp;
    peak_index = (peak_index + 1) % MAX_PEAKS;
}

float calculate_BPM(void) {
    int valid_peaks = 0; // Number of valid peaks
    if (peak_index < MAX_PEAKS) {
        valid_peaks = peak_index;
    } else {
        valid_peaks = MAX_PEAKS;
    }

    if (valid_peaks < 2) {
        return 0.0; // Not enough peaks to calculate rate
    }

    // Calculate time intervals between consecutive peaks
    int interval_count = valid_peaks - 1;
    uint32_t intervals[interval_count];
    for (int i = 0; i < interval_count; i++) {
        int current = (peak_index - i - 1 + MAX_PEAKS) % MAX_PEAKS;
        int previous = (current - 1 + MAX_PEAKS) % MAX_PEAKS;
        intervals[i] = peak_timestamps[current] - peak_timestamps[previous];
    }

    // Compute the average interval
    uint32_t sum_intervals = 0;
    for (int i = 0; i < interval_count; i++) {
        sum_intervals += intervals[i];
    }
    float avg_period = (float)sum_intervals / interval_count;

    // Convert average period to beats per minute
    return 60000.0 / avg_period; // 60000 ms per minute
}
// ------- Heart Rate Calculation ------------------------------


void main(void) {
    // Verify ADC channel is ready
    if (!adc_is_ready_dt(&adc_channel)) {
        printk("ADC controller device %s not ready\n", adc_channel.dev->name);
        return;
    }

    // Configure ADC channel
    int err = adc_channel_setup_dt(&adc_channel);
    if (err < 0) {
        printk("Could not setup ADC channel (%d)\n", err);
        return;
    }

    // Initialize ADC sequence
    err = adc_sequence_init_dt(&adc_channel, &sequence);
    if (err < 0) {
        printk("Could not initialize ADC sequence (%d)\n", err);
        return;
    }

    // Filter Variables
    int32_t prev_sample = 0; // Derivative filter variable

    // Heart Rate Variables
    bool rising = false;
    uint32_t last_peak_time = 0; // Timestamp of the last valid peak
    int32_t previous_val = 0;

    // Continuously read ADC values
    while (1) {
        err = adc_read(adc_channel.dev, &sequence);
        if (err < 0) {
            printk("ADC read failed (%d)\n", err);

        } else {

            int32_t val_mv = convert_to_mv(buf); // Convert raw ADC value to millivolts
            // printk("Raw ADC value: %d, ADC value: %d mV\n", buf, val_mv);
            printk(">v:%d\n", val_mv);

            // Filtering code
            int32_t edge_enhanced = derivative_filter(val_mv, prev_sample);
            prev_sample = val_mv;
            printf(">e:%d\n", edge_enhanced); // Edge enhanced signal
        
            // BPM calculation
            if (detect_peak(val_mv, previous_val, &rising)) {
                uint32_t current_time = k_uptime_get(); // Get current uptime in milliseconds

                // Check if enough time has passed since the last peak
                if (current_time - last_peak_time >= MIN_PEAK_INTERVAL_MS) {
                    last_peak_time = current_time; // Update the last peak time
                    add_peak_timestamp(current_time); // Add the peak timestamp

                    // Calculate and display the BPM
                    float bpm = calculate_BPM();
                    if (bpm > 0) {
                        printf(">b:%.2f\n", bpm); // beats per minute  
                    } 
                }
            }
            previous_val = val_mv;
        }

        k_sleep(K_MSEC(100)); // sleep for 100ms, 10Hz
    }
    return;
}