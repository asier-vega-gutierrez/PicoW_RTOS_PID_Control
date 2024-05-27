//Free RTOS
#include "FreeRTOS.h"
//C std
#include <time.h>
//Own
#include "const/const.h"
#include "globals/globals.h"

// PID structure definition
typedef struct {
    double Kp;  // Proportional gain
    double Ki;  // Integral gain
    double Kd;  // Derivative gain
    double previous_error;  // Previous error
    double integral;  // Integral of the error
    double setpoint;  // Desired setpoint
} PID;

// Function to initialize the PID structure
void PID_Init(PID *pid, double Kp, double Ki, double Kd, double setpoint) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->previous_error = 0;
    pid->integral = 0;
    pid->setpoint = setpoint;
}

// Function to compute the PID output
double PID_Compute(PID *pid, double current_value) {
    double error = pid->setpoint - current_value;
    pid->integral += error;
    double derivative = error - pid->previous_error;
    double output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;
    pid->previous_error = error;
    return output;
}

//Para 
void pid(void *parameters){
    PID pid;
    double setpoint = 100;  // Desired value
    double current_value = 0;  // Current value
    double control_signal;
    double Kp = 1.0, Ki = 0.1, Kd = 0.0;
    double time_step = 1.0;  // Time step in seconds
    time_t start, end;
    double elapsed_time;

    PID_Init(&pid, Kp, Ki, Kd, setpoint);

    while (1) {

        

        /*time(&start);
        // Compute the control signal
        control_signal = PID_Compute(&pid, current_value);
        // Update the process variable (this is just an example, replace with your actual process model)
        current_value += control_signal * time_step;
        printf("Setpoint: %.2f, Current Value: %.2f, Control Signal: %.2f\n", setpoint, current_value, control_signal);
        // Wait for the next time step
        do {
            time(&end);
            elapsed_time = difftime(end, start);
        } while (elapsed_time < time_step);*/

    }
}