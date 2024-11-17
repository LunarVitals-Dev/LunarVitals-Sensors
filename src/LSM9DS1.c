#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#define I2C_NODE DT_NODELABEL(i2c0)
#define LSM9DS1_AG_ADDR 0x6B // I2C address of the LSM9DS1 Accelerometer/Gyroscope
#define LSM9DS1_M_ADDR  0x1E // I2C address of the magnetometer

#define WHO_AM_I_REG 0x0F // WHO_AM_I register address
#define CTRL_REG1_G  0x10 // Gyro control register 1
#define CTRL_REG6_XL 0x20 // Accelerometer control register 6

#define OUT_X_L_G  0x18 // Gyro X-axis output low byte
#define OUT_X_L_XL 0x28 // Accelerometer X-axis output low byte
#define OUT_X_L_M  0x28 // Magnetometer X-axis output low byte

// Function to write a byte to a register
int write_register(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    uint8_t buffer[2] = {reg_addr, data};
    return i2c_write(i2c_dev, buffer, sizeof(buffer), dev_addr);
}

// Function to read a byte from a register
int read_register(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data) {
    int ret = i2c_write_read(i2c_dev, dev_addr, &reg_addr, 1, data, 1);
    return ret;
}

// Function to read multiple bytes from sequential registers
int read_registers(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    reg_addr |= 0x80; // Set MSB to enable auto-increment
    int ret = i2c_write_read(i2c_dev, dev_addr, &reg_addr, 1, data, len);
    return ret;
}

// Initialize LSM9DS1 settings
void lsm9ds1_init(const struct device *i2c_dev) {
    uint8_t who_am_i_ag, who_am_i_m;

    // Initialize accelerometer/gyroscope (AG)
    if (read_register(i2c_dev, LSM9DS1_AG_ADDR, WHO_AM_I_REG, &who_am_i_ag) != 0 || who_am_i_ag != 0x68) {
        printk("LSM9DS1 accelerometer/gyroscope not detected\n");
    } else {
        printk("LSM9DS1 accelerometer/gyroscope detected\n");
        // Set up gyroscope control (952 Hz, 2000 dps)
        uint8_t gyro_ctrl_reg1 = (0x6 << 5) | (0x3 << 3); // 952 Hz, 2000 dps
        write_register(i2c_dev, LSM9DS1_AG_ADDR, CTRL_REG1_G, gyro_ctrl_reg1);

        // Set up accelerometer control (952 Hz, 8g)
        uint8_t accel_ctrl_reg6 = (0x6 << 5) | (0x2 << 3); // 952 Hz, ±8 g
        write_register(i2c_dev, LSM9DS1_AG_ADDR, CTRL_REG6_XL, accel_ctrl_reg6);
    }

    // Initialize magnetometer (M)
    if (read_register(i2c_dev, LSM9DS1_M_ADDR, WHO_AM_I_REG, &who_am_i_m) != 0 || who_am_i_m != 0x3D) {
        printk("LSM9DS1 magnetometer not detected\n");
    } else {
        printk("LSM9DS1 magnetometer detected\n");
        // Set up magnetometer control registers
        // Example: write_register(i2c_dev, LSM9DS1_M_ADDR, CTRL_REG1_M, 0x70);
    }
}

// Function to read and print LSM9DS1 data
void read_lsm9ds1_data(const struct device *i2c_dev) {
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    int16_t mag_x, mag_y, mag_z;
    uint8_t data[6];

    // Read accelerometer data
    if (read_registers(i2c_dev, LSM9DS1_AG_ADDR, OUT_X_L_XL, data, 6) == 0) {
        accel_x = (int16_t)((data[1] << 8) | data[0]);
        accel_y = (int16_t)((data[3] << 8) | data[2]);
        accel_z = (int16_t)((data[5] << 8) | data[4]);
        printk("Accelerometer: X=%d, Y=%d, Z=%d\n", accel_x, accel_y, accel_z);
    } else {
        printk("Failed to read accelerometer data\n");
    }

    // Read gyroscope data
    if (read_registers(i2c_dev, LSM9DS1_AG_ADDR, OUT_X_L_G, data, 6) == 0) {
        gyro_x = (int16_t)((data[1] << 8) | data[0]);
        gyro_y = (int16_t)((data[3] << 8) | data[2]);
        gyro_z = (int16_t)((data[5] << 8) | data[4]);
        printk("Gyroscope: X=%d, Y=%d, Z=%d\n", gyro_x, gyro_y, gyro_z);
    } else {
        printk("Failed to read gyroscope data\n");
    }

    // Read magnetometer data
    if (read_registers(i2c_dev, LSM9DS1_M_ADDR, OUT_X_L_M, data, 6) == 0) {
        mag_x = (int16_t)((data[1] << 8) | data[0]);
        mag_y = (int16_t)((data[3] << 8) | data[2]);
        mag_z = (int16_t)((data[5] << 8) | data[4]);
        // printk("Magnetometer: X=%d, Y=%d, Z=%d\n", mag_x, mag_y, mag_z);
    } else {
        printk("Failed to read magnetometer data\n");
    }
}

