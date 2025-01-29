cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(victor_pulse_HR)

target_sources(app PRIVATE src/main.c)
