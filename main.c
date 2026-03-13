#include "time_measurement.h"
#include "driver_usart.h"
#include "driver_gpio.h"

int main() {
    time_begin();
    gpio_begin();
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

    while (1) {
        usart_write_blocking((uint8_t*)"Test\r\n", 6);
        delay(1000);
    }    
}  
