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
)

pico_add_extra_outputs(${ProjectName})