#include "FreeRTOS.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "const/const.h"
#include "globals/globals.h"


static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {

    //No se porque pero es necesario despertar al sensor cada vez
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(I2C_PORT, I2C_ADDR, buf, 2, true);

    // Leemos los 6 bytes del acelerometro
    //Esta variable se utiliza de manera temporal para almacenar los bytes devueltos
    uint8_t buffer[6];
    uint8_t val = 0x3B; // registro del acelerometro
    i2c_write_blocking(I2C_PORT, I2C_ADDR, &val, 1, true);
    i2c_read_blocking(I2C_PORT, I2C_ADDR, buffer, 6, false); // false para indica que finalizamos comunicacion
    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Leemos los 6 bytes del giroscopio
    val = 0x43; // registro del giroscopio
    i2c_write_blocking(I2C_PORT, I2C_ADDR, &val, 1, true);
    i2c_read_blocking(I2C_PORT, I2C_ADDR, buffer, 6, false); // false para indica que finalizamos comunicacion
    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Leemos los 2 bytes de la temperatura
    uint8_t tempBuffer[2];
    val = 0x41; // registro de la temperatura
    i2c_write_blocking(I2C_PORT, I2C_ADDR, &val, 1, true);
    i2c_read_blocking(I2C_PORT, I2C_ADDR, tempBuffer, 2, false); // false para indica que finalizamos comunicacion
    *temp = (tempBuffer[0] << 8 | tempBuffer[1]);
}

// Establecimiento y reset de los registros a leer
static void mpu6050_reset() {
    //Arrancamos el sensor
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(I2C_PORT, I2C_ADDR, buf, 2, true);

    //Indicamos los rangos del giroscopio y del acelerometro
    int8_t buf_acc[] = {0x1C, 0x00}; // Set accelerometer range to ±2g (default)
    uint8_t buf_gyro[] = {0x1B, 0x00}; // Set gyroscope range to ±250 degrees/sec (default)
    i2c_write_blocking(I2C_PORT, I2C_ADDR, buf_acc, 2, true);
    i2c_write_blocking(I2C_PORT, I2C_ADDR, buf_gyro, 2, true);
}

void task_get_orientation(void *parameters){
    //Inicializamos i2c a 400KHz
    i2c_init(I2C_PORT, 400 * 1000);

    //Indicamos que usamos los pines como i2c
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SCL);
    gpio_pull_up(PIN_SDA);

    //Hacemos un primer reset al sensor
    mpu6050_reset();

    //Variable que contienen los datos en bruto del sensor
    int16_t acceleration[3];
    int16_t gyroscope[3];
    int16_t temp;
    //Variables que contienen los datos procesados del sensor
    float Ax, Ay, Az;
    float Gx, Gy, Gz;
    float pitch = 0.0, roll = 0.0, yaw = 0.0;
    //Variables para calcular el tiempro entre ciclos
    uint64_t last_time = time_us_64();
    uint64_t current_time;
    float elapsed_time;
    //Para enviar mensajes al serial
    char msg_serial[150];

    while (1) {
        //Leemos valores
        mpu6050_read_raw(acceleration, gyroscope, &temp);

        //Calculamos la diferencia de tiempo para hacer el calculo de la inclinacion mas adelante
        current_time = time_us_64();
        elapsed_time = (current_time - last_time) / 1000000.0f;
        last_time = current_time;

        //Convertimos los datos del acelerometro a Gs (asumiendo ±2g rango)
        Ax = acceleration[0] / 16384.0;
        Ay = acceleration[1] / 16384.0;
        Az = acceleration[2] / 16384.0;
        //Convertimos los datos del giroscopio a grados (asumiendo ±250 deg/sec rango)
        Gx = gyroscope[0] / 131.0;
        Gy = gyroscope[1] / 131.0;
        Gz = gyroscope[2] / 131.0;

        //Calculamos los datos de inclinacion
        float acc_pitch = atan2(Ay, sqrt(Ax * Ax + Az * Az)) * 180 / M_PI;
        float acc_roll = atan2(-Ax, Az) * 180 / M_PI;
        pitch = 0.98 * (pitch + Gy * elapsed_time) + 0.02 * acc_pitch;
        roll = 0.98 * (roll + Gx * elapsed_time) + 0.02 * acc_roll;
        //Para el yaw hay que hacer una acumulacion por lo tanto se genera mucho rudio, con esto se elimina
        if(0.5 < Gz * elapsed_time || -0.5 > Gz * elapsed_time){
            yaw += Gz * elapsed_time;
        }
        
        //printf("Pitch = %f °, Roll = %f °, Yaw = %f °\n", pitch, roll, yaw);

        //Enviamos los datos a a cola del serial
        memset(msg_serial, 0, sizeof(msg_serial));
        sprintf(msg_serial, "Pitch = %f °, Roll = %f °, Yaw = %f °", pitch, roll, yaw);
        msg_serial[sizeof(msg_serial) - 1] = '\0';
        xQueueSend(queue_serial, (void *)&msg_serial, 0);

        //Esperamos un tiempo
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}