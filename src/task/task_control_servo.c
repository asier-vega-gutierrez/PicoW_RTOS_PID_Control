//Free RTOS
#include "FreeRTOS.h"
#include "queue.h"
//Own
#include "const/const.h"
#include "globals/globals.h"

//Para mover el servo
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

void control_servo(void *parameters){

    //Inicializamos el pin a PWM
    gpio_init(PIN_PWM);
    gpio_set_dir(PIN_PWM, GPIO_OUT);

    //Varibles para controlar la posicion del servo
    float servo_goal = 70;
    float servo_current = 0;

    //Para enviar mensajes al serial
    char msg_serial[50];

    while (1) {
        //Tenemos que comprovar con un margen si la posicion objetivo a cambiado
        if (!(servo_current + 0.5 > servo_goal && servo_current - 0.5 < servo_goal)){
            //Si ha cambiado entonces le decimos al servo que valla a la nueva posicion objetivo
            move_servo(servo_goal);
            //Pintamos por el serial el cambio de posicon del servo
            sprintf(msg_serial, "Servo = %f", servo_goal);
            xQueueSend(queue_serial, (void *)&msg_serial, 0);
            //Actualizamos la posicion actual
            servo_current = servo_goal;
        }
        //Esperamos un tiempo
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}