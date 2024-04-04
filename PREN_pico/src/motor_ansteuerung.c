#include "McuLib.h"
#include "McuGPIO.h"
#include "McuWait.h"
#include "motor_ansteuerung.h"

// Hubeinheit
static McuGPIO_Handle_t Hub_Dir_Pin;
static McuGPIO_Handle_t Hub_Step_Pin;
// Revolver
static McuGPIO_Handle_t Rev_Dir_Pin;
static McuGPIO_Handle_t Rev_Step_Pin;


void Motor_Ansteuerung_Init(void){
    McuLib_Init();
    McuWait_Init();
    McuGPIO_Init();
    Motoren_Init();
}

void Motoren_Init(void){
    // erstellt eine Konfigurationsvariable und holt die Standardkonfiguration
    McuGPIO_Config_t config_hub_dir;
    McuGPIO_GetDefaultConfig(&config_hub_dir);
    McuGPIO_Config_t config_hub_step;
    McuGPIO_GetDefaultConfig(&config_hub_step);
    // die GPIO Pin Nummer zuweisen
    config_hub_dir.hw.pin = 11;
    config_hub_step.hw.pin = 13;
    // device handle auf die Variable setzen
    Hub_Dir_Pin = McuGPIO_InitGPIO(&config_hub_dir);
    Hub_Step_Pin = McuGPIO_InitGPIO(&config_hub_step);
    // Als Output konfigurieren
    McuGPIO_SetAsOutput(Hub_Dir_Pin, false);
    McuGPIO_SetAsOutput(Hub_Step_Pin, false);
}

void Hub_Bewegung(bool dir, uint16_t steps){
    // direction
    if(dir){
        McuGPIO_SetHigh(Hub_Dir_Pin);
    }
    else {
        McuGPIO_SetLow(Hub_Dir_Pin);
    }
    // steps
    while(true){
        McuGPIO_SetHigh(Hub_Dir_Pin);
        //McuWait_Waitms(2000);
        //for(int i=0 ; i<steps ; i++){
            McuGPIO_SetLow(Hub_Step_Pin);
            McuWait_Waitus(500);
            McuGPIO_SetHigh(Hub_Step_Pin);
            McuWait_Waitus(500);
        //}
        //McuWait_Waitms(2000);
        //McuGPIO_SetLow(Hub_Dir_Pin);  
        //McuWait_Waitms(2000);  
        //for(int i=0 ; i<steps ; i++){
        //    McuGPIO_SetLow(Hub_Step_Pin);
        //    McuWait_Waitus(500);
        //    McuGPIO_SetHigh(Hub_Step_Pin);
        //    McuWait_Waitus(500);
        //}
        //McuWait_Waitms(2000);
    }

}

void Rev_Bewegung(bool dir, uint16_t steps){
    // direction
    if(dir){
        McuGPIO_SetHigh(Rev_Dir_Pin);
    }
    else {
        McuGPIO_SetLow(Rev_Dir_Pin);
    }
    // steps
    for(int i=0 ; i<steps ; i++){
        McuGPIO_SetHigh(Rev_Step_Pin);
        McuWait_Waitms(50);
        McuGPIO_SetLow(Rev_Step_Pin);
        McuWait_Waitms(50);
    } 
}