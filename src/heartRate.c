#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <stdio.h>
#include <zephyr/sys/printk.h> // For printk

#define ADC_REF_VOLTAGE_MV 600   // Internal reference in mV
#define ADC_GAIN (1.0 / 6.0)     // Gain of 1/6
#define ADC_RESOLUTION 4096      // 12-bit resolution
#define THRESHOLD 2000           // Adjustable threshold for pulse detection
#define SAMPLE_INTERVAL_MS 10    // Sampling interval in milliseconds

static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

static int16_t buf;
static struct adc_sequence sequence = {
    .buffer = &buf,
    .buffer_size = sizeof(buf),
};

int32_t convert_to_mv(int16_t raw_value) {
    return (int32_t)((raw_value * ADC_REF_VOLTAGE_MV * (1.0 / ADC_GAIN)) / ADC_RESOLUTION);
}

void main(void) {
    if (!adc_is_ready_dt(&adc_channel)) {
        printk("ADC controller device %s not ready\n", adc_channel.dev->name);
        return;
    }

    int err = adc_channel_setup_dt(&adc_channel);
    if (err < 0) {
        printk("Could not setup ADC channel (%d)\n", err);
        return;
    }

    err = adc_sequence_init_dt(&adc_channel, &sequence);
    if (err < 0) {
        printk("Could not initialize ADC sequence (%d)\n", err);
        return;
    }

    int32_t previous_val = 0;
    bool pulse_detected = false;
    uint32_t last_pulse_time = 0;
    uint32_t current_time = 0;
    uint32_t bpm = 0;

    while (1) {
        err = adc_read(adc_channel.dev, &sequence);
        if (err < 0) {
            printk("ADC read failed (%d)\n", err);
        } else {
            int32_t val_mv = convert_to_mv(buf);
            //printk("Raw ADC value: %d, ADC value: %d mV\n", buf, val_mv);

            current_time = k_uptime_get_32(); // Get system uptime in milliseconds

            // Detect rising edge
            if (val_mv > THRESHOLD && !pulse_detected && val_mv > previous_val) {
                pulse_detected = true;
                if (last_pulse_time > 0) {
                    uint32_t interval_ms = current_time - last_pulse_time;
                    bpm = 60000 / interval_ms; // Calculate BPM
                    printk("Pulse detected! BPM: %d\n", bpm);
                }
                last_pulse_time = current_time;
            }

            // Reset pulse detection when signal falls below the threshold
            if (val_mv < THRESHOLD) {
                pulse_detected = false;
            }

            previous_val = val_mv;
        }

        k_sleep(K_MSEC(SAMPLE_INTERVAL_MS)); // Sleep before the next reading
    }
}
