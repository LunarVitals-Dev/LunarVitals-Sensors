# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(pulse_oximeter)

target_sources(app PRIVATE src/main.c src/heart_rate.c src/i2c_device.c src/MAX30102.c src/spo2_algorithm.c)
