cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(MPU6050)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/i2c.c)
target_sources(app PRIVATE src/BMP280.c)
target_sources(app PRIVATE src/MLX90614.c)
target_sources(app PRIVATE src/MPU6050.c)