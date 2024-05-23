#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
 
#define ADC_PIN_12_VOLT (26)
#define ADC_PIN_5_VOLT (27) 

void StromSensorInit(void){
    adc_init();
    // Make sure GPIO is high-impedance, no pullups etc
    adc_select_input(0);
    adc_gpio_init(26);
    adc_set_clkdiv(9600);       //clock divisor = 9600 -> 5`000 samples per second https://hackaday.io/project/180428-raspberry-pi-pico-adc-sampling-and-fft/details
    adc_fifo_setup(true, true, 4, false, false);
    //adc_irq_set_enabled(true);
    adc_run(true);

    adc_select_input(1);
    adc_gpio_init(27);
    adc_set_clkdiv(9600);       //clock divisor = 9600 -> 5`000 samples per second https://hackaday.io/project/180428-raspberry-pi-pico-adc-sampling-and-fft/details
    adc_fifo_setup(true, true, 4, false, false);
    //adc_irq_set_enabled(true);
    adc_run(true);
}


//tested
int read_Sensor_12V(void){
    adc_select_input(0);
    const float conversion_factor = (3.3/4096);
    float result_float = adc_read();
    result_float = result_float * conversion_factor;
    result_float = result_float -2.5;
    result_float = result_float *1.3;

    // Skalierung und Konvertierung in int
    int result_int = (int)(result_float * 1000);

    return result_int;
}


//tested
int read_Sensor_5V(void){
    adc_select_input(1);
    const float conversion_factor = (3.3/4096);
    float result_float = adc_read();
    result_float = result_float * conversion_factor;
    result_float = result_float -2.5;
    result_float = result_float *1.3;

    // Skalierung und Konvertierung in int
    int result_int = (int)(result_float * 1000);
    
    return result_int;
}