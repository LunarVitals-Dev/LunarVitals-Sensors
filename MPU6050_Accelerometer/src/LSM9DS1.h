#ifndef LSM9DS1_H
#define LSM9DS1_H

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// LSM9DS1 Register Addresses
#define LSM9DS1_AG_ADDR 0x6B // I2C address of the LSM9DS1 Accelerometer/Gyroscope
#define LSM9DS1_M_ADDR  0x1E // I2C address of the magnetometer
#define WHO_AM_I_REG 0x0F // WHO_AM_I register address
#define CTRL_REG1_G  0x10 // Gyro control register 1
#define CTRL_REG6_XL 0x20 // Accelerometer control register 6
#define OUT_X_L_G  0x18 // Gyro X-axis output low byte
#define OUT_X_L_XL 0x28 // Accelerometer X-axis output low byte
#define OUT_X_L_M  0x28 // Magnetometer X-axis output low byte

#endif