cmake_minimum_required(VERSION 3.12)

#Nombre del proyecto
SET(ProjectName pico-rtos)

#Seteamos la variable al skd del pico y al kernel de free_rtos
SET(PICO_SDK_PATH /home/ubuntu/pico-sdk/)
SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/free_rtos)


#Importamos la librerias
include(pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

#Definimos el proyecto
project(${ProjectName})

#Inicializamos el SDK
pico_sdk_init()

#Anadimos el directorio
add_subdirectory(src)

# if you have anything in "lib" folder then uncomment below - remember to add a CMakeLists.txt
# file to the "lib" directory
#add_subdirectory(lib)