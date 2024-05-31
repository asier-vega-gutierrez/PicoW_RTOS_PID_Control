#ifndef CONSTANTS_H
#define CONSTANTS_H

//Pines de i2c y nombre del puerto i2c
const int PIN_SCL = 5;
const int PIN_SDA = 4;
#define I2C_PORT i2c0
//Direccion i2c del sensor (por defecto)
const int I2C_ADDR = 0x68;
//Pin para el PWM del servo
const int PIN_PWM = 6;
//Pin de parada de emergencia (tambien se usa como rearme)
const int PIN_EM = 7;
//Pin de reset del sistema
const int PIN_RESET = 8;

//Tamaños y tipo de colas
const int QUEUE_SERIAL_SIZE = 20;
const char QUEUE_SERIAL_DATA[150];
const int QUEUE_SENSOR_SIZE = 5;
const float QUEUE_SENSOR_DATA;
const int QUEUE_SERVO_SIZE = 1;
const float QUEUE_SERVO_DATA;

//Limites del servo
const int PULSE_WIDTH_MAX = 2500;
const int PULSE_WIDTH_MIN = 300;

//Configuracion del PID
const float KP = 1.5;



#endif