## Control PID de servo atraves de sensor MPU6050
<ul>
    <li>Fechas: Junio 2024</li>
    <li>Descripción: 
   Se parte del sistema operativo en tiempo real FreeRTOS y una Raspberry Pi Pico W. Con el que se realiza el control de un servo a través de un PID. Se recoge la posición del servo a través de un sensor MPU6050 (cálculo del “yaw”) por medio de una tarea. Esta información es recibida por la tarea del PID que realiza sus cálculos con respecto al “setpoint” previamente fijado. La acción de control se envía a la tarea servo para realizar el ajuste. Por otro lado, se pinta información de estas tareas en por el serial y se puede realizar una parada de emergencia y rearme del sistema, a través de dos botones.
        
![foto](https://github.com/asier-vega-gutierrez/PicoW_RTOS_PID_Control/blob/main/doc/system.png)

</li>
    <li>Resultados:</li>
</ul>
