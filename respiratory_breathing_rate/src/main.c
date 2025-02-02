/* Notes by Allan 2/1/2025:

This code reads an analog signal from an ADC channel (Connected to pin P0.02 on nrf52840dk) 
and calculates the breathing rate of a person using a moving average filter and 
peak detection algorithm.

The raw ADC value is converted to millivolts and passed through a moving average filter 
to remove noise. The moving average value is then used to detect peaks in the signal, 
which are used to calculate the breathing rate in breaths per minute.

Git pushed on 2/1/2025
*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <stdio.h>  // Include stdio.h for printf

#define ADC_REF_VOLTAGE_MV 600 // Internal reference in mV
#define ADC_GAIN (1.0 / 6.0)    // Gain of 1/6
#define ADC_RESOLUTION 4096     // 12-bit resolution

/* ADC channel configuration via Device Tree */
static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

/* ADC buffer and sequence configuration */
static int16_t buf;
static struct adc_sequence sequence = {
    .buffer = &buf,
    .buffer_size = sizeof(buf), 
    /* Optional calibration */
    //.calibrate = true,
};

// Convert raw ADC value to millivolts
int32_t convert_to_mv(int16_t raw_value) {
    return (int32_t)((raw_value * ADC_REF_VOLTAGE_MV * (1.0 / ADC_GAIN)) / ADC_RESOLUTION);
}

// -------- Filtering ---------------------------------------------
#include <math.h>  // Include for exponential calculations if needed
#define MOVING_AVERAGE_WINDOW 5
#define EMA_ALPHA 0.2  // Weight for Exponential Moving Average

/* Moving Average Filter */
int32_t moving_average_filter(int32_t *buffer, int32_t new_sample) {
    static int32_t sum = 0;
    static int index = 0;
    static int32_t data_buffer[MOVING_AVERAGE_WINDOW] = {0};

    sum -= data_buffer[index];
    data_buffer[index] = new_sample;
    sum += new_sample;

    index = (index + 1) % MOVING_AVERAGE_WINDOW;
    return sum / MOVING_AVERAGE_WINDOW;
}
// -------- Filtering ---------------------------------------------


// ------- Breathing Rate Calculation ------------------------------
#define BREATHING_THRESHOLD 10   // Minimum change to detect a breath
#define MAX_PEAKS 10            // Circular buffer for peak timestamps
#define MIN_PEAK_INTERVAL_MS 1500 // ~40 breaths per minute

// Detect peaks
bool detect_peak(int32_t current_value, int32_t prev_value, bool *rising) {
    if (*rising && current_value < prev_value) {
        *rising = false;
        return true; // Peak detected
    } else if (!*rising && current_value > prev_value + BREATHING_THRESHOLD) {
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

float calculate_breathing_rate_new(void) {

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

    // Convert average period to breaths per minute
    return 60000.0 / avg_period; // 60000 ms per minute
}
// ------- Breathing Rate Calculation ------------------------------


int main(void){
    // Verify ADC readiness
    if (!adc_is_ready_dt(&adc_channel)) {
        printf("ADC controller device %s not ready\n", adc_channel.dev->name);
        return -1;
    }

    // Configure ADC channel
    int err = adc_channel_setup_dt(&adc_channel);
    if (err < 0) {
        printf("Could not setup ADC channel (%d)\n", err);
        return -1;
    }

    // Initialize ADC sequence 
    err = adc_sequence_init_dt(&adc_channel, &sequence);
    if (err < 0) {
        printf("Could not initialize ADC sequence (%d)\n", err);
        return -1;
    }

    // Filter Variables
    int32_t prev_val_moving_avg = 0;

    // Breathing rate variables 
    bool rising = false;
    uint32_t last_peak_time = 0; // Timestamp of the last valid peak

    // Continuously read ADC values
    while (1) {
        err = adc_read(adc_channel.dev, &sequence);
        if (err < 0) {
            printf("ADC read failed (%d)\n", err);

        } else {

            int32_t val_mv = convert_to_mv(buf);
            //printf("Raw ADC value: %d\n", buf);

            // Millivolts (non-filtered value)
            // printf(">v:%d\n", val_mv);

            // Filtering code
            // Apply Moving Average Filter
            int32_t moving_avg = moving_average_filter(&prev_val_moving_avg, val_mv);
            moving_avg = moving_avg - 2100;
            printf(">m:%d\n", moving_avg); // Moving average value

            // Breathing rate calculation
            if (detect_peak(moving_avg, prev_val_moving_avg, &rising)) {
                uint32_t current_time = k_uptime_get(); // Get current uptime in milliseconds

                // Check if enough time has passed since the last peak
                if (current_time - last_peak_time >= MIN_PEAK_INTERVAL_MS) {
                    last_peak_time = current_time; // Update the last peak time
                    add_peak_timestamp(current_time); // Add the peak timestamp

                    // Calcuulate and display the breathing rate
                    float breathing_rate = calculate_breathing_rate_new();
                    if (breathing_rate > 0) {
                        printf(">b:%.2f\n", breathing_rate); // breathing rate in breaths per minute  
                    } 
                }
            }
            prev_val_moving_avg = moving_avg;
        }  
        k_sleep(K_MSEC(100));  // Sleep for 100ms, 10Hz sampling rate
    }
    return 0;
}