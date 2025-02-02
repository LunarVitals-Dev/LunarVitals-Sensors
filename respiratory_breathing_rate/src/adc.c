#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <stdio.h>

#define ADC_CHANNEL_COUNT 8

/* ADC channel configuration */
static const struct adc_dt_spec adc_channels[] = {
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 2),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 3),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 4),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 5),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 6),
    ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 7),
};

/* ADC buffer */
static int16_t buf[ADC_CHANNEL_COUNT];

void main(void) {
    struct adc_sequence sequence;
    for (int i = 0; i < ADC_CHANNEL_COUNT; i++) {
        const struct adc_dt_spec *adc_channel = &adc_channels[i];

        if (!adc_is_ready_dt(adc_channel)) {
            printf("ADC channel %d not ready\n", i);
            continue;
        }

        int err = adc_channel_setup_dt(adc_channel);
        if (err < 0) {
            printf("Failed to setup ADC channel %d (%d)\n", i, err);
            continue;
        }

        sequence.buffer = &buf[i];
        sequence.buffer_size = sizeof(buf[i]);
        err = adc_sequence_init_dt(adc_channel, &sequence);
        if (err < 0) {
            printf("Failed to initialize ADC sequence for channel %d (%d)\n", i, err);
            continue;
        }

        err = adc_read(adc_channel->dev, &sequence);
        if (err < 0) {
            printf("ADC read failed for channel %d (%d)\n", i, err);
            continue;
        }

        printf("Channel %d: Raw ADC value = %d\n", i, buf[i]);
    }
}