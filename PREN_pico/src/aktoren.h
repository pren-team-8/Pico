#ifndef AKTOREN_H_
#define AKTOREN_H_

#include "McuGPIO.h"

void aktorenInit(void);
void pushHubmagnet();
bool Enschalter(McuGPIO_Handle_t Endschalter);
void Lautsprecher(McuGPIO_Handle_t Lautsprecher);

#endif