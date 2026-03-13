#include <ch32v00x.h>
#include "time_measurement.h"

void time_begin() {
    SysTick->CNT = 0;
    SysTick->SR = 0;
    SysTick->CTLR |= 5;
}  

time_period_t millis() {
    return SysTick->CNT / (SystemCoreClock / 1000);
}

void delay(time_period_t ms) {
    time_period_t start = millis();
    while (millis() - start <= ms)
        asm ("nop");      
}    
