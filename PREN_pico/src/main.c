#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "McuLib.h"
#include "McuWait.h"
#include "McuGPIO.h"

#include "uart_communication.h"
#include "motor_ansteuerung.h"
#include "FreeRtosPren.h"
#include "aktoren.h"

#include "McuRB.h"

#define DEBUG (1)

//Ringbuffer
McuRB_Handle_t Ringbuffer = NULL;
McuRB_Handle_t UartReceiveBuffer = NULL;

//Hubmagnete
McuGPIO_Handle_t Hubmagnet1_Pin;
McuGPIO_Handle_t Hubmagnet2_Pin;
McuGPIO_Handle_t Hubmagnet3_Pin;
McuGPIO_Handle_t Hubmagnet4_Pin;

//Endschalter
McuGPIO_Handle_t Endschalter1_Pin;
McuGPIO_Handle_t Endschalter2_Pin;

//Lautsprecher
McuGPIO_Handle_t Lautsprecher_Pin;

//MCUwait
uint32_t SystemCoreClock = 120000000;


int main(void) {

     // #if DEBUG
     //      timer_hw->dbgpause = 0;
     // #endif
     // Ringbuffer = NULL;

    stdio_init_all();
    McuWait_Init();

    /*INIT*/
    //Motor Init
    Motor_Ansteuerung_Init();
    //uart
//     uart_Communication_Init();
    //aktoren
    aktorenInit();

//     FreeRtosInit();
//     vTaskStartScheduler();

    //  pushHubmagnet(Hubmagnet1_Pin);
    //  McuWait_Waitms(500);
    //  pushHubmagnet(Hubmagnet2_Pin);
    //  McuWait_Waitms(500);
    //  pushHubmagnet(Hubmagnet3_Pin);
    //  McuWait_Waitms(500);
    //  pushHubmagnet(Hubmagnet4_Pin);
    //  McuWait_Waitms(500);

    // bool direction = true;
    //  Rev_Bewegung(true, 1280);          //Während ganzem Programm Enable auf LOW =>Haltemoment gewährleistet.
    //  pushHubmagnet(Hubmagnet2_Pin);     //Anpassen damit Aktoren auf Enable PIN zugreifen können.
    //  McuWait_Waitms(200);
    //  Rev_Bewegung(true,1650);
    //  pushHubmagnet(Hubmagnet3_Pin);
    //  McuWait_Waitms(200);
    //  Rev_Bewegung(false,1650);
    //  pushHubmagnet(Hubmagnet2_Pin);
    //  McuWait_Waitms(200);
    //  Rev_Bewegung(true,1650);
    //  pushHubmagnet(Hubmagnet3_Pin);
    //  McuWait_Waitms(200);
    //  Hub_Bewegung(false,1000);
    //  Hub_Bewegung(true,6000);
    bool direction = true;
    while(true){
    Hub_Bewegung(direction,2000);
    Rev_Bewegung(direction,800);
    direction = !direction;
    }

    for(;;) {}
    return 0;
}