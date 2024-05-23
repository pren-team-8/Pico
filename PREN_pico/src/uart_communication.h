#ifndef UART_COMMUNICATION_H_
#define UART_COMMUNICATION_H_

void uart_send(char tosend[]);

void uart_Communication_Init(void);
void uart_Communication_Deinit(void);

void uartread(void);

#endif