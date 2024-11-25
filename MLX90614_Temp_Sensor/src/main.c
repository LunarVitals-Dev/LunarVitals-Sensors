#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>
#include <stdlib.h>

// MLX90614 I2C address
#define I2C_NODE DT_NODELABEL(i2c0) // I2C instance
#define MLX90614_ADDR 0x5A // Default I2C address of MLX90614

// MLX90614 Registers
#define MLX90614_TA 0x06 // Ambient temperature register
#define MLX90614_TOBJ1 0x07 // Object 1 temperature register

// Function to read a 16-bit value from MLX90614
int read_mlx90614_register(const struct device *i2c_dev, uint8_t reg_addr, uint16_t *data) {
    uint8_t buffer[3];
    int ret = i2c_write_read(i2c_dev, MLX90614_ADDR, &reg_addr, 1, buffer, 3);
    if (ret < 0) {
        return ret;
    }
    *data = (buffer[0] | (buffer[1] << 8)); // Combine high and low byte
    return 0;
}

// Function to read and print temperature from MLX90614
void read_mlx90614_data(const struct device *i2c_dev) {
    uint16_t ambient_temp_raw, object_temp_raw;
    float ambient_temp, object_temp;

    // Read ambient temperature
    if (read_mlx90614_register(i2c_dev, MLX90614_TA, &ambient_temp_raw) == 0) {
        ambient_temp = ambient_temp_raw * 0.02 - 273.15; // Convert to Celsius
        printk("Ambient Temperature: %.2f °C\n", ambient_temp);
    } else {
        printk("Failed to read ambient temperature\n");
    }

    // Read object temperature
    if (read_mlx90614_register(i2c_dev, MLX90614_TOBJ1, &object_temp_raw) == 0) {
        object_temp = object_temp_raw * 0.02 - 273.15; // Convert to Celsius
        printk("Object Temperature: %.2f °C\n", object_temp);
    } else {
        printk("Failed to read object temperature\n");
    }
}

// Main function
void main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);

    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return;
    }

    while (1) {
        read_mlx90614_data(i2c_dev); // Read MLX90614 data
        k_msleep(3000);  // Delay for 3 seconds
    }
}