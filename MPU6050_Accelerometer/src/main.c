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


int main(void) {
    i2c_init();
    while (1) {
        i2c_read_data();

        k_msleep(1000);  
    }
    return 0;
}
