#include "blinky_rp_sdk.h"
#include "hardware/gpio.h"
#include "McuWait.h"


#if 0
static void delay(void) {
    for(volatile int i=0; i<5000000; i++) {
        __asm("nop");
    }
}
#endif

#if 1
void blinky_sdk_rp(void) {
    const unsigned int LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    for(int i = 0;i<10;i++) {
        gpio_put(LED_PIN, 0);
        //delay();
        McuWait_Waitms(500);
        gpio_put(LED_PIN, 1);
        //delay();
        McuWait_Waitms(500);
    }
}
#endif