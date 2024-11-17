#ifndef BMP280_H
#define BMP280_H

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <stdint.h>

// I2C device node label and LSM9DS1 I2C address
#define I2C_NODE DT_NODELABEL(i2c0)
#define BMP280_ADDR 0x76  // Change to 0x77 if SDO pin is high

// BMP280 Register Addresses
#define BMP280_REG_CALIB_START    0x88
#define BMP280_REG_CHIPID         0xD0
#define BMP280_REG_SOFTRESET      0xE0
#define BMP280_REG_STATUS         0xF3
#define BMP280_REG_CONTROL        0xF4
#define BMP280_REG_CONFIG         0xF5
#define BMP280_REG_PRESSURE_MSB   0xF7
#define BMP280_REG_TEMPERATURE_MSB 0xFA

// Function prototypes
int write_register(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
int read_register(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data);
int read_registers(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);
void bmp280_init(const struct device *i2c_dev);
void read_bmp280_data(const struct device *i2c_dev);

#endif