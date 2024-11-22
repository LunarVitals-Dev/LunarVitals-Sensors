#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define DS18B20_PIN 3
#define DS18B20_CONVERT_T       0x44
#define DS18B20_READ_SCRATCHPAD 0xBE
#define DS18B20_SKIP_ROM        0xCC

static const struct device *gpio_dev;

static int ds18b20_reset(void)
{
    int presence;
    
    // Configure as strong output
    gpio_pin_configure(gpio_dev, DS18B20_PIN, GPIO_OUTPUT | GPIO_PUSH_PULL);
    
    // Strong pull down
    gpio_pin_set(gpio_dev, DS18B20_PIN, 0);
    k_busy_wait(480);

    // Release and switch to input
    gpio_pin_configure(gpio_dev, DS18B20_PIN, GPIO_INPUT | GPIO_PULL_UP);
    k_busy_wait(70);

    // Read presence
    presence = gpio_pin_get(gpio_dev, DS18B20_PIN);
    k_busy_wait(410);

    return presence ? -1 : 0;
}

static void ds18b20_write_bit(uint8_t bit)
{
    gpio_pin_configure(gpio_dev, DS18B20_PIN, GPIO_OUTPUT | GPIO_PUSH_PULL);
    gpio_pin_set(gpio_dev, DS18B20_PIN, 0);
    
    if (bit) {
        k_busy_wait(5);
        gpio_pin_set(gpio_dev, DS18B20_PIN, 1);
        k_busy_wait(55);
    } else {
        k_busy_wait(60);
        gpio_pin_set(gpio_dev, DS18B20_PIN, 1);
        k_busy_wait(5);
    }
}

static uint8_t ds18b20_read_bit(void)
{
    uint8_t bit;
    
    gpio_pin_configure(gpio_dev, DS18B20_PIN, GPIO_OUTPUT | GPIO_PUSH_PULL);
    gpio_pin_set(gpio_dev, DS18B20_PIN, 0);
    k_busy_wait(5);
    
    gpio_pin_configure(gpio_dev, DS18B20_PIN, GPIO_INPUT | GPIO_PULL_UP);
    k_busy_wait(5);
    
    bit = gpio_pin_get(gpio_dev, DS18B20_PIN);
    k_busy_wait(50);
    
    return bit;
}

static void ds18b20_write_byte(uint8_t byte)
{
    for (int i = 0; i < 8; i++) {
        ds18b20_write_bit(byte & (1 << i));
    }
}

static uint8_t ds18b20_read_byte(void)
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        if (ds18b20_read_bit()) {
            byte |= (1 << i);
        }
    }
    return byte;
}

static int ds18b20_init(void)
{
    gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));
    if (!gpio_dev) {
        return -1;
    }

    // Initial pin setup
    int ret = gpio_pin_configure(gpio_dev, DS18B20_PIN, 
                               GPIO_OUTPUT | GPIO_PUSH_PULL);
    if (ret < 0) {
        return -1;
    }
    
    // Set initial state high
    gpio_pin_set(gpio_dev, DS18B20_PIN, 1);
    k_sleep(K_MSEC(100));

    ret = ds18b20_reset();
    if (ret < 0) {
        return -1;
    }
    
    return 0;
}

static float ds18b20_read_temp(void)
{
    int16_t temp;
    uint8_t temp_l, temp_h;
    
    ds18b20_reset();
    ds18b20_write_byte(DS18B20_SKIP_ROM);
    ds18b20_write_byte(DS18B20_CONVERT_T);
    
    k_sleep(K_MSEC(750));
    
    ds18b20_reset();
    ds18b20_write_byte(DS18B20_SKIP_ROM);
    ds18b20_write_byte(DS18B20_READ_SCRATCHPAD);
    
    temp_l = ds18b20_read_byte();
    temp_h = ds18b20_read_byte();
    
    temp = (temp_h << 8) | temp_l;
    float celsius = temp * 0.0625f;
    
    if (celsius < -55.0f || celsius > 125.0f) {
        return 0.0f;
    }
    
    return celsius;
}

int main(void)
{
    if (ds18b20_init() < 0) {
        printk("Sensor initialization failed\n");
        return -1;
    }

    printk("Sensor initialized successfully\n");

    while (1) {
        float temp = ds18b20_read_temp();
        int temp_whole = (int)temp;
        int temp_frac = (int)((temp - temp_whole) * 100);
        if (temp_frac < 0) temp_frac = -temp_frac;
        
        printk("Temperature: %d.%02d°C\n", temp_whole, temp_frac);
        k_sleep(K_MSEC(1000));
    }
    return 0;
}

// #include <zephyr/kernel.h>
// #include <zephyr/drivers/adc.h>
// #include <zephyr/sys/printk.h>

// // ADC definitions
// #define ADC_NODE        DT_NODELABEL(adc)
// #define ADC_CHANNEL     3  // Using GPIO P0.03/AIN3
// #define ADC_RESOLUTION  12
// #define ADC_GAIN        ADC_GAIN_1_6
// #define ADC_REFERENCE   ADC_REF_INTERNAL
// #define ADC_ACQUISITION_TIME ADC_ACQ_TIME_DEFAULT

// static const struct device *adc_dev;
// static const struct adc_channel_cfg channel_cfg = {
//     .gain = ADC_GAIN,
//     .reference = ADC_REFERENCE,
//     .acquisition_time = ADC_ACQUISITION_TIME,
//     .channel_id = ADC_CHANNEL,
//     .input_positive = SAADC_CH_PSELP_PSELP_AnalogInput3
// };

// static struct adc_sequence sequence = {
//     .channels = BIT(ADC_CHANNEL),
//     .buffer = NULL,
//     .buffer_size = 0,
//     .resolution = ADC_RESOLUTION,
// };

// int main(void)
// {
//     int err;
//     int16_t buf;
    
//     // Get ADC device
//     adc_dev = DEVICE_DT_GET(ADC_NODE);
//     if (!device_is_ready(adc_dev)) {
//         printk("ADC device not ready\n");
//         return -1;
//     }

//     // Configure ADC channel
//     err = adc_channel_setup(adc_dev, &channel_cfg);
//     if (err < 0) {
//         printk("Error in adc setup: %d\n", err);
//         return -1;
//     }

//     // Configure sequence
//     sequence.buffer = &buf;
//     sequence.buffer_size = sizeof(buf);

//     printk("MyoWare sensor ready\n");

//     while (1) {
//         // Read ADC value
//         err = adc_read(adc_dev, &sequence);
//         if (err < 0) {
//             printk("ADC read error: %d\n", err);
//             continue;
//         }

//         // Convert to millivolts
//         int32_t mv = buf;
//         adc_raw_to_millivolts(adc_ref_internal(adc_dev), ADC_GAIN,
//                              ADC_RESOLUTION, &mv);

//         printk("Muscle Voltage: %d mV\n", mv);
//         k_sleep(K_MSEC(100));  // Read every 100ms
//     }
//     return 0;
// }