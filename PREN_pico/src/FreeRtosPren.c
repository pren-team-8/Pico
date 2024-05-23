#include "FreeRtosPren.h"
#include "McuLib.h"
#include "motor_ansteuerung.h"
#include "uart_communication.h"
#include <string.h>
#include <stdio.h>
#include "McuWait.h"
#include "McuRB.h"
#include "McuGPIO.h"
#include "StromSensor.h"
#include "aktoren.h"

//Steps für 90 GRad
#define STEPS_FOR_NINETEEN_DEGRES (67)

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
char Col;       //1 = Blau, 2 = Gelb, 3 = Rot

//Char-Array für die Positionen des Revolvers
char RevolverPos1[4] = {'1', '2', '0', '3'};
char RevolverPos2[4] = {'2', '0', '3', '1'};
char RevolverPos3[4] = {'0', '3', '1', '2'};
char RevolverPos4[4] = {'3', '1', '2', '0'};

//Positionsortung des Revolvers
bool direction = 0;      // 1 = Uhrzeigersinn, 0 = gegenUhrzeigersinn
uint8_t steps = 0;          // Wieviele Steps muss der Motor machen

//Momentanposition des Revolvers
uint8_t RevolverCurrentPos = 1;
uint8_t RevolverPos = 1;      

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
    } 
    else if(RevolverCurrentPos == RevolverPos){
        steps = 0;
    }
        else {
        //ERROR
    }
    //Steps
    steps = steps * STEPS_FOR_NINETEEN_DEGRES;
    Rev_Bewegung(direction,steps);
    //update der RevolverPosition
    RevolverCurrentPos = RevolverPos;

    //Welches Hubmagnet muss auslösen
    //Datentyp casten
    int pos_int = pos -'0';
    switch(pos_int) {
        case 1:
            McuWait_Waitms(250);
            pushHubmagnet(Hubmagnet1_Pin);
            McuWait_Waitms(250);
            break;
        case 2:
            McuWait_Waitms(250);
            pushHubmagnet(Hubmagnet2_Pin);
            McuWait_Waitms(250);
            break;
        case 3:
            McuWait_Waitms(250);
            pushHubmagnet(Hubmagnet3_Pin);
            McuWait_Waitms(250);
            break;
        case 4:
            McuWait_Waitms(250);
            pushHubmagnet(Hubmagnet4_Pin);
            McuWait_Waitms(250);
            break;
        default:
            //Fehlerbehandlung
            break;
    }
}

void CommandEnd(void){  // Alles für den Befehl End. Herunterfahren und zusammenstossen. Hochfahren. Sobald nach oben gefahren, Buzzer auslösen.
    Hub_Bewegung(false, 450);
    Hub_Ende();
    Lautsprecher(Lautsprecher_Pin);
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
        //static char Wert5V[4] = ""; // String zum Speichern des Werts
        //static char Wert12V[4] = ""; // String zum Speichern des Werts
            
        // Umwandlung in String und Auslesung des Sensors
        static char result_str_5V[4] = "";
        sprintf(result_str_5V, "%d", read_Sensor_5V());

        // Umwandlung in String und Auslesung des Sensors
        static char result_str_12V[4] = "";
        sprintf(result_str_12V, "%d", read_Sensor_12V());

        // Den Wert schicken
        uart_send(result_str_5V);
        uart_send("\n\r");
        uart_send(result_str_12V);
        uart_send("\n\r");
    }
}

static void Ansteuerung(void *pv) {
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(10000));
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
            // } else {
            //     while(McuRB_Peek())
            }
        }
        uart_send("OK");
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

