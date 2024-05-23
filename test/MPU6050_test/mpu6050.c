#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <string.h>

#define PIN_SCL 5
#define PIN_SDA 4
#define I2C_PORT i2c0

static int addr = 0x68;

static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    uint8_t buffer[6];

    // Leemos los 6 bytes del acelerometro
    uint8_t val = 0x3B; // registro del acelerometro
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buffer, 6, false); // false para indica que finalizamos comunicacion

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Leemos los 6 bytes del giroscopio
    val = 0x43; // registro del giroscopio
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buffer, 6, false); // false para indica que finalizamos comunicacion

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Leemos los 2 bytes de la temperatura
    uint8_t tempBuffer[2];
    val = 0x41; // registro de la temperatura
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, tempBuffer, 2, false); // false para indica que finalizamos comunicacion

    *temp = (tempBuffer[0] << 8 | tempBuffer[1]);
}

// Establecimiento y reset de los registros a leer
static void mpu6050_reset() {
    // Wake up the MPU6050 since it starts in sleep mode
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(I2C_PORT, addr, buf, 2, true);

    // Set accelerometer and gyroscope full-scale range
    uint8_t buf_acc[] = {0x1C, 0x00}; // Set accelerometer range to ±2g (default)
    uint8_t buf_gyro[] = {0x1B, 0x00}; // Set gyroscope range to ±250 degrees/sec (default)
    i2c_write_blocking(I2C_PORT, addr, buf_acc, 2, true);
    i2c_write_blocking(I2C_PORT, addr, buf_gyro, 2, true);
}

int main() {
    // Iniciar stdio para usar la terminal
    stdio_init_all();

    // Inicializamos i2c a 400KHz
    i2c_init(I2C_PORT, 400 * 1000);
    // Indicamos que usamos los pines como i2c
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SCL);
    gpio_pull_up(PIN_SDA);

    // Hacemos un primer reset al sensor
    mpu6050_reset();

    // Iniciamos las variables
    int16_t acceleration[3];
    int16_t gyroscope[3];
    int16_t temp;

    while (1) {
        // Leemos valores
        mpu6050_read_raw(acceleration, gyroscope, &temp);
        // Pintamos los resultados
        printf("Ax = %d, Ay = %d, Az = %d \n", acceleration[0], acceleration[1], acceleration[2]);
        printf("Gx = %d, Gy = %d, Gz = %d \n", gyroscope[0], gyroscope[1], gyroscope[2]);
        printf("T = %f \n", (temp / 340.0) + 36.53);
        sleep_ms(1000);
    }
    return 0;
}
