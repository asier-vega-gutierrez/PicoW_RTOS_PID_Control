cmake_minimum_required(VERSION 3.12)

SET(PICO_SDK_PATH /home/ubuntu/pico-sdk/)

# Import those libraries
include(pico_sdk_import.cmake)

# Define project
project(mpu6050)
set(CMAKE_C_STANDAR 11)
set(CMAKE_CXX_STANDAR 17)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(mpu6050 mpu6050.c)

target_link_libraries(mpu6050 pico_stdlib hardware_i2c)

pico_enable_stdio_usb(mpu6050 1) #habilitamos usar la comunicacion usb
pico_enable_stdio_uart(mpu6050 0) #deshabilitamos usar la comunicacion uart (0,1)

pico_add_extra_outputs(mpu6050)