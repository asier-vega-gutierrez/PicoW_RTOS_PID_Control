//Free RTOS
#include "FreeRTOS.h"
//C std
#include <time.h>
#include "queue.h"
//Own
#include "const/const.h"
#include "globals/globals.h"

float calculate_control_value(float setpoint, float processVariable) {
    //Ajustamos el valor del preoceso a rangos de 180 grados (el servo solo permite controlar hasta 180)
    if (processVariable > 180){
        processVariable = 180.0;
    }else if (processVariable < 0.0){
        processVariable = 0.0;
    }
    //Calculamos le error
    float error = setpoint - processVariable;
    //Calculamos la accion proporcional
    float controlOutput = KP * error;
    //Ajustamos la accion de control para que como maximo te mande a 180 grados o a -180 grados (te has pasado)
    if (controlOutput > 180){
        controlOutput = 180.0;
    }else if (controlOutput < -180.0){
        controlOutput = -180.0;
    }
    return controlOutput;
}

//Funcion para botener el current value
float read_current_value(){
    float sensor_data[QUEUE_SENSOR_SIZE];
    float sum = 0.0;
    float avg = 0.0;
    int num_elements = sizeof(sensor_data) / sizeof(sensor_data[0]);
    //Nada mas se inicia la tarea se deven obtener los ultimos 5 valores
    for (int i = 0; i < num_elements; i++) {
        //para ello se vacia la cola completamente (esta esta actualizandose constantemente simpre que se llena en su orgen)
        xQueueReceive(queue_sensor, (void *)&sensor_data[i], 0);
        //Vamos sumando todos los valores
        sum += sensor_data[i];
    }
    //Devolvemos el recurso compartido
    xSemaphoreGive(mutex_queue_sensor_recieve);
    //Calculamos la media
    avg = sum / num_elements;
    return avg;
}

//Para 
void pid(void *parameters){

    float current_value = 0.0;
    float setpoint = 100.0;
    float control_value = 0.0;
    float servo_goal = 0.0;

    //Para enviar mensajes al serial
    char msg_serial[150];

    while (1) {

        //Se va a accader al metodo recieve de una lista que esta compartido con otra tarea lo que usamo un mutex
        if(xSemaphoreTake(mutex_queue_sensor_recieve, 0) == pdTRUE){
            //Establecemos el valor actual
            current_value = read_current_value();
        }

        control_value = calculate_control_value(setpoint,current_value);

        //Enviamos los valores al serial si no se llega a cojer el mutex se envia el valor anterior
        memset(msg_serial, 0, sizeof(msg_serial));
        sprintf(msg_serial, "Current Value = %f", current_value);
        msg_serial[sizeof(msg_serial) - 1] = '\0';
        xQueueSend(queue_serial, (void *)&msg_serial, 0);
        memset(msg_serial, 0, sizeof(msg_serial));
        sprintf(msg_serial, "Set Point = %f", setpoint);
        msg_serial[sizeof(msg_serial) - 1] = '\0';
        xQueueSend(queue_serial, (void *)&msg_serial, 0);
        memset(msg_serial, 0, sizeof(msg_serial));
        sprintf(msg_serial, "Control Value = %f", control_value);
        msg_serial[sizeof(msg_serial) - 1] = '\0';
        xQueueSend(queue_serial, (void *)&msg_serial, 0);

        //Enviamos el valor calculado al servo
        servo_goal = current_value + control_value;
        //Como no queremos que se consuma nuca el valor, esto es que no se reciva nunca para evitar leer 0, sobrescribimos el unico valor de la lista
        xQueueOverwrite(queue_servo, (void *)&servo_goal);
        
        //Esperamos un tiempo
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}