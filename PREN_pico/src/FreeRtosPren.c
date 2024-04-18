#include "FreeRtosPren.h"
#include "McuLib.h"
#include "motor_ansteuerung.h"
#include "uart_communication.h"
#include <string.h>
#include "McuWait.h"
#include "McuRB.h"
#include "McuGPIO.h"

//Steps für 90 GRad
#define STEPS_FOR_NINETEEN_DEGRES (90)

//Ringbuffer
extern McuRB_Handle_t Ringbuffer;
char Befehl[5];
uint8_t index_Befehl = 0;
char data;

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

//test
char uartRxBuffer[100];
uint8_t indexRxBuffer = 0;

//Hier wird das Element gespeichert, welches aus dem Ringbuffer herausgezogen wird
char ElementRingBuffer;

//Positionspeicherung an welcher die Würfel liegen
char Pos;
char Col;       //1 = Rot, 2 = Blau, 3 = Gelb

//Char-Array für die Positionen des Revolvers
char RevolverPos1[4] = {'1', '2', '3', '0'};
char RevolverPos2[4] = {'0', '1', '2', '3'};
char RevolverPos3[4] = {'3', '0', '1', '2'};
char RevolverPos4[4] = {'2', '3', '0', '1'};

//Positionsortung des Revolvers
bool direction = 0;      // 1 = Uhrzeigersinn, 0 = gegenUhrzeigersinn
uint8_t steps = 0;          // Wieviele Steps muss der Motor machen

//Momentanposition des Revolvers
uint8_t RevolverCurrentPos = 0;
uint8_t RevolverPos = 0;      

void RevolverLogik(char pos,char col){ // Hier sollte die Logik entstehen, wie man nun den Revolver bewegen muss damit der Würfel mit der entsprechenden Farbe am richtigen Ort liegt
    // - 48 um ein char in ein int umzuwandeln, 1 laut Asci Tabelle Dec 49
    //An welche Position muss der Revolver
    if(RevolverPos1[pos-49] == col){
        RevolverPos = 1;
    } else if(RevolverPos2[pos-49] == col){
        RevolverPos = 2;
    } else if(RevolverPos3[pos-49] == col){
        RevolverPos = 3;
    } else if(RevolverPos4[pos-49] == col){
        RevolverPos = 4;
    } else {
        //ERROR
    }
    //Direction
    if(RevolverCurrentPos > RevolverPos){
        direction = 0;
        steps = RevolverCurrentPos - RevolverPos;
    } else if (RevolverCurrentPos < RevolverPos){
        direction = 1;
        steps = RevolverPos - RevolverCurrentPos;
    } else {
        //ERROR
    }
    //Steps
    steps = steps * STEPS_FOR_NINETEEN_DEGRES;
    Rev_Bewegung(direction,steps);
}

void CommandEnd(void){  // Alles für den Befehl End. Herunterfahren und zusammenstossen. Hochfahren. Sobald nach oben gefahren, Buzzer auslösen.

}

void CommandPos(void){  // Alles für den Befehl Pos. Color muss noch ausgewertet werden
    McuRB_Get(Ringbuffer,&Pos); //Position an der der entsprechende Würfel liegen muss.
    McuRB_Get(Ringbuffer,&ElementRingBuffer); //C
    McuRB_Get(Ringbuffer,&ElementRingBuffer); //o
    McuRB_Get(Ringbuffer,&ElementRingBuffer); //l
    McuRB_Get(Ringbuffer,&Col); //Welche Farbe an der entsprechenden Position liegen muss.
    RevolverLogik(Pos,Col);
}

static void Strommessung(void *pv) {
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(10000));
        //TODO Strommessung
        
    }
}

static void Ansteuerung(void *pv) {
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(20));
        McuRB_Get(Ringbuffer,&ElementRingBuffer);
        if(ElementRingBuffer == '\000'){ //Befehl noch nicht angekommen oder keiner vorhanden
            
        } else {    //Befehl vorhanden, Auswertung folgt
            if(ElementRingBuffer == 'P'){
                McuRB_Get(Ringbuffer,&ElementRingBuffer);
                if(ElementRingBuffer == 'o'){
                    McuRB_Get(Ringbuffer,&ElementRingBuffer);
                    if(ElementRingBuffer == 's');   //Befehl POS
                    {
                        //McuRB_Get(Ringbuffer,&ElementRingBuffer);
                        CommandPos();
                    }
                }
            } else if (ElementRingBuffer == 'E'){
                McuRB_Get(Ringbuffer,&ElementRingBuffer);
                if(ElementRingBuffer == 'n'){
                    McuRB_Get(Ringbuffer,&ElementRingBuffer);
                    if(ElementRingBuffer == 'd');   //Befehl END
                    {
                        CommandEnd();
                    }
                }
            }
        }
    }
}

void InitTaskStrommessung(void){
    if(xTaskCreate(Strommessung,
    "Strommessung",
    600/sizeof(StackType_t),
    (void*)NULL,
    tskIDLE_PRIORITY+3,
    (TaskHandle_t*)NULL
    ) != pdPASS)
    {
        for(;;){} 
    }
}

void InitTaskAnsteuerung(void){
   if(xTaskCreate(Ansteuerung,
    "Ansteuerung",
    600/sizeof(StackType_t),
    (void*)NULL,
    tskIDLE_PRIORITY+2,
    (TaskHandle_t*)NULL
    ) != pdPASS)
    {
        for(;;){} 
    }
}



void FreeRtosInit(void){
    McuRTOS_Init();
    InitTaskAnsteuerung();
    InitTaskStrommessung();
}

