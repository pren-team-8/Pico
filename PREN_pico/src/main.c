#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "McuLib.h"
#include "McuWait.h"

#include "uart_communication.h"
#include "motor_ansteuerung.h"
#include "FreeRtosPren.h"



uint32_t SystemCoreClock = 120000000;



int main(void) {
    stdio_init_all();
    Motor_Ansteuerung_Init();
    
    //uart
    uart_Communication_Init();
    McuWait_Init();

    
    while(true){
        uart_communication_uart_test();
        McuWait_Waitms(5000);
        //uartread();
    }

    //FreeRtos
    //FreeRtosInit();
    //vTaskStartScheduler();

    //for debugging to see if the uart_test has completed

    for(;;) {}
    return 0;
}