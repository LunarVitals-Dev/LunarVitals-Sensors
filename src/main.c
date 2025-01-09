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

int32_t convert_to_mv(int16_t raw_value) {
    return (int32_t)((raw_value * ADC_REF_VOLTAGE_MV * (1.0 / ADC_GAIN)) / ADC_RESOLUTION);
}
void main(void)
{
    /* Verify ADC readiness */
    if (!adc_is_ready_dt(&adc_channel)) {
        printf("ADC controller device %s not ready\n", adc_channel.dev->name);
        return;
    }

    /* Configure ADC channel */
    int err = adc_channel_setup_dt(&adc_channel);
    if (err < 0) {
        printf("Could not setup ADC channel (%d)\n", err);
        return;
    }

    /* Initialize ADC sequence */
    err = adc_sequence_init_dt(&adc_channel, &sequence);
    if (err < 0) {
        printf("Could not initialize ADC sequence (%d)\n", err);
        return;
    }

    /* Continually read from the ADC */
    while (1) {
        err = adc_read(adc_channel.dev, &sequence);
        if (err < 0) {
            printf("ADC read failed (%d)\n", err);
        } else {
            int32_t val_mv = convert_to_mv(buf);
            //printf("Raw ADC value: %d\n", buf);
            uint32_t timestamp = k_uptime_get(); // Get the current uptime in milliseconds
            printf("%u %d\n", timestamp, val_mv);
           
        }
        /* Add a delay to avoid overwhelming the system (e.g., 1 second) */
        k_sleep(K_MSEC(100));  // Sleep for 1 second before reading again
    }
}