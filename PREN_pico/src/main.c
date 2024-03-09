#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "McuLib.h"
#include "McuWait.h"

#include "uart_communication.h"


#define DEBUG   (1)

uint32_t SystemCoreClock = 120000000;



int main(void) {
    #if DEBUG /*workaroung for CMIS-DAP*/
        timer_hw->dbgpause=0;
    #endif
    stdio_init_all();
    McuLib_Init();
    uart_Communication_Init();
    McuWait_Init();

    //uart_communication
    McuWait_Waitms(2000);
    uart_communication_uart_test();





    //for debugging to see if the uart_test has completed
    McuWait_Waitms(10000);
    McuWait_Waitms(500);  

    for(;;) {}
    return 0;
}