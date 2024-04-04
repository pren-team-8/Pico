#include "FreeRtosPren.h"
#include "McuLib.h"
#include "motor_ansteuerung.h"
#include "uart_communication.h"
#include <string.h>
#include "McuWait.h"

//uart
extern char uartRxBuffer[100];
extern uint8_t indexRxBuffer;

static void Strommessung(void *pv) {
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(10000));
        //TODO Strommessung
        
    }
}

static void Ansteuerung(void *pv) {
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(500));
        //TODO Ansteuerung
            // Überprüfen, ob es genug Zeichen im Puffer gibt, um einen vollständigen Befehl zu lesen
        while (indexRxBuffer >= 10) {
            // Extrahiere den Befehl aus den ersten 10 Zeichen im Puffer
            char command[11]; // Stellen Sie sicher, dass ein zusätzliches Zeichen für das Null-Byte vorhanden ist
            memcpy(command, uartRxBuffer, 10);
            command[10] = '\0'; // Null-Byte hinzufügen, um eine C-String zu erstellen

            // Hier können Sie den Befehl verarbeiten, z.B. anhand einer Switch-Anweisung
            // Die Verarbeitung des Befehls erfolgt basierend auf dem Wert von command
            // Nehmen Sie an, dass jeder Befehl aus 10 Zeichen besteht
            if (strcmp(command, "Test      ") == 0) {
                McuWait_Waitms(10);
               // Aktion für den Befehl "COMMAND001"
            } else if (strcmp(command, "COMMAND002") == 0) {
                // Aktion für den Befehl "COMMAND002"
            } else {
                // Unbekannter Befehl, möglicherweise Fehlerbehandlung
            }

        // Verschieben Sie den Index, um den verarbeiteten Befehl aus dem Puffer zu entfernen
        //indexRxBuffer -= 10;
        //memmove(uartRxBuffer, uartRxBuffer + 10, indexRxBuffer);
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

