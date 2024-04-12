#include "FreeRtosPren.h"
#include "McuLib.h"
#include "motor_ansteuerung.h"
#include "uart_communication.h"
#include <string.h>
#include "McuWait.h"
#include "McuRB.h"
#include "McuGPIO.h"

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
char Col;

void RevolverLogik(char pos,char col){ // Hier sollte die Logik entstehen, wie man nun den Revolver bewegen muss damit der Würfel mit der entsprechenden Farbe am richtigen Ort liegt

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
                        McuRB_Get(Ringbuffer,&ElementRingBuffer);
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

