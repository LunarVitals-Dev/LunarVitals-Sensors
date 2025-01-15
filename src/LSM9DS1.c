#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

// HOW TO CONNECT: (LSM9DS1 --> Nordic)
// VIN --> VDD
// GND --> GND
// SCL --> Pin 27
// SDA --> Pin 26
// SDOAG --> unconnected
// SDOM --> unconnected
// CS_G --> VDD
// CS_M --> VDD



#define I2C_NODE DT_NODELABEL(i2c0)
#define LSM9DS1_AG_ADDR 0x6B // I2C address of the LSM9DS1 Accelerometer/Gyroscope (becomes 0x6A if connected to VDD)
#define LSM9DS1_M_ADDR  0x1E // I2C address of the magnetometer (becomes 0x1C if connected to VDD)

#define WHO_AM_I_REG 0x0F // WHO_AM_I register address
#define CTRL_REG1_G  0x10 // Gyro control register 1
#define CTRL_REG6_XL 0x20 // Accelerometer control register 6

#define OUT_X_L_G  0x18 // Gyro X-axis output low byte
#define OUT_X_L_XL 0x28 // Accelerometer X-axis output low byte
#define OUT_X_L_M  0x28 // Magnetometer X-axis output low byte

#define CTRL_REG1_M 0x20
#define CTRL_REG2_M 0x21
#define CTRL_REG3_M 0x22
#define CTRL_REG4_M 0x23
#define CTRL_REG5_M 0x24

// Sensitivity (from LSM9DS1 datasheet):
// Assuming configured scales: ±8g for accelerometer, ±2000 dps for gyroscope, ±4 gauss for magnetometer.

// Accelerometer ±8g: 0.244 mg/LSB = 0.000244 g/LSB
#define ACCEL_SENSITIVITY_G 0.000244f 

// Gyroscope ±2000 dps: 70 mdps/LSB = 0.07 dps/LSB
#define GYRO_SENSITIVITY_DPS 0.07f

// Magnetometer ±4 gauss: ~0.14 mGauss/LSB = 0.00014 Gauss/LSB
#define MAG_SENSITIVITY_GAUSS 0.00014f


// Function to write a byte to a register
int write_register_LSM(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    uint8_t buffer[2] = {reg_addr, data};
    return i2c_write(i2c_dev, buffer, sizeof(buffer), dev_addr);
}

// Function to read a byte from a register
int read_register_LSM(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data) {
    return i2c_write_read(i2c_dev, dev_addr, &reg_addr, 1, data, 1);
}

// Function to read multiple bytes from sequential registers
int read_registers_LSM(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    reg_addr |= 0x80; // Set MSB to enable auto-increment
    return i2c_write_read(i2c_dev, dev_addr, &reg_addr, 1, data, len);
}

// Initialize LSM9DS1 settings
void lsm9ds1_init(const struct device *i2c_dev) {
    uint8_t who_am_i_ag, who_am_i_m;

    // Initialize accelerometer/gyroscope (AG)
    if (read_register_LSM(i2c_dev, LSM9DS1_AG_ADDR, WHO_AM_I_REG, &who_am_i_ag) != 0 || who_am_i_ag != 0x68) {
        printk("LSM9DS1 accelerometer/gyroscope not detected\n");
    } else {
        printk("LSM9DS1 accelerometer/gyroscope detected\n");
        // Set up gyroscope: ODR=952 Hz (110), FS=±2000 dps (11)
        // CTRL_REG1_G bits: ODR_G[7:5]=110, FS_G[4:3]=11, rest=0
        // (0x6 << 5) = 0xC0, (0x3 << 3)=0x18 => 0xC0|0x18=0xD8
        write_register_LSM(i2c_dev, LSM9DS1_AG_ADDR, CTRL_REG1_G, 0xD8);

        // Set up accelerometer: ODR=952 Hz (110), FS=±8g (10)
        // CTRL_REG6_XL bits: ODR_XL[7:5]=110, FS_XL[4:3]=10 => 0xC0|0x10=0xD0
        write_register_LSM(i2c_dev, LSM9DS1_AG_ADDR, CTRL_REG6_XL, 0xD0);
    }

    // Initialize magnetometer (M)
    if (read_register_LSM(i2c_dev, LSM9DS1_M_ADDR, WHO_AM_I_REG, &who_am_i_m) != 0 || who_am_i_m != 0x3D) {
        printk("LSM9DS1 magnetometer not detected\n");
    } else {
        printk("LSM9DS1 magnetometer detected\n");
        // Example configuration for magnetometer:
        // CTRL_REG1_M (0x70): OM=11 (high-perf XY), DO=100 (10Hz), TEMP_COMP=0
        write_register_LSM(i2c_dev, LSM9DS1_M_ADDR, CTRL_REG1_M, 0x70);
        // CTRL_REG2_M: FS=00 (±4 gauss)
        write_register_LSM(i2c_dev, LSM9DS1_M_ADDR, CTRL_REG2_M, 0x00);
        // CTRL_REG3_M: Continuous-conversion mode (0x00)
        write_register_LSM(i2c_dev, LSM9DS1_M_ADDR, CTRL_REG3_M, 0x00);
        // CTRL_REG4_M: OMZ=11 high-performance on Z
        write_register_LSM(i2c_dev, LSM9DS1_M_ADDR, CTRL_REG4_M, 0x0C);
        // CTRL_REG5_M: Default (0x00) no soft iron correction, no BDU
        write_register_LSM(i2c_dev, LSM9DS1_M_ADDR, CTRL_REG5_M, 0x00);
    }

    // Allow some time after configuration
    k_msleep(100);
}

// Function to read and print LSM9DS1 data
void read_lsm9ds1_data(const struct device *i2c_dev) {
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_x, gyro_y, gyro_z;
    int16_t mag_x, mag_y, mag_z;
    uint8_t data[6];

    // Read accelerometer data
    if (read_registers_LSM(i2c_dev, LSM9DS1_AG_ADDR, OUT_X_L_XL, data, 6) == 0) {
        accel_x = (int16_t)((data[1] << 8) | data[0]);
        accel_y = (int16_t)((data[3] << 8) | data[2]);
        accel_z = (int16_t)((data[5] << 8) | data[4]);

        float accel_x_g = accel_x * ACCEL_SENSITIVITY_G;
        float accel_y_g = accel_y * ACCEL_SENSITIVITY_G;
        float accel_z_g = accel_z * ACCEL_SENSITIVITY_G;

        printk("Accelerometer (raw): X=%d, Y=%d, Z=%d\n", accel_x, accel_y, accel_z);
        printk("Accelerometer (g): X=%.3f, Y=%.3f, Z=%.3f\n", accel_x_g, accel_y_g, accel_z_g);
    } else {
        printk("Failed to read accelerometer data\n");
    }

    // Read gyroscope data
    if (read_registers_LSM(i2c_dev, LSM9DS1_AG_ADDR, OUT_X_L_G, data, 6) == 0) {
        gyro_x = (int16_t)((data[1] << 8) | data[0]);
        gyro_y = (int16_t)((data[3] << 8) | data[2]);
        gyro_z = (int16_t)((data[5] << 8) | data[4]);

        float gyro_x_dps = gyro_x * GYRO_SENSITIVITY_DPS;
        float gyro_y_dps = gyro_y * GYRO_SENSITIVITY_DPS;
        float gyro_z_dps = gyro_z * GYRO_SENSITIVITY_DPS;

        printk("Gyroscope (raw): X=%d, Y=%d, Z=%d\n", gyro_x, gyro_y, gyro_z);
        printk("Gyroscope (dps): X=%.2f, Y=%.2f, Z=%.2f\n", gyro_x_dps, gyro_y_dps, gyro_z_dps);
    } else {
        printk("Failed to read gyroscope data\n");
    }

    // Read magnetometer data
    if (read_registers_LSM(i2c_dev, LSM9DS1_M_ADDR, OUT_X_L_M, data, 6) == 0) {
        mag_x = (int16_t)((data[1] << 8) | data[0]);
        mag_y = (int16_t)((data[3] << 8) | data[2]);
        mag_z = (int16_t)((data[5] << 8) | data[4]);

        float mag_x_gauss = mag_x * MAG_SENSITIVITY_GAUSS;
        float mag_y_gauss = mag_y * MAG_SENSITIVITY_GAUSS;
        float mag_z_gauss = mag_z * MAG_SENSITIVITY_GAUSS;

        printk("Magnetometer (raw): X=%d, Y=%d, Z=%d\n", mag_x, mag_y, mag_z);
        printk("Magnetometer (G): X=%.4f, Y=%.4f, Z=%.4f\n", mag_x_gauss, mag_y_gauss, mag_z_gauss);
    } else {
        printk("Failed to read magnetometer data\n");
    }
}

