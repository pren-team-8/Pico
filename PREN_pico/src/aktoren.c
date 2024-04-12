#include "aktoren.h"
#include "McuWait.h"
#include "McuLib.h"

//Hubmagnete
extern McuGPIO_Handle_t Hubmagnet1_Pin;
extern McuGPIO_Handle_t Hubmagnet2_Pin;
extern McuGPIO_Handle_t Hubmagnet3_Pin;
extern McuGPIO_Handle_t Hubmagnet4_Pin;

//Endschalter
extern McuGPIO_Handle_t Endschalter1_Pin;
extern McuGPIO_Handle_t Endschalter2_Pin;

//Lautsprecher
extern McuGPIO_Handle_t Lautsprecher_Pin;


void aktorenInit(void){
    McuGPIO_Init();

    McuGPIO_Config_t config_Hubmagnet1_Pin;
    McuGPIO_Config_t config_Hubmagnet2_Pin;
    McuGPIO_Config_t config_Hubmagnet3_Pin;
    McuGPIO_Config_t config_Hubmagnet4_Pin;
    McuGPIO_Config_t config_Endschalter1_Pin;
    McuGPIO_Config_t config_Endschalter2_Pin;
    McuGPIO_Config_t config_Lautsprecher_Pin;

    McuGPIO_GetDefaultConfig(&config_Hubmagnet1_Pin);
    McuGPIO_GetDefaultConfig(&config_Hubmagnet2_Pin);
    McuGPIO_GetDefaultConfig(&config_Hubmagnet3_Pin);
    McuGPIO_GetDefaultConfig(&config_Hubmagnet4_Pin);
    McuGPIO_GetDefaultConfig(&config_Endschalter1_Pin);
    McuGPIO_GetDefaultConfig(&config_Endschalter2_Pin);
    McuGPIO_GetDefaultConfig(&config_Lautsprecher_Pin);

    config_Hubmagnet1_Pin.hw.pin = 2;
    config_Hubmagnet2_Pin.hw.pin = 3;
    config_Hubmagnet3_Pin.hw.pin = 4;
    config_Hubmagnet4_Pin.hw.pin = 5;
    config_Endschalter1_Pin.hw.pin = 6;
    config_Endschalter2_Pin.hw.pin = 7;
    config_Lautsprecher_Pin.hw.pin = 15;

    //Inputs
        //konfiguriert die Pins als input
    config_Endschalter1_Pin.isInput = true;
    config_Endschalter2_Pin.isInput = true;
        //Pull-Up aktivieren
    config_Endschalter1_Pin.hw.pull = McuGPIO_PULL_UP;
    config_Endschalter2_Pin.hw.pull = McuGPIO_PULL_UP;
        //init
    Endschalter1_Pin = McuGPIO_InitGPIO(&config_Endschalter1_Pin);
    Endschalter2_Pin = McuGPIO_InitGPIO(&config_Endschalter2_Pin);


    //Outputs
        //init
    Hubmagnet1_Pin = McuGPIO_InitGPIO(&config_Hubmagnet1_Pin);
    Hubmagnet2_Pin = McuGPIO_InitGPIO(&config_Hubmagnet2_Pin);
    Hubmagnet3_Pin = McuGPIO_InitGPIO(&config_Hubmagnet3_Pin);
    Hubmagnet4_Pin = McuGPIO_InitGPIO(&config_Hubmagnet4_Pin);
    Lautsprecher_Pin = McuGPIO_InitGPIO(&config_Lautsprecher_Pin);
        //output
    McuGPIO_SetAsOutput(Hubmagnet1_Pin, false);
    McuGPIO_SetAsOutput(Hubmagnet2_Pin, false);
    McuGPIO_SetAsOutput(Hubmagnet3_Pin, false);
    McuGPIO_SetAsOutput(Hubmagnet4_Pin, false);
    McuGPIO_SetAsOutput(Lautsprecher_Pin, false);
}

//tested
void pushHubmagnet(McuGPIO_Handle_t Hubmagnet){
    McuGPIO_SetHigh(Hubmagnet);
    McuWait_Waitms(300);
    McuGPIO_SetLow(Hubmagnet);
}

//not tested
bool Endschalter(McuGPIO_Handle_t Endschalter){
    if(McuGPIO_IsLow(Endschalter)){
        return false;
    } else {
        return true;
    }
}

//not tested
void Lautsprecher(McuGPIO_Handle_t Lautsprecher){
    McuGPIO_SetHigh(Lautsprecher);
    McuWait_Waitms(2000);
    McuGPIO_SetLow(Lautsprecher);
}