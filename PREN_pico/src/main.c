#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "McuLib.h"
#include "McuWait.h"

#include "uart_communication.h"
#include "motor_ansteuerung.h"
#include "FreeRtosPren.h"
#include "aktoren.h"

#include "McuRB.h"

//Ringbuffer
McuRB_Handle_t Ringbuffer = NULL;
McuRB_Handle_t UartReceiveBuffer = NULL;


//MCUwait
uint32_t SystemCoreClock = 120000000;

//test
char test1;


int main(void) {
    stdio_init_all();
    McuWait_Init();

    /*INIT*/
    //Motor Init
    Motor_Ansteuerung_Init();
    //uart
    uart_Communication_Init();
    //aktoren
    aktorenInit();
    // char Temp[] = {'P','o','s'};
    // for(int i = 0; i<3; i++){
    //     McuRB_Put(Ringbuffer,&Temp[i]);
    // }

    //Hub_Bewegung(true,3000);
    // while(true){
    //     pushHubmagnet();
    // }
    //FreeRtos start
    FreeRtosInit();
    vTaskStartScheduler();

    for(;;) {}
    return 0;
}