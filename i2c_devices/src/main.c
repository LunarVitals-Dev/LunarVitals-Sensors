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


int main(void) {
    const struct device *i2c_dev0 = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    const struct device *i2c_dev1 = DEVICE_DT_GET(DT_NODELABEL(i2c1));

    if (!device_is_ready(i2c_dev0)) {
        printk("I2C0 device not ready\n");
        return -1;
    }

    if (!device_is_ready(i2c_dev1)) {
        printk("I2C1 device not ready\n");
        return -1;
    }

    mpu6050_init(i2c_dev0);
    bmp280_init(i2c_dev1);

    while (1) {
        read_mpu6050_data(i2c_dev0);
        read_mlx90614_data(i2c_dev0);
        read_bmp280_data(i2c_dev1);

        k_msleep(1000);  
    }

    return 0;
}
