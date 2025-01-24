#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "MPU6050.h"
#include "MLX90614.h"
#include "BMP280.h"
#include "i2c.h"
#include "i2c.h"
#include "MAX30102.h"
#include "heart_rate.h"

static const struct i2c_dt_spec dev_max30102 = MAX30102_DT_SPEC;


int main(void) {
    i2c_init();
    max30102_default_setup(&dev_max30102);

    int ms_counter = 0; // Millisecond counter

    while (1) {
        // Read MAX30102 data every millisecond
        max30102_read_data(&dev_max30102);
        k_msleep(10);  // 1 ms delay

        // Increment the millisecond counter
        ms_counter += 1;

        // Every second, print the accumulated data
        if (ms_counter >= 200) {
            ms_counter = 0; // Reset counter
            i2c_read_data();  // Read and print other sensors
            max30102_print_data();  // Print MAX30102 data
        }
    }

    return 0;
}

