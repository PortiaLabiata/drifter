#include "hardware_config.h"
#include "time_measurement.h"
#include "driver_gpio.h"
#include "driver_tim.h"
#include "driver_usart.h"

void configure_system(void) {
    time_begin();
    gpio_begin();
}

void configure_usart(void) {
    gpio_configure(GPIOD, 5,
                   GPIO_MODE_OUTPUT_10MHZ, GPIO_CNF_AF_PP);
    gpio_configure(GPIOD, 6,
                   GPIO_MODE_INPUT, GPIO_CNF_FLOATING);
    usart_begin();
    
    usart_config_s usart_cfg = {
        .speed = 9600,
        .word_length = USART_WORD_LENGTH_8BIT,
        .parity = USART_PARITY_DISABLED,
    };
    usart_configure(&usart_cfg);
}

void configure_pwm(void) {
    gpio_configure(GPIOD, 0,
                   GPIO_MODE_OUTPUT_10MHZ, GPIO_CNF_AF_PP);
    gpio_configure(GPIOD, 2,
                   GPIO_MODE_OUTPUT_10MHZ, GPIO_CNF_AF_PP);
    gpio_configure(GPIOA, 1,
                   GPIO_MODE_OUTPUT_10MHZ, GPIO_CNF_AF_PP);
    gpio_configure(GPIOA, 2,
                   GPIO_MODE_OUTPUT_10MHZ, GPIO_CNF_AF_PP);
    tim_begin();

    tim_config_s tim_cfg = {
        .period = 1000,
        .psc = 0,
        .channels = {
            {true, true, 500},
            {false, false, 0},
            {true, true, 500},
            {false, false, 0},
        },
    };
    tim_config(&tim_cfg);   
}
