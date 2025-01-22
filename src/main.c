/*
 * Copyright (c) 2024 Open Pixel Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// TODO: turn the max30102 driver into a separate header file and include it here
// TODO: create a skeleton i2c header file and make the max30102 driver inherit from it

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

#include "i2c_device.h"
#include "MAX30102.h"
#include "heart_rate.h"

static const struct i2c_dt_spec dev_max30102 = MAX30102_DT_SPEC;

// static char last_byte;

// #define DATA_BUFFER_SIZE 32
// typedef struct buffer {
// 	uint32_t red[DATA_BUFFER_SIZE];
// 	uint32_t ir[DATA_BUFFER_SIZE];
// 	uint8_t head_ptr;
// 	uint8_t tail_ptr;
// } sensor_struct;

// sensor_struct sensor_data;

// /*
//  * @brief Callback which is called when a write request is received from the master.
//  * @param config Pointer to the target configuration.
//  */
// int sample_target_write_requested_cb(struct i2c_target_config *config)
// {
// 	printk("sample target write requested\n");
// 	return 0;
// }

// /*
//  * @brief Callback which is called when a write is received from the master.
//  * @param config Pointer to the target configuration.
//  * @param val The byte received from the master.
//  */
// int sample_target_write_received_cb(struct i2c_target_config *config, uint8_t val)
// {
// 	printk("sample target write received: 0x%02x\n", val);
// 	last_byte = val;
// 	return 0;
// }

// /*
//  * @brief Callback which is called when a read request is received from the master.
//  * @param config Pointer to the target configuration.
//  * @param val Pointer to the byte to be sent to the master.
//  */
// int sample_target_read_requested_cb(struct i2c_target_config *config, uint8_t *val)
// {
// 	printk("sample target read request: 0x%02x\n", *val);
// 	*val = 0x42;
// 	return 0;
// }

// /*
//  * @brief Callback which is called when a read is processed from the master.
//  * @param config Pointer to the target configuration.
//  * @param val Pointer to the next byte to be sent to the master.
//  */
// int sample_target_read_processed_cb(struct i2c_target_config *config, uint8_t *val)
// {
// 	printk("sample target read processed: 0x%02x\n", *val);
// 	*val = 0x43;
// 	return 0;
// }

// /*
//  * @brief Callback which is called when the master sends a stop condition.
//  * @param config Pointer to the target configuration.
//  */
// int sample_target_stop_cb(struct i2c_target_config *config)
// {
// 	printk("sample target stop callback\n");
// 	return 0;
// }

// static struct i2c_target_callbacks sample_target_callbacks = {
// 	.write_requested = sample_target_write_requested_cb,
// 	.write_received = sample_target_write_received_cb,
// 	.read_requested = sample_target_read_requested_cb,
// 	.read_processed = sample_target_read_processed_cb,
// 	.stop = sample_target_stop_cb,
// };

// int write_byte(uint8_t address, uint8_t data)
// {
// 	int ret;
// 	ret = i2c_reg_write_byte_dt(&dev_i2c0, address, data);
// 	if (ret < 0) {
// 		printk("Failed to write byte %x to register %x\n", data, address);
// 		return -1;
// 	} else {
// 		printk("Wrote byte %2x to register %2x\n", data, address);
// 		return 0;
// 	}
// }

// int read_register(const struct i2c_dt_spec *i2c_dev, uint8_t reg_addr, uint8_t *data) {
// 	int ret = i2c_write_read_dt(i2c_dev, &reg_addr, 1, data, 1);
// 	if (ret < 0) {
// 		printk("Failed to read register %x\n", reg_addr);
// 		return -1;
// 	} else {
// 		printk("Read register %x\n", reg_addr);
// 		return 0;
// 	}
// }

// // Function to read multiple bytes from sequential registers
// int read_registers(const struct i2c_dt_spec *i2c_dev, uint8_t reg_addr, uint8_t *data, size_t len) {
//     int ret = i2c_write_read_dt(i2c_dev, &reg_addr, 1, data, len);
// 	if (ret < 0) {
// 		printk("Failed to read %d registers starting from %x\n", len, reg_addr);
// 		return -1;
// 	} else {
// 		//printk("Read %d registers starting from %x\n", len, reg_addr);
// 		return 0;
// 	}
// }

