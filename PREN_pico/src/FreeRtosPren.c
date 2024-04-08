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

static void Strommessung(void *pv) {
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(10000));
        //TODO Strommessung
        
    }
}

static void Ansteuerung(void *pv) {
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(5000));
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

