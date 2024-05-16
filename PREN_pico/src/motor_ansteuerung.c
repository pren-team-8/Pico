#include "McuLib.h"
#include "McuGPIO.h"
#include "McuWait.h"
#include "motor_ansteuerung.h"
#include "aktoren.h"

// Hubeinheit
static McuGPIO_Handle_t Hub_Dir_Pin;
static McuGPIO_Handle_t Hub_Step_Pin;
static McuGPIO_Handle_t Hub_EN_Pin;
// Revolver
static McuGPIO_Handle_t Rev_Dir_Pin;
static McuGPIO_Handle_t Rev_Step_Pin;
extern McuGPIO_Handle_t Rev_EN_Pin;
// Hubmagnete
extern McuGPIO_Handle_t Endschalter1_Pin;
extern McuGPIO_Handle_t Endschalter2_Pin;


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
    uint16_t maxpause = 1200;
    uint16_t minpause = 900;
    uint16_t pause = (maxpause-minpause)/100;
    int Verzögerung = 0;
    McuGPIO_SetLow(Hub_EN_Pin);
    // direction
    if(dir){
        McuGPIO_SetHigh(Hub_Dir_Pin);
    }
    else {
        McuGPIO_SetLow(Hub_Dir_Pin);
    }
    

    if(steps<=200){ //Schrittmotor kann nicht richtig beschleunigen da zu wenig steps
        for(int i = 0; i<(steps/2);i++){ //Beschleunigung
            McuGPIO_SetLow(Hub_Step_Pin);
            McuWait_Waitus(maxpause-(pause*Verzögerung)); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
            McuGPIO_SetHigh(Hub_Step_Pin);
            McuWait_Waitus(maxpause-(pause*Verzögerung));
            Verzögerung++;
        }
        for(int i = 0; i<steps;i++){ //Bremsvorgang
            McuGPIO_SetLow(Hub_Step_Pin);
            McuWait_Waitus(maxpause-(pause*Verzögerung)); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
            McuGPIO_SetHigh(Hub_Step_Pin);
            McuWait_Waitus(maxpause-(pause*Verzögerung));
            Verzögerung--;
        }
    } else if(steps>200) { // genügend steps für fullspeed
        for(int i=0 ; i<steps ; i++){
            if(i<100){ // Beschleunigung
                McuGPIO_SetLow(Hub_Step_Pin);
                McuWait_Waitus(maxpause-(pause*Verzögerung)); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
                McuGPIO_SetHigh(Hub_Step_Pin);
                McuWait_Waitus(maxpause-(pause*Verzögerung));
                Verzögerung++;
            } else if(i>(steps-100)) { // Bremsvorgang
                McuGPIO_SetLow(Hub_Step_Pin);
                McuWait_Waitus(maxpause-(pause*Verzögerung)); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
                McuGPIO_SetHigh(Hub_Step_Pin);
                McuWait_Waitus(maxpause-(pause*Verzögerung));
                Verzögerung--;
            } else { // Fullspeed
                McuGPIO_SetLow(Hub_Step_Pin);
                McuWait_Waitus(minpause); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
                McuGPIO_SetHigh(Hub_Step_Pin);
                McuWait_Waitus(minpause);
            }
        }
    }
    McuGPIO_SetHigh(Hub_EN_Pin);
}

