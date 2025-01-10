#ifndef MLX90614_H
#define MLX90614_H

#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// MLX90614 Registers
#define MLX90614_ADDR 0x5A // Default I2C address of MLX90614
#define MLX90614_TA 0x06 // Ambient temperature register
#define MLX90614_TOBJ1 0x07 // Object 1 temperature register

#endif