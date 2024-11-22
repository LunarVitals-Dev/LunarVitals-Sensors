#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>
#include <stdlib.h>

#define I2C_NODE DT_NODELABEL(i2c0) // I2C instance
#define MPU6050_ADDR 0x68          // Default I2C address of MPU6050

// MPU6050 Registers
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43


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
    uint8_t who_am_i;

    // Read WHO_AM_I register
    if (read_register(i2c_dev, WHO_AM_I_REG, &who_am_i) != 0 || who_am_i != 0x68) {
        printk("MPU6050 not detected! (WHO_AM_I: 0x%02X)\n", who_am_i);
        return;
    }

    printk("MPU6050 detected (WHO_AM_I: 0x%02X)\n", who_am_i);

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
        float gyro_x_float = (float)gyro_x / 16384.0f; // Convert to float
        float gyro_y_float = (float)gyro_y / 16384.0f;
        float gyro_z_float = (float)gyro_z / 16384.0f;

        printk("Gyroscope (°/s): X=%.4f, Y=%.4f, Z=%.4f\n", gyro_x_float, gyro_y_float, gyro_z_float);

    } else {
        printk("Failed to read gyroscope data\n");
    }
}



// Main function
void main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);

    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not ready\n");
        return;
    }

    mpu6050_init(i2c_dev);

    while (1) {
        read_mpu6050_data(i2c_dev);
        k_msleep(3000);  // Delay for 1 second
    }
}
