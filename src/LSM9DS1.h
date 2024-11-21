#ifndef LSM9DS1_H
#define LSM9DS1_H

#include <zephyr/device.h>
#include <stdint.h>

// I2C device node label and LSM9DS1 I2C address
#define I2C_NODE DT_NODELABEL(i2c0)
#define LSM9DS1_ADDR 0x6B // I2C address of the LSM9DS1 Accelerometer/Gyroscope

// Register addresses
#define WHO_AM_I_REG 0x0F // WHO_AM_I register address
#define CTRL_REG1_G  0x10 // Gyro control register 1

#define OUT_X_L_G  0x18 // Gyro X-axis output low byte
#define OUT_X_L_XL 0x28 // Accelerometer X-axis output low byte
#define OUT_X_L_M  0x28 // Magnetometer X-axis output low byte

// Function prototypes
int write_register_LSM(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
int read_register_LSM(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data);
int read_registers_LSM(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);
void lsm9ds1_init(const struct device *i2c_dev);
void read_lsm9ds1_data(const struct device *i2c_dev);

#endif // LSM9DS1_H