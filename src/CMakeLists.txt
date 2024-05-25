#Añadimos el main como ejecutable
add_executable(${ProjectName}
    main.c
)

#Añadimos los directorios
target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

#Anadimos la librerias
target_link_libraries(${ProjectName} 
    pico_stdlib 
    FreeRTOS-Kernel-Heap4
    hardware_i2c
)

pico_enable_stdio_usb(${ProjectName} 1) #habilitamos usar la comunicacion usb
pico_enable_stdio_uart(${ProjectName} 0) #deshabilitamos usar la comunicacion uart (0,1)
pico_add_extra_outputs(${ProjectName})