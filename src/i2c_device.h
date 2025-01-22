/*
    * i2c_device.h
    *
    *  Created on: 2021. 4. 1.
    * 
    *  Description: A class to handle I2C communication with a device
    *  that is connected to the Zephyr board and exists in the device tree
    *  as a node. This class is a wrapper around the Zephyr I2C API.
*/

#ifndef D_I2C_DEVICE_H_
#define D_I2C_DEVICE_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

#define REPORT_SUCCESS false

bool d_i2c_is_ready(const struct i2c_dt_spec *i2c_dev);
bool d_i2c_write_byte(const struct i2c_dt_spec *i2c_dev, uint8_t address, uint8_t data);
bool d_i2c_read_register(const struct i2c_dt_spec *i2c_dev, uint8_t reg_addr, uint8_t *data);
bool d_i2c_read_registers(const struct i2c_dt_spec *i2c_dev, uint8_t reg_addr, uint8_t *data, size_t len);

#endif