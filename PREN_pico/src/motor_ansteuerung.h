#ifndef MOTOR_ANSTEUERUNG__H_
#define MOTOR_ANSTEUERUNG__H_

// initialisiert alle Bibliotheken für die ansteuerung und konfiguration
void Motor_Ansteuerung_Init(void);

// initialisiert die Pins etc. für die Motoren
void Motoren_Init(void);

// Bewegungen
void Hub_Bewegung(bool dir,uint16_t steps);
void Rev_Bewegung(bool dir,uint16_t steps);

#endif