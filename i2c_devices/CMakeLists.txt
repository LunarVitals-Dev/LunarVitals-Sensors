cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(i2c_devices)

target_sources(app PRIVATE src/main.c src/heart_rate.c src/i2c.c src/MAX30102.c src/BMP280.c src/MLX90614.c src/MPU6050.c)