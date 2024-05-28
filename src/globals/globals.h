
#include "queue.h"

#ifndef GLOBALS_H
#define GLOBALS_H

//Cola para usar el serial
QueueHandle_t queue_serial;
//Cola para guardar los datos del sensor
QueueHandle_t queue_sensor;

#endif
