#include "driver_gpio.h"

void delay() {
    for (int i = 0; i < 1000; i++) {
        asm ("nop");
    }
}  

int main() {
    gpio_begin();
    gpio_configure(GPIOC, 0, GPIO_MODE_OUTPUT_30MHZ, GPIO_CNF_ANALOG_PP);
    while (1) {
        gpio_set(GPIOC, 0);
        delay();
        gpio_reset(GPIOC, 0);
        delay();
    }    
}  
