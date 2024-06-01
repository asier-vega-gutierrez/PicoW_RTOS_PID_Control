#include "FreeRTOS.h"
#include "const/const.h"
#include "globals/globals.h"

//Callback de la interrupcion asociada al boton
void emergency_callback(uint gpio, uint32_t events) {
    //Cuando se active esta callback activamos el taskhandle que activa la tarea del ciclo de emergencia
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xTaskHandleEmergency, &xHigherPriorityTaskWoken);
    // Force a context switch if necessary
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

//Tarea para gestionar el ciclo de emergencia
void task_emergency(void *pvParameters) {
    
    //Iniciacion del boton de reset
    gpio_init(PIN_RESET);
    gpio_set_dir(PIN_RESET, GPIO_IN);

    //Para enviar mensajes al serial
    char msg_serial[150];
    //Para gestionar si se ha hecho reset o no
    bool reset = 1;

    while (1) {
        
        //Esperamos a la notificacion del callback del ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        //Pramamos las tareas que afectan a la emergencia
        vTaskSuspend(xTaskHandleControlServo);
        vTaskSuspend(xTaskHandleGetOrientation);
        vTaskSuspend(xTaskHandlePID);

        //Enviamos un mensaje al serial de que se ha producido una emergencia
        memset(msg_serial, 0, sizeof(msg_serial));
        sprintf(msg_serial, "Emergency STOP!!!");
        msg_serial[sizeof(msg_serial) - 1] = '\0';
        xQueueSend(queue_serial, (void *)&msg_serial, 0);
        //Tenemos que meter un delay para que esta tarea no estorbe a la tarea del serial (menor prioridad) y asi esl serial vacie su lista
        vTaskDelay(pdMS_TO_TICKS(100));
        
        //Establecemos que no se ha enviado señal de reset
        reset = 0;

        //Esperamos hasta recivir la señal de reset
        while(reset == 0){
            if(gpio_get(PIN_RESET) == 1){
                //Enviamos un mensaje al serial de que se ha reiniciado el sistema
                memset(msg_serial, 0, sizeof(msg_serial));
                sprintf(msg_serial, "Emergency RELEASE!!!");
                msg_serial[sizeof(msg_serial) - 1] = '\0';
                xQueueSend(queue_serial, (void *)&msg_serial, 0);
                //Reinicimoas las tareas
                vTaskResume(xTaskHandleControlServo);
                vTaskResume(xTaskHandleGetOrientation);
                vTaskResume(xTaskHandlePID);
                //Establecmos que se ha recibido señal de reset
                reset = 1;
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        //Añadimos un tiempo de espera
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}