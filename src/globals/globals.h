
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


//Mutex 
SemaphoreHandle_t mutex_queue_sensor_recieve;

#endif
