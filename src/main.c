//Free RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//Pico
#include "pico/stdlib.h"
//C std
#include <stdio.h>
//Own
#include "const/const.h"
#include "globals/globals.h"
#include "task/task_get_orientation.c"
#include "task/task_serial.c"
#include "task/task_control_servo.c"
#include "task/task_PID.c"


//Tarea para leer el sensor mpu6050
void task_get_orientation(void *parameters);
//Tarea para usar el serial
void task_serial(void *parameters);
//Tarea para usar el servo
void control_servo(void *parameters);
//Tarea para calcular el movimiento con el PID
void pid(void *parameters);

int main(){

    //Iniciar stdio para usar la terminal
    stdio_init_all();

    //Iniciacion de colas
    queue_serial = xQueueCreate(queue_serial_size, sizeof(queue_serial_data));

    //Iniciacion de la tarea del mpu6050
    xTaskCreate(task_get_orientation, "task_get_orientation", 256, NULL, 4, NULL);
    //Iniciacion de la tarea para usar el serial
    xTaskCreate(task_serial, "task_serial", 256, NULL, 1, NULL);
    //Iniciacion de la tarea del servo
    xTaskCreate(control_servo, "control_servo", 256, NULL, 3, NULL);
    //Iniciacion de la tarea del PID
    xTaskCreate(pid, "PID", 256, NULL, 2, NULL);

    vTaskStartScheduler();
    while(1){};
}