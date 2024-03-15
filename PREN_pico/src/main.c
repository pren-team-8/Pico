#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "McuLib.h"
#include "McuWait.h"

#include "uart_communication.h"
#include "motor_ansteuerung.h"


uint32_t SystemCoreClock = 120000000;



int main(void) {
    stdio_init_all();
    Motor_Ansteuerung_Init();
    
    //uart
    /*
    uart_Communication_Init();
    McuWait_Init();

    //uart_communication
    McuWait_Waitms(2000);
    uart_communication_uart_test();
    */
    //motor
    Hub_Bewegung(1,1000);



    //for debugging to see if the uart_test has completed

    for(;;) {}
    return 0;
}