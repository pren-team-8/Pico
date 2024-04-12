#include "FreeRtosPren.h"
#include "McuLib.h"
#include "motor_ansteuerung.h"
#include "uart_communication.h"
#include <string.h>
#include "McuWait.h"
#include "McuRB.h"

//Ringbuffer
extern McuRB_Handle_t Ringbuffer;
char Befehl[5];
uint8_t index_Befehl = 0;
char data;

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

        //while (data != '\040')
        
        // for(int x = 0; x < 5 ; x++)
        // {
        //     McuRB_Get(Ringbuffer,&data);
        //     if(data == '\000'){
        //         //Buffer leer
        //         break;
        //     }
        //     Befehl[index_Befehl] = data;
        // }
        // char Befehl1 = (char)Befehl;
        // index_Befehl = 0;
        // if(Befehl[0] != '\000'){
        //     McuWait_Waitms(10);
        // }
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

