# Install script for directory: /opt/nordic/ncs/v2.8.0/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/opt/nordic/ncs/toolchains/15b490767d")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "MinSizeRel")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/opt/nordic/ncs/toolchains/15b490767d/opt/zephyr-sdk/arm-zephyr-eabi/bin/arm-zephyr-eabi-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/arch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/soc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/boards/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/subsys/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/drivers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/nrf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/trusted-firmware-m/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/cjson/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/azure-sdk-for-c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/cirrus-logic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/suit-processor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/memfault-firmware-sdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/canopennode/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/chre/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/lz4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/nanopb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/zscilib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/cmsis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/cmsis-dsp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/cmsis-nn/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/hal_nordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/hal_st/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/hal_wurthelektronik/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/hostap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/libmetal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/liblc3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/littlefs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/loramac-node/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/lvgl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/mipi-sys-t/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/nrf_hw_models/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/open-amp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/picolibc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/segger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/tinycrypt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/uoscore-uedhoc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/zcbor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/nrfxlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/modules/connectedhomeip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/cmake/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/cmake/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/owner/victor_chest_monitor/build/victor_chest_monitor/zephyr/cmake/reports/cmake_install.cmake")
endif()