void Rev_Bewegung(bool dir, uint16_t steps){
     //Bewegung ermöglichen
    uint16_t maxpause = 3000;
    uint16_t minpause = 2200;
    McuGPIO_SetLow(Rev_EN_Pin);
    // direction
    if(dir){
        McuGPIO_SetHigh(Rev_Dir_Pin);
    }
    else {
        McuGPIO_SetLow(Rev_Dir_Pin);
    }
    uint16_t pause = (maxpause-minpause)/150;
    int Verzögerung = 0;

    if(steps<=300){ //Schrittmotor kann nicht richtig beschleunigen da zu wenig steps
        for(int i = 0; i<(steps/2);i++){ //Beschleunigung
            McuGPIO_SetLow(Rev_Step_Pin);
            McuWait_Waitus(maxpause-(pause*Verzögerung)); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
            McuGPIO_SetHigh(Rev_Step_Pin);
            McuWait_Waitus(maxpause-(pause*Verzögerung));
            Verzögerung++;
        }
        for(int i = 0; i<steps;i++){ //Bremsvorgang
            McuGPIO_SetLow(Rev_Step_Pin);
            McuWait_Waitus(maxpause-(pause*Verzögerung)); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
            McuGPIO_SetHigh(Rev_Step_Pin);
            McuWait_Waitus(maxpause-(pause*Verzögerung));
            Verzögerung--;
        }
    } else if(steps>300) { // genügend steps für fullspeed
        for(int i=0 ; i<steps ; i++){
            if(i<150){ // Beschleunigung
                McuGPIO_SetLow(Rev_Step_Pin);
                McuWait_Waitus(maxpause-(pause*Verzögerung)); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
                McuGPIO_SetHigh(Rev_Step_Pin);
                McuWait_Waitus(maxpause-(pause*Verzögerung));
                Verzögerung++;
            } else if(i>(steps-150)) { // Bremsvorgang
                McuGPIO_SetLow(Rev_Step_Pin);
                McuWait_Waitus(maxpause-(pause*Verzögerung)); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
                McuGPIO_SetHigh(Rev_Step_Pin);
                McuWait_Waitus(maxpause-(pause*Verzögerung));
                Verzögerung--;
            } else { // Fullspeed
                McuGPIO_SetLow(Rev_Step_Pin);
                McuWait_Waitus(minpause); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
                McuGPIO_SetHigh(Rev_Step_Pin);
                McuWait_Waitus(minpause);
            }

        }
    }
//     McuGPIO_SetHigh(Rev_EN_Pin);
}

void Rev_Init(void){

    uint16_t minpause = 2200;
    McuGPIO_SetLow(Rev_EN_Pin);
    McuGPIO_SetHigh(Rev_Dir_Pin);
    
    while(Endschalter(Endschalter1_Pin) == false){
        McuGPIO_SetLow(Rev_Step_Pin);
        McuWait_Waitus(minpause); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
        McuGPIO_SetHigh(Rev_Step_Pin);
        McuWait_Waitus(minpause);
    }
    minpause = 9000;
    for(int i = 0; i<20; i++){
        McuGPIO_SetLow(Rev_Step_Pin);
        McuWait_Waitus(minpause); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
        McuGPIO_SetHigh(Rev_Step_Pin);
        McuWait_Waitus(minpause);
    }
    McuGPIO_SetLow(Rev_Dir_Pin);
   while(Endschalter(Endschalter1_Pin) == false){
        McuGPIO_SetLow(Rev_Step_Pin);
        McuWait_Waitus(minpause); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
        McuGPIO_SetHigh(Rev_Step_Pin);
        McuWait_Waitus(minpause);
    }
    while(Endschalter(Endschalter1_Pin) == true){
        McuGPIO_SetLow(Rev_Step_Pin);
        McuWait_Waitus(minpause); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
        McuGPIO_SetHigh(Rev_Step_Pin);
        McuWait_Waitus(minpause);
    }
    for(int i = 0; i<10; i++){
        McuGPIO_SetLow(Rev_Step_Pin);
        McuWait_Waitus(minpause); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
        McuGPIO_SetHigh(Rev_Step_Pin);
        McuWait_Waitus(minpause);
    }
    Rev_Bewegung(true, 26);
}

void Hub_Init(void){

    uint16_t maxpause = 1200;
    McuGPIO_SetLow(Hub_EN_Pin);
    // direction
    McuGPIO_SetHigh(Hub_Dir_Pin);
    
     while(Endschalter(Endschalter2_Pin) == false){
        McuGPIO_SetLow(Hub_Step_Pin);
        McuWait_Waitus(maxpause); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
        McuGPIO_SetHigh(Hub_Step_Pin);
        McuWait_Waitus(maxpause);
    }
    McuWait_Waitms(100);
    Hub_Bewegung(false, 2480);
   
}

void Hub_Ende(void){
    Hub_Bewegung(true, 2400);

    McuGPIO_SetLow(Hub_EN_Pin);
    // direction
    McuGPIO_SetHigh(Hub_Dir_Pin);
    uint16_t maxpause = 1200;
    while(Endschalter(Endschalter2_Pin) == false){
        McuGPIO_SetLow(Hub_Step_Pin);
        McuWait_Waitus(maxpause); //750          //400 MINIMAL Pause bei Half Step => Max.Geschwindigkeit
        McuGPIO_SetHigh(Hub_Step_Pin);
        McuWait_Waitus(maxpause);
    }
    McuGPIO_SetHigh(Hub_EN_Pin);
    McuGPIO_SetHigh(Rev_EN_Pin);
}
