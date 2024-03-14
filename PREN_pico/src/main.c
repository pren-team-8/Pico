#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "McuLib.h"
#include "McuWait.h"

#include "uart_communication.h"
#include "motor_ansteuerung.h"


#define DEBUG   (1)

uint32_t SystemCoreClock = 120000000;



int main(void) {
    #if DEBUG /*workaroung for CMIS-DAP*/
        timer_hw->dbgpause=0;
    #endif
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
    Motor_Ansteuerung_Init();
    Motor_Hub_Test(100);



    //for debugging to see if the uart_test has completed

    for(;;) {}
    return 0;
}