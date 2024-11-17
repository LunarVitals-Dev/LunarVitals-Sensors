#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <stdint.h>

#define I2C_NODE DT_NODELABEL(i2c0)
#define BMP280_ADDR 0x76  // Change to 0x77 if SDO pin is high

// BMP280 Register Addresses
#define BMP280_REG_CALIB_START    0x88
#define BMP280_REG_CHIPID         0xD0
#define BMP280_REG_SOFTRESET      0xE0
#define BMP280_REG_STATUS         0xF3
#define BMP280_REG_CONTROL        0xF4
#define BMP280_REG_CONFIG         0xF5
#define BMP280_REG_PRESSURE_MSB   0xF7
#define BMP280_REG_TEMPERATURE_MSB 0xFA

// Calibration parameters
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
int32_t t_fine;

// Function to write a byte to a register
int write_register(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t data) {
    uint8_t buffer[2] = {reg_addr, data};
    return i2c_write(i2c_dev, buffer, sizeof(buffer), dev_addr);
}

// Function to read a byte from a register
int read_register(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data) {
    return i2c_write_read(i2c_dev, dev_addr, &reg_addr, 1, data, 1);
}

// Function to read multiple bytes from sequential registers
int read_registers(const struct device *i2c_dev, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    return i2c_write_read(i2c_dev, dev_addr, &reg_addr, 1, data, len);
}

// Initialize BMP280 settings
void bmp280_init(const struct device *i2c_dev) {
    uint8_t chip_id;

    // Read chip ID
    if (read_register(i2c_dev, BMP280_ADDR, BMP280_REG_CHIPID, &chip_id) != 0 || chip_id != 0x58) {
        printk("BMP280 sensor not detected\n");
        return;
    } else {
        printk("BMP280 sensor detected, Chip ID: 0x%x\n", chip_id);

        // Read calibration data
        uint8_t calib_data[24];
        if (read_registers(i2c_dev, BMP280_ADDR, BMP280_REG_CALIB_START, calib_data, 24) != 0) {
            printk("Failed to read calibration data\n");
            return;
        }

        // Parse calibration data
        dig_T1 = (uint16_t)(calib_data[1] << 8 | calib_data[0]);
        dig_T2 = (int16_t)(calib_data[3] << 8 | calib_data[2]);
        dig_T3 = (int16_t)(calib_data[5] << 8 | calib_data[4]);
        dig_P1 = (uint16_t)(calib_data[7] << 8 | calib_data[6]);
        dig_P2 = (int16_t)(calib_data[9] << 8 | calib_data[8]);
        dig_P3 = (int16_t)(calib_data[11] << 8 | calib_data[10]);
        dig_P4 = (int16_t)(calib_data[13] << 8 | calib_data[12]);
        dig_P5 = (int16_t)(calib_data[15] << 8 | calib_data[14]);
        dig_P6 = (int16_t)(calib_data[17] << 8 | calib_data[16]);
        dig_P7 = (int16_t)(calib_data[19] << 8 | calib_data[18]);
        dig_P8 = (int16_t)(calib_data[21] << 8 | calib_data[20]);
        dig_P9 = (int16_t)(calib_data[23] << 8 | calib_data[22]);

        // Configure the sensor: Normal mode, temp and pressure oversampling x1
        uint8_t ctrl_meas = (0x01 << 5) | (0x01 << 2) | 0x03;  // osrs_t, osrs_p, mode
        write_register(i2c_dev, BMP280_ADDR, BMP280_REG_CONTROL, ctrl_meas);

        // Configuration register (optional settings)
        uint8_t config = (0x00 << 5) | (0x00 << 2) | 0x00;  // t_sb, filter, spi3w_en
        write_register(i2c_dev, BMP280_ADDR, BMP280_REG_CONFIG, config);
    }
}

// Function to read and print BMP280 data
void read_bmp280_data(const struct device *i2c_dev) {
    uint8_t data[6];

    // Read raw pressure and temperature data
    if (read_registers(i2c_dev, BMP280_ADDR, BMP280_REG_PRESSURE_MSB, data, 6) != 0) {
        printk("Failed to read BMP280 data\n");
        return;
    }

    // Combine bytes to get raw readings
    int32_t adc_P = ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4) | (data[2] >> 4);
    int32_t adc_T = ((int32_t)data[3] << 12) | ((int32_t)data[4] << 4) | (data[5] >> 4);

    // Temperature compensation
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
                      ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
                    ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    int32_t T = (t_fine * 5 + 128) >> 8;

    // Pressure compensation
    int64_t var1_p = ((int64_t)t_fine) - 128000;
    int64_t var2_p = var1_p * var1_p * (int64_t)dig_P6;
    var2_p = var2_p + ((var1_p * (int64_t)dig_P5) << 17);
    var2_p = var2_p + (((int64_t)dig_P4) << 35);
    var1_p = ((var1_p * var1_p * (int64_t)dig_P3) >> 8) + ((var1_p * (int64_t)dig_P2) << 12);
    var1_p = (((((int64_t)1) << 47) + var1_p)) * ((int64_t)dig_P1) >> 33;

    if (var1_p == 0) {
        printk("Avoiding division by zero\n");
        return;  // Avoid division by zero
    }

    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - var2_p) * 3125) / var1_p;
    var1_p = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2_p = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1_p + var2_p) >> 8) + (((int64_t)dig_P7) << 4);

    // Convert to human-readable units
    uint32_t pressure = (uint32_t)(p >> 8);  // Pressure in Pa
    float temperature = T / 100.0f;          // Temperature in °C
    float pressure_hPa = pressure / 100.0f;  // Pressure in hPa

    // Print results
    printk("Temperature: %d.%02d °C, Pressure: %d.%02d hPa\n",
           (int)temperature, (int)(temperature * 100) % 100,
           (int)pressure_hPa, (int)(pressure_hPa * 100) % 100);
}

void main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);

    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not found\n");
        return;
    }
    else {
        printk("found");
    }

    bmp280_init(i2c_dev);

    while (1) {
        read_bmp280_data(i2c_dev);
        k_sleep(K_SECONDS(1));
    }
}
