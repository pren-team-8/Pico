#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "uart_communication.h"
#include "McuWait.h"
#include "McuRB.h"

#define UART_ID_UART0 uart0
#define UART_ID_UART1 uart1
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN_UART0 0
#define UART_RX_PIN_UART0 1

#define UART_TX_PIN_UART1 4
#define UART_RX_PIN_UART1 5

// Ringbuffer
extern McuRB_Handle_t Ringbuffer;
McuRB_Config_t Ringbuffer_config;
char Uart[100];
char tempUartArray[5];
char tempUart;
uint8_t tempUart_index = 0;
uint8_t uart_index = 0;

char uartxBuffer[100];
uint8_t indexxBuffer = 0;



//ISR Callback Function for UART0 RX Interrupt
void on_uart_rx_uart0() {
    while (uart_is_readable(UART_ID_UART0)) { // uart_is_readable -> Determine whether data is waiting in the RX FIFO.
        char temp = uart_getc(UART_ID_UART0);
        McuRB_Put(Ringbuffer,&temp);
        McuRB_Get(Ringbuffer,&uartxBuffer[indexxBuffer]);
        indexxBuffer++;
    }
    //tempUart[uart_index] = '\0';
    //char Uart_receive = tempUart;
    //for(int h = 0;h<5;h++){
    //    McuRB_Get(Ringbuffer,&tempUart);
    //    tempUartArray[h] = tempUart;
    //}
    //McuWait_Waitms(50);
}

//ISR Callback Function for UART1 RX Interrupt
void on_uart_rx_uart1() {
    while (uart_is_readable(UART_ID_UART1)) { // uart_is_readable -> Determine whether data is waiting in the RX FIFO.
        char j = uart_getc(UART_ID_UART1);
        uint8_t result = McuRB_Put(Ringbuffer,&j);
        // if(result != "ERR_OK"){
        //     //Error Handling
        // }
    }
}

void uart_communication_uart_test(void){
    uart_puts(UART_ID_UART0, "Hallo");
    McuWait_WaitCycles(125000000);  //Wait approx. 1 second
    uart_puts(UART_ID_UART1, "UART1 TO UART0");
}

void uart_Communication_Init() {

    //Init Ringbuffer
    McuRB_Init();
    McuRB_GetDefaultconfig(&Ringbuffer_config);
    Ringbuffer = McuRB_InitRB(&Ringbuffer_config);

    McuRB_GetDefaultconfig(&Ringbuffer_config);
    Ringbuffer = McuRB_InitRB(&Ringbuffer_config);

    // Set up our UART with a basic baud rate.
    uart_init(UART_ID_UART0, 2400);
    uart_init(UART_ID_UART1, 2400);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN_UART0, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN_UART0, GPIO_FUNC_UART);

    gpio_set_function(UART_TX_PIN_UART1, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN_UART1, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    int __unused actual_uart0 = uart_set_baudrate(UART_ID_UART0, BAUD_RATE);
    int __unused actual_uart1 = uart_set_baudrate(UART_ID_UART1, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID_UART0, false, false);
    uart_set_hw_flow(UART_ID_UART1, false, false);

    // Set our data format
    uart_set_format(UART_ID_UART0, DATA_BITS, STOP_BITS, PARITY);
    uart_set_format(UART_ID_UART1 , DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID_UART0, false);
    uart_set_fifo_enabled(UART_ID_UART1, false);

    
    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ_UART0 = UART_ID_UART0 == uart0 ? UART0_IRQ : UART1_IRQ;
    int UART_IRQ_UART1 = UART_ID_UART1 == uart0 ? UART0_IRQ : UART1_IRQ;
    
    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ_UART0, on_uart_rx_uart0);
    irq_set_enabled(UART_IRQ_UART0, true);
    irq_set_exclusive_handler(UART_IRQ_UART1, on_uart_rx_uart1);
    irq_set_enabled(UART_IRQ_UART1, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID_UART0, true, false);
    uart_set_irq_enables(UART_ID_UART1, true, false);

    // OK, all set up.
}

void uart_Communication_Deinit(void) {

}