//Free RTOS
#include "FreeRTOS.h"
#include "queue.h"
//Own
#include "const/const.h"
#include "globals/globals.h"

//Para pintar por el serial de forma comu
void task_serial(void *parameters){
    char msg_serial[150];

    while (1) {
        //Recibimos mensajes de otras tarea para pintar en el serial
        if (xQueueReceive(queue_serial, (void *)&msg_serial, 0) == pdTRUE) {
            printf("%s \n",msg_serial);
        }
        //Esperamos un tiempo
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}