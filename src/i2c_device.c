/*
    * i2c_device.c
    *
    *  Created on: 2021. 4. 1.
    * 
    *  Description: A class to handle I2C communication with a device
    *  that is connected to the Zephyr board and exists in the device tree
    *  as a node. This class is a wrapper around the Zephyr I2C API.
    * 
    * TODO: generalize more
*/

#include "i2c_device.h"

bool d_i2c_is_ready(const struct i2c_dt_spec *i2c_dev) {
    if (!i2c_is_ready_dt(i2c_dev)){
        printk("I2C bus is not ready\n");
        return false;
    } else {
        return true;
    }
}

bool d_i2c_write_byte(const struct i2c_dt_spec *i2c_dev, uint8_t address, uint8_t data)
{
	int ret;
	ret = i2c_reg_write_byte_dt(i2c_dev, address, data);
	if (ret < 0) {
		printk("Failed to write byte %x to register %x\n", data, address);
		return false;
	} else {
		if(REPORT_SUCCESS) printk("Wrote byte %2x to register %2x\n", data, address);
		return true;
	}
}

bool d_i2c_read_register(const struct i2c_dt_spec *i2c_dev, uint8_t reg_addr, uint8_t *data) {
    int ret = i2c_write_read_dt(i2c_dev, &reg_addr, 1, data, 1);
    if (ret < 0) {
        printk("Failed to read register %x\n", reg_addr);
        return false;
    } else {
        if(REPORT_SUCCESS) printk("Read register %x\n", reg_addr);
        return true;
    }
}

bool d_i2c_read_registers(const struct i2c_dt_spec *i2c_dev, uint8_t reg_addr, uint8_t *data, size_t len) {
    int ret = i2c_write_read_dt(i2c_dev, &reg_addr, 1, data, len);
    if (ret < 0) { 
        printk("Failed to read %d registers starting from %x\n", len, reg_addr);
        return false;
    } else {
        if(REPORT_SUCCESS) printk("Read %d registers starting from %x\n", len, reg_addr);
        return true;
    }
}