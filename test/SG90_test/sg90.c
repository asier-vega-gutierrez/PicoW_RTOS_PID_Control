#include "pico/stdlib.h"
#include <stdio.h>

#define PIN_PWM 6

const int PULSE_WIDTH_MAX = 2500;
const int PULSE_WIDTH_MIN = 300;

void move_servo(int servo_goal){
    //Calculamos el timepo que se deve mover
    int pulse_widht = (servo_goal * ((PULSE_WIDTH_MAX)/180)) + PULSE_WIDTH_MIN;
    //Recalculamos los maximos y minimos 
    if (pulse_widht > PULSE_WIDTH_MAX){
        pulse_widht = PULSE_WIDTH_MAX;
    }
    if (pulse_widht < PULSE_WIDTH_MIN){
        pulse_widht = PULSE_WIDTH_MIN;
    }
    //Enviamos 10 señales al servo de la posicion que queremos
    for (int i = 1; i <= 10; ++i) {
        gpio_put(PIN_PWM, 1);
        sleep_us(pulse_widht);
        gpio_put(PIN_PWM, 0);
        sleep_us(20000 - pulse_widht);
    }
}

int main() {
    //Iniciar stdio para usar la terminal
    stdio_init_all();

    //Inicializamos el pin a PWM
    gpio_init(PIN_PWM);
    gpio_set_dir(PIN_PWM, GPIO_OUT);

    int servo_goal = 70;
    int servo_current = 0;

    while(1){
        if (servo_current != servo_goal){
            move_servo(servo_goal);
        }
        servo_current = servo_goal;
        sleep_ms(1000);
    }

    return 0;
}
