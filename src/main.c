#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#include <zephyr/kernel.h>
#include <stdint.h>

#include "LSM9DS1.h"
#include "BMP280.h"

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);

    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not found\n");
        return 0;
    }

    // Initialize the LSM9DS1 sensor
    lsm9ds1_init(i2c_dev);
    bmp280_init(i2c_dev);

    while (1) {
        read_lsm9ds1_data(i2c_dev);
        read_bmp280_data(i2c_dev);
        k_sleep(K_MSEC(2000));
    }

    return -1;
}