// typedef enum{
// 	HEART_RATE = 2,
// 	SPO2 = 3,
// 	MULTI_LED = 7
// } MAX30102_mode_t;

// /*
// 	 * @brief Setup the pulse oximeter
// 	 * @param sample_avg The number of samples to average. Must be a multiple of 2 between 1 and 32 - default 4
// 	 * @param fifo_rollover Whether the FIFO should rollover. Must be true or false - default true
// 	 * @param fifo_int_threshold The FIFO interrupt threshold. Must be between 0 and 15 - default 4
// 	 * @param mode The mode of the pulse oximeter. Must be one of HEART_RATE, SPO2, or MULTI_LED - default MULTI_LED
// 	 * @param sample_rate The sample rate of the ADC. Must be one of 50, 100, 200, 400, 800, 1000, 1600, 3200 - default 50
// 	 * @param pulse_width The pulse width of the LED. Must be one of 69, 118, 215, 411 - default 69
// 	 * @param adc_range The range of the ADC. Must be one of 2048, 4096, 8192, 16384 - default 16384
//  */
// void pulse_oximeter_setup(uint8_t sample_avg, bool fifo_rollover, uint8_t fifo_int_threshold, MAX30102_mode_t mode, int sample_rate, int pulse_width, int adc_range)
// {
// 	// TODO: add support for parameters
// 	uint8_t address, data;
// 	if (!i2c_is_ready_dt(&dev_i2c0)) {
// 		printk("I2C bus is not ready\n");
// 	}

// 	// reset the device
// 	if (write_byte(0x09, 0x40) < 0) {
// 		printk("Failed to reset device\n");
// 		exit(-1);
// 	}

// 	// switch(sample_avg){
// 	// 	case 1:
// 	// 		data = 0x00; // 000_x_0000
// 	// 		break;
// 	// }

// 	// // sample avg = 4, rollover disabled, almost full = 17 // 010_0_1111
// 	address = 0x08;
// 	data = 0x0F; // 010_0_1111
// 	write_byte(address, data);

// 	//mode = multi led // 0_0_xxx_111
// 	//mode = sp02 // 0_0_xxx_011
// 	address = 0x09;
// 	data = 0x03; 
// 	write_byte(address, data);

// 	//adc range = 16384, sample rate = 50, pulse width = 69 // x11_000_00
// 	//SP02 ADC = 4096, sample rate = 100, pulse width = 411 // x01_001_11
// 	address = 0x0A;
// 	data = 0x27; 
// 	write_byte(address, data);
	
// 	// set power level to 6.2mA (0x1F)
// 	address = 0x0C;
// 	data = 0x1F;
// 	write_byte(address, data);

// 	address = 0x0D;
// 	data = 0x1F;
// 	write_byte(address, data);

// 	// // multi_led control registers
// 	// address = 0x11; 
// 	// data = 0x07; // x_001_x_010 (RED and IR LED)
// 	// write_byte(address, data);

// 	// address = 0x12;
// 	// data = 0x07; // x_000_x_000 (NO LED)
// 	// write_byte(address, data);

// 	// clear fifo
// 	write_byte(0x04, 0x00);
// 	write_byte(0x05, 0x00);
// 	write_byte(0x06, 0x00);

// }

// int get_data(void)
// {
// 	// uint8_t read_ptr, write_ptr, ovf_ctr;
// 	// read_register(&dev_i2c0, 0x04, &read_ptr);
// 	// read_register(&dev_i2c0, 0x05, &ovf_ctr);
// 	// read_register(&dev_i2c0, 0x06, &write_ptr);

// 	// if (read_ptr == write_ptr) {
// 	// 	printk("FIFO is empty\n");
// 	// 	printk("Read pointer: %d, Write pointer: %d, Overflow counter: %d\n", read_ptr, write_ptr, ovf_ctr);
// 	// 	return 0;
// 	// } else {
// 	// 	printk("FIFO is not empty\n");
// 	// 	printk("Read pointer: %d, Write pointer: %d, Overflow counter: %d\n", read_ptr, write_ptr, ovf_ctr);
// 	// 	int num_samples_to_read = (write_ptr - read_ptr) & 0x1F; // mask to get the lower 5 bits of the difference

// 	// 	// know the number of samples we need to read
// 	// 	int num_bytes_to_read = num_samples_to_read * 6; // 3 bytes for red and 3 bytes for ir for each sample

// 	// 	// prepare to read data from the fifo buffer address many times


// 	// 	uint8_t data[num_bytes_to_read];
// 	// 	read_registers(&dev_i2c0, 0x07, data, num_bytes_to_read);

// 	// 	for (int i = 0; i < num_bytes_to_read; i += 6) {
// 	// 		sensor_data.head_ptr ++;
// 	// 		sensor_data.head_ptr %= DATA_BUFFER_SIZE;
			
// 	// 		sensor_data.red[sensor_data.head_ptr] = ((data[i] << 16) | (data[i + 1] << 8) | data[i + 2]) & 0x3FFFF;
// 	// 		sensor_data.ir[sensor_data.head_ptr] = ((data[i + 3] << 16) | (data[i + 4] << 8) | data[i + 5]) & 0x3FFFF;
// 	// 	}
// 	// 	return num_samples_to_read;
// 	// }

// 	uint8_t data[6];
// 	int ret = read_registers(&dev_i2c0, 0x07, data, 6);
// 	if (ret == 0){
// 		sensor_data.head_ptr ++;
// 		sensor_data.head_ptr %= DATA_BUFFER_SIZE;	
// 		sensor_data.red[sensor_data.head_ptr] = ((data[0] << 16) | (data[1] << 8) | data[2]) & 0x3FFFF;
// 		sensor_data.ir[sensor_data.head_ptr] = ((data[3] << 16) | (data[4] << 8) | data[5]) & 0x3FFFF;
// 	}
// 	return 6;
// }

int main(void)
{
	// struct i2c_target_config target_cfg = {
	// 	.address = 0x60,
	// 	.callbacks = &sample_target_callbacks,
	// };

	printk("i2c custom target sample\n");

	max30102_default_setup(&dev_max30102);

	max30102_read_data(&dev_max30102);

	// const uint8_t RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
	// uint8_t rates[RATE_SIZE]; //Array of heart rates
	// uint8_t rateSpot = 0;
	// long lastBeat = 0; //Time at which the last beat occurred

	// float beatsPerMinute;
	// int beatAvg;

	// while(true){
	// 	if (get_k("No data\n");
	// 	} else {
	// 		long irVal = sensor_data.ir[sensor_data.head_ptr];

	// 		if (checkForBeat(irVal) == true) {
    // 			//We sensed a beat!
    // 			long delta = k_uptime_get() - lastBeat;
    // 			lastBeat = k_uptime_get();
			
    // 			beatsPerMinute = 60 / (delta / 1000.0);
			
    // 			if (beatsPerMinute < 255 && beatsPerMinute > 20) {
    // 			  rates[rateSpot++] = (uint8_t)beatsPerMinute; //Store this reading in the array
    // 			  rateSpot %= RATE_SIZE; //Wrap variable
			
    // 			  //Take average of readings
    // 			  beatAvg = 0;
    // 			  for (uint8_t x = 0 ; x < RATE_SIZE ; x++)
    // 			    beatAvg += rates[x];
    // 			  beatAvg /= RATE_SIZE;
    // 			}
  	// 		}

	// 		printk(">Red:%d,IR:%d,BPM:%f,AvgBPM:%f\n", sensor_data.red[sensor_data.head_ptr], sensor_data.ir[sensor_data.head_ptr], beatsPerMinute, beatAvg);
	// 	}
	// 	k_sleep(K_MSEC(10));
	// }data() == 0){
	// 		print

	return 0;
}
