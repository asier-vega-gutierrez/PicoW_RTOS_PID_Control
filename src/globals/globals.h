
#include "queue.h"
#include "semphr.h"

#ifndef GLOBALS_H
#define GLOBALS_H

//Cola para usar el serial
QueueHandle_t queue_serial;
//Cola para guardar los datos del sensor
QueueHandle_t queue_sensor;
//Cola para pasar el valor al servo
QueueHandle_t queue_servo;

//Mutex para controlar la recepcion multiple de los valores de la cola del sensor
SemaphoreHandle_t mutex_queue_sensor_recieve;

//Handlers para activar o desactivar tareas
TaskHandle_t xTaskHandleControlServo = NULL;
TaskHandle_t xTaskHandleGetOrientation = NULL;
TaskHandle_t xTaskHandlePID = NULL;
TaskHandle_t xTaskHandleSerial = NULL;
TaskHandle_t xTaskHandleEmergency = NULL;

//

#endif
