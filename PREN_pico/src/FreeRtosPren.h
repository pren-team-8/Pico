#include "McuRTOS.h"

#ifndef FREERTOS__H_
#define FREERTOS__H_

// initialisiert alles für das Betriebssystem FreeRtos
void FreeRtosInit(void);

void RevolverLogik(char pos,char col);
void CommandEnd(void);


#endif