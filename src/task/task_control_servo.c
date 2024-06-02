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
    for (int i = 1; i < 50; ++i) {
        gpio_put(PIN_PWM, 1);
        sleep_us(pulse_widht);
        gpio_put(PIN_PWM, 0);
        sleep_us(20000 - pulse_widht);
        gpio_put(PIN_PWM, 0);
    }
}

void control_servo(void *parameters){

    //Inicializamos el pin a PWM
    gpio_init(PIN_PWM);
    gpio_set_dir(PIN_PWM, GPIO_OUT);

    //Variables para controlar la posicion del servo
    float servo_goal = 0;
    float servo_current = 0; //este servo current es para desactivar el servo si ya esta en su posicion

    //Para enviar mensajes al serial
    char msg_serial[50];

    //Colocamos el servo en la posicion inicial
    servo_goal = 10;
    move_servo(servo_goal);
    //vTaskDelay(10000 / portTICK_PERIOD_MS);

    while (1) {

        servo_goal = 100;
        move_servo(servo_goal);

        /*//Leemos el valor pero sin eliminarlo de la lista
        xQueuePeek(queue_servo, (void *)&servo_goal, 0);



        //Tenemos que comprovar con un margen si la posicion objetivo a cambiado
        if (!(servo_current + 5.0 > 100 && servo_current - 5.0 < 100)){
            
            servo_goal = 100;
            //Si ha cambiado entonces le decimos al servo que valla a la nueva posicion objetivo
            move_servo(servo_goal);
            
            //Actualizamos la posicion actual
            servo_current = servo_goal;

            //Pintamos por el serial el cambio de posicion del servo
            memset(msg_serial, 0, sizeof(msg_serial));
            sprintf(msg_serial, "--------Servo Goal = %f", servo_goal);
            msg_serial[sizeof(msg_serial) - 1] = '\0';
            xQueueSend(queue_serial, (void *)&msg_serial, 0);

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        //Esperamos un tiempo
        vTaskDelay(10000 / portTICK_PERIOD_MS);*/
    }
}