#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
 
#define ADC_PIN_12_VOLT (26)
#define ADC_PIN_5_VOLT (27) 

void StromSensorInit(void){
    adc_init();
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    adc_gpio_init(27);
}


//not tested
uint16_t read_Sensor_12V(void){
    adc_select_input(0);
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();
    result = result * conversion_factor;
    return result;
}


//not tested
uint16_t read_Sensor_5V(void){
    adc_select_input(1);
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();
    result = result * conversion_factor;
    return result;
}