#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"

#include "task/task_get_orientation.c"


//Tarea para leer el sensor mpu6050
void task_get_orientation(void *parameters);

void control_servo(void *parameters){

}

void serial(void *parameters){

}

int main(){

    //Iniciar stdio para usar la terminal
    stdio_init_all();

    //Tarea de recojida de datos 
    xTaskCreate(task_get_orientation, "task_get_orientation", 256, NULL, 1, NULL);

    vTaskStartScheduler();
    while(1){};
}