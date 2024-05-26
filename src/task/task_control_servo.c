//Free RTOS
#include "FreeRTOS.h"
#include "queue.h"
//Own
#include "const/const.h"
#include "globals/globals.h"

void control_servo(void *parameters){

    //Para enviar mensajes al serial
    char msg_serial[50];

    while (1) {

        sprintf(msg_serial, "A");
        xQueueSend(queue_serial, (void *)&msg_serial, 0);

        //Esperamos un tiempo
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}