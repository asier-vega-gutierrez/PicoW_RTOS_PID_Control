//Free RTOS
#include "FreeRTOS.h"
//C std
#include <time.h>
#include "queue.h"
//Own
#include "const/const.h"
#include "globals/globals.h"

//Para 
void pid(void *parameters){

    //Esperamos a que la lista de 10 lecturas del sensor se llene completamente
    vTaskDelay(500 / portTICK_PERIOD_MS);

    //Para enviar mensajes al serial
    char msg_serial[150];

    float sensor_data[QUEUE_SENSOR_SIZE];
    float sum = 0.0;
    float avg = 0.0;
    int num_elements = sizeof(sensor_data) / sizeof(sensor_data[0]);

    while (1) {
        
        //Nada mas se inicia la tarea se deven obtener los ultimos 5 valores
        for (int i = 0; i < num_elements; i++) {
            //para ello se vacia la cola completamente (esta esta actualizandose constantemente simpre que se llena en su orgen)
            xQueueReceive(queue_sensor, (void *)&sensor_data[i], 0);
            //Vamos sumando todos los valores
            sum += sensor_data[i];
        }
        //Calculamos la media
        avg = sum / num_elements;
        
        //Enviamos la media al serial
        memset(msg_serial, 0, sizeof(msg_serial));
        sprintf(msg_serial, "Current Value = %f", avg);
        msg_serial[sizeof(msg_serial) - 1] = '\0';
        xQueueSend(queue_serial, (void *)&msg_serial, 0);

        //Reseteamos la acumualcion de valores
        sum = 0.0;

        //Esperamos un tiempo
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}