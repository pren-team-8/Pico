#ifndef UART_COMMUNICATION_H_
#define UART_COMMUNICATION_H_

/*!
 * Test function for sending from UART0 to UART1 of RP2040
 */
void uart_communication_uart_test(void);

void uart_Communication_Init(void);
void uart_Communication_Deinit(void);

#endif