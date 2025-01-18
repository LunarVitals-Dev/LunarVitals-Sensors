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

#define I2C_NODE DT_NODELABEL(i2c0) 
#define I2C_NODE DT_NODELABEL(i2c1) 

// Function to write a byte to a register
int write_register(const struct device *i2c_dev, uint8_t reg_addr, uint8_t data) {
    uint8_t buffer[2] = {reg_addr, data};
    return i2c_write(i2c_dev, buffer, sizeof(buffer), MPU6050_ADDR);
}

// Function to read a byte from a register
int read_register(const struct device *i2c_dev, uint8_t reg_addr, uint8_t *data) {
    return i2c_write_read(i2c_dev, MPU6050_ADDR, &reg_addr, 1, data, 1);
}

// Function to read multiple bytes from sequential registers
int read_registers(const struct device *i2c_dev, uint8_t reg_addr, uint8_t *data, size_t len) {
    return i2c_write_read(i2c_dev, MPU6050_ADDR, &reg_addr, 1, data, len);
}

// Initialize MPU6050 settings
void mpu6050_init(const struct device *i2c_dev) {
    uint8_t device_id;

    // Read device_id register
    if (read_register(i2c_dev, DEVICE_ID, &device_id) != 0 || device_id != 0x68) {
        printk("MPU6050 not detected! (device_id: 0x%02X)\n", device_id);
        return;
    }

    printk("MPU6050 detected (device_id: 0x%02X)\n", device_id);

    // Wake up MPU6050 by writing 0x00 to PWR_MGMT_1
    if (write_register(i2c_dev, PWR_MGMT_1, 0x00) != 0) {
        printk("Failed to wake up MPU6050\n");
    } else {
        printk("MPU6050 initialized successfully\n");
    }
}

// Function to read and print MPU6050 data with string conversion for float
void read_mpu6050_data(const struct device *i2c_dev) {
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    uint8_t data[6];

    // Read accelerometer data (6 bytes)
    if (read_registers(i2c_dev, ACCEL_XOUT_H, data, 6) == 0) {
        accel_x = (int16_t)((data[0] << 8) | data[1]); // Combine high and low byte
        accel_y = (int16_t)((data[2] << 8) | data[3]);
        accel_z = (int16_t)((data[4] << 8) | data[5]);

        // Print raw accelerometer values for debugging
        //printk("Raw Accelerometer (int16_t): X=%d, Y=%d, Z=%d\n", accel_x, accel_y, accel_z);

        float accel_x_float = (float)accel_x / 16384.0f; // Convert to float
        float accel_y_float = (float)accel_y / 16384.0f;
        float accel_z_float = (float)accel_z / 16384.0f;

        printk("Accelerometer (g): X=%.4f, Y=%.4f, Z=%.4f\n", accel_x_float, accel_y_float, accel_z_float);

    } else {
        printk("Failed to read accelerometer data\n");
    }

    // Read gyroscope data (6 bytes)
    if (read_registers(i2c_dev, GYRO_XOUT_H, data, 6) == 0) {
        gyro_x = (int16_t)((data[0] << 8) | data[1]);
        gyro_y = (int16_t)((data[2] << 8) | data[3]);
        gyro_z = (int16_t)((data[4] << 8) | data[5]);

        // Print raw gyroscope values for debugging
        //printk("Raw Gyroscope (int16_t): X=%d, Y=%d, Z=%d\n", gyro_x, gyro_y, gyro_z);
        float gyro_x_float = (float)gyro_x / 131.0f; // Convert to float
        float gyro_y_float = (float)gyro_y / 131.0f;
        float gyro_z_float = (float)gyro_z / 131.0f;

        printk("Gyroscope (°/s): X=%.4f, Y=%.4f, Z=%.4f\n", gyro_x_float, gyro_y_float, gyro_z_float);

    } else {
        printk("Failed to read gyroscope data\n");
    }
}

// Function to read a 16-bit value from MLX90614
int read_mlx90614_register(const struct device *i2c_dev, uint8_t reg_addr, uint16_t *data) {
    uint8_t buffer[3];
    int ret = i2c_write_read(i2c_dev, MLX90614_ADDR, &reg_addr, 1, buffer, 3);
    if (ret < 0) {
        return ret;
    }
    *data = (buffer[0] | (buffer[1] << 8)); // Combine high and low byte
    return 0;
}

// Function to read and print temperature from MLX90614
void read_mlx90614_data(const struct device *i2c_dev) {
    uint16_t ambient_temp_raw, object_temp_raw;
    float ambient_temp, object_temp;

    // Read ambient temperature
    if (read_mlx90614_register(i2c_dev, MLX90614_TA, &ambient_temp_raw) == 0) {
        ambient_temp = ambient_temp_raw * 0.02 - 273.15; // Convert to Celsius
        printk("Ambient Temperature: %.2f °C\n", ambient_temp);
    } else {
        printk("Failed to read ambient temperature\n");
    }

    // Read object temperature
    if (read_mlx90614_register(i2c_dev, MLX90614_TOBJ1, &object_temp_raw) == 0) {
        object_temp = object_temp_raw * 0.02 - 273.15; // Convert to Celsius
        printk("Object Temperature: %.2f °C\n", object_temp);
    } else {
        printk("Failed to read object temperature\n");
    }
}

// Function to write a byte to a register
int write_register_BMP(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    uint8_t buffer[2] = {reg_addr, data};
    return i2c_write(i2c_dev, buffer, sizeof(buffer), dev_addr);
}

// Function to read multiple bytes from sequential registers
int read_registers_BMP(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    return i2c_write_read(i2c_dev, dev_addr, &reg_addr, 1, data, len);
}

// Initialize BMP280
void bmp280_init(const struct device *i2c_dev) {
    uint8_t chip_id;

    // Read Chip ID
    if (read_registers_BMP(i2c_dev, BMP280_ADDR, BMP280_REG_CHIPID, &chip_id, 1) != 0 || chip_id != 0x58) {
        printk("Error: BMP280 not detected or invalid Chip ID\n");
        return;
    }
    printk("BMP280 detected. Chip ID: 0x%x\n", chip_id);

    // Reset the sensor
    write_register_BMP(i2c_dev, BMP280_ADDR, BMP280_REG_SOFTRESET, 0xB6);
    k_sleep(K_MSEC(100));  // Allow time for reset

    // Read calibration data
    uint8_t calib_data[24];
    if (read_registers_BMP(i2c_dev, BMP280_ADDR, BMP280_REG_CALIB_START, calib_data, sizeof(calib_data)) != 0) {
        printk("Error: Failed to read calibration data\n");
        return;
    }

    // Parse calibration data
    dig_T1 = (calib_data[1] << 8) | calib_data[0];
    dig_T2 = (calib_data[3] << 8) | calib_data[2];
    dig_T3 = (calib_data[5] << 8) | calib_data[4];
    dig_P1 = (calib_data[7] << 8) | calib_data[6];
    dig_P2 = (calib_data[9] << 8) | calib_data[8];
    dig_P3 = (calib_data[11] << 8) | calib_data[10];
    dig_P4 = (calib_data[13] << 8) | calib_data[12];
    dig_P5 = (calib_data[15] << 8) | calib_data[14];
    dig_P6 = (calib_data[17] << 8) | calib_data[16];
    dig_P7 = (calib_data[19] << 8) | calib_data[18];
    dig_P8 = (calib_data[21] << 8) | calib_data[20];
    dig_P9 = (calib_data[23] << 8) | calib_data[22];

    // Configure sensor: normal mode, oversampling x1 for temp & pressure
    uint8_t ctrl_meas = (0x01 << 5) | (0x01 << 2) | 0x03;
    write_register_BMP(i2c_dev, BMP280_ADDR, BMP280_REG_CONTROL, ctrl_meas);
    write_register_BMP(i2c_dev, BMP280_ADDR, BMP280_REG_CONFIG, 0);  // Default config
}

// Read and process BMP280 data
void read_bmp280_data(const struct device *i2c_dev) {
    uint8_t data[6];

    if (read_registers_BMP(i2c_dev, BMP280_ADDR, BMP280_REG_PRESSURE_MSB, data, sizeof(data)) != 0) {
        printk("Error: Failed to read BMP280 data\n");
        return;
    }

    int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
    int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);

    // Temperature compensation
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    float celsius = ((t_fine * 5 + 128) >> 8) / 100.0f;
    float fahrenheit =(celsius *  1.8) + 32.0;

    // Pressure compensation
    int64_t var1_p = ((int64_t)t_fine) - 128000;
    int64_t var2_p = var1_p * var1_p * (int64_t)dig_P6 + ((var1_p * (int64_t)dig_P5) << 17) + (((int64_t)dig_P4) << 35);
    var1_p = (((var1_p * var1_p * (int64_t)dig_P3) >> 8) + ((var1_p * (int64_t)dig_P2) << 12));
    var1_p = ((((int64_t)1 << 47) + var1_p) * (int64_t)dig_P1) >> 33;

    if (var1_p == 0) {
        printk("Error: Division by zero in pressure calculation\n");
        return;
    }

    int64_t p = ((((int64_t)1048576 - adc_P) << 31) - var2_p) * 3125 / var1_p;
    var1_p = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2_p = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1_p + var2_p) >> 8) + (((int64_t)dig_P7) << 4);

    float pressure = p / 25600.0f;  // Convert to hPa

    printk("Temperature: %.2f °C / %.2f °F, Pressure: %.2f hPa\n", celsius, fahrenheit, pressure);
}

// Main function
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
