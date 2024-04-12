#ifndef AKTOREN_H_
#define AKTOREN_H_

#include "McuGPIO.h"

void aktorenInit(void);
void pushHubmagnet(McuGPIO_Handle_t Hubmagnet);
bool Endschalter(McuGPIO_Handle_t Endschalter);
void Lautsprecher(McuGPIO_Handle_t Lautsprecher);

#endif