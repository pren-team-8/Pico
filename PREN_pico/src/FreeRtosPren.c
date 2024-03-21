#include "FreeRtosPren.h"
#include "McuLib.h"
#include "motor_ansteuerung.h"
#include "uart_communication.h"

#include "blinky_rp_sdk.h"

static void Strommessung(void *pv) {
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(10000));
        blinky_sdk_rp();
    }
}

static void Ansteuerung(void *pv) {
    for(;;){
        uartread();
        //vTaskDelay(pdMS_TO_TICKS(500));
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

