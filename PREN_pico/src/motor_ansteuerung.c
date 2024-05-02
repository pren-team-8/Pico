#include "McuLib.h"
#include "McuGPIO.h"
#include "McuWait.h"
#include "motor_ansteuerung.h"

// Hubeinheit
static McuGPIO_Handle_t Hub_Dir_Pin;
static McuGPIO_Handle_t Hub_Step_Pin;
static McuGPIO_Handle_t Hub_EN_Pin;
// Revolver
static McuGPIO_Handle_t Rev_Dir_Pin;
static McuGPIO_Handle_t Rev_Step_Pin;
static McuGPIO_Handle_t Rev_EN_Pin;


void Motor_Ansteuerung_Init(void){
    McuLib_Init();
    McuWait_Init();
    McuGPIO_Init();
    Motoren_Init();
}

void Motoren_Init(void){
    //HUB erstellt eine Konfigurationsvariable und holt die Standardkonfiguration
    McuGPIO_Config_t config_hub_dir;
    McuGPIO_GetDefaultConfig(&config_hub_dir);
    McuGPIO_Config_t config_hub_step;
    McuGPIO_GetDefaultConfig(&config_hub_step);
    McuGPIO_Config_t config_hub_EN;
    McuGPIO_GetDefaultConfig(&config_hub_EN);
    // die GPIO Pin Nummer zuweisen
    config_hub_dir.hw.pin = 11;
    config_hub_step.hw.pin = 10;
    config_hub_EN.hw.pin = 8;
    // device handle auf die Variable setzen
    Hub_Dir_Pin = McuGPIO_InitGPIO(&config_hub_dir);
    Hub_Step_Pin = McuGPIO_InitGPIO(&config_hub_step);
    Hub_EN_Pin = McuGPIO_InitGPIO(&config_hub_EN);
    // Als Output konfigurieren
    McuGPIO_SetAsOutput(Hub_Dir_Pin, true);
    McuGPIO_SetAsOutput(Hub_Step_Pin, true);
    McuGPIO_SetAsOutput(Hub_EN_Pin, true);

    //REVOLVER erstellt eine Konfigurationsvariable und holt die Standardkonfiguration
    McuGPIO_Config_t config_Rev_dir;
    McuGPIO_GetDefaultConfig(&config_Rev_dir);
    McuGPIO_Config_t config_Rev_step;
    McuGPIO_GetDefaultConfig(&config_Rev_step);
    McuGPIO_Config_t config_Rev_EN;
    McuGPIO_GetDefaultConfig(&config_Rev_EN);
    // die GPIO Pin Nummer zuweisen
    config_Rev_dir.hw.pin = 13;
    config_Rev_step.hw.pin = 19;
    config_Rev_EN.hw.pin = 9;
    // device handle auf die Variable setzen
    Rev_Dir_Pin = McuGPIO_InitGPIO(&config_Rev_dir);
    Rev_Step_Pin = McuGPIO_InitGPIO(&config_Rev_step);
    Rev_EN_Pin = McuGPIO_InitGPIO(&config_Rev_EN);
    // Als Output konfigurieren
    McuGPIO_SetAsOutput(Rev_Dir_Pin, true);
    McuGPIO_SetAsOutput(Rev_Step_Pin, true);
    McuGPIO_SetAsOutput(Rev_EN_Pin, true);
 }

void Hub_Bewegung(bool dir, uint16_t steps){
    //Bewegung ermöglichen
    McuGPIO_SetLow(Hub_EN_Pin);
    // direction
    if(dir){
        McuGPIO_SetHigh(Hub_Dir_Pin);
    }
    else {
        McuGPIO_SetLow(Hub_Dir_Pin);
    }
    // steps
    // while(true){
    //     McuGPIO_SetHigh(Hub_Dir_Pin);
    //     //McuWait_Waitms(2000);
    //     //for(int i=0 ; i<steps ; i++){
    //         McuGPIO_SetLow(Hub_Step_Pin);
    //         McuWait_Waitus(500);
    //         McuGPIO_SetHigh(Hub_Step_Pin);
    //         McuWait_Waitus(500);
    //     //}
        //McuWait_Waitms(2000);
        //McuGPIO_SetLow(Hub_Dir_Pin);  
        //McuWait_Waitms(2000);  
    for(int i=0 ; i<steps ; i++){
        McuGPIO_SetLow(Hub_Step_Pin);
        McuWait_Waitus(400); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
        McuGPIO_SetHigh(Hub_Step_Pin);
        McuWait_Waitus(400);
    }
    McuGPIO_SetHigh(Hub_EN_Pin);
}

void Rev_Bewegung(bool dir, uint16_t steps){
    McuGPIO_SetLow(Rev_EN_Pin);
    // direction
    if(dir){
        McuGPIO_SetHigh(Rev_Dir_Pin);
    }
    else {
        McuGPIO_SetLow(Rev_Dir_Pin);
    }
    // steps
    for(int i=0 ; i<steps ; i++){
        McuGPIO_SetLow(Rev_Step_Pin);
        McuWait_Waitus(400);
        McuGPIO_SetHigh(Rev_Step_Pin);
        McuWait_Waitus(400);
    }
    // McuGPIO_SetHigh(Rev_EN_Pin);
}