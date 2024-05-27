cmake_minimum_required(VERSION 3.12)

SET(PICO_SDK_PATH /home/ubuntu/pico-sdk/)

# Import those libraries
include(pico_sdk_import.cmake)

# Define project
project(sg90)
set(CMAKE_C_STANDAR 11)
set(CMAKE_CXX_STANDAR 17)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(sg90 sg90.c)

target_link_libraries(sg90 pico_stdlib)

pico_enable_stdio_usb(sg90 1) #habilitamos usar la comunicacion usb
pico_enable_stdio_uart(sg90 0) #deshabilitamos usar la comunicacion uart (0,1)

pico_add_extra_outputs(sg90)