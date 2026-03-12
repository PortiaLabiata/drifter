#ifndef DRIVER_GPIO_H
#define DRIVER_GPIO_H

#include <ch32v00x.h>
#include <stdint.h>

typedef uint8_t pin_t;

enum gpio_mode_e {
    GPIO_MODE_INPUT = 0b00,
    GPIO_MODE_OUTPUT_10MHZ = 0b01,
    GPIO_MODE_OUTPUT_2MHZ = 0x10,
    GPIO_MODE_OUTPUT_30MHZ = 0b11,
    GPIO_MODE_MASK = 0b11,
};

enum gpio_cnf_e {
    GPIO_CNF_ANALOG_PP = 0b00,
    GPIO_CNF_FLOATING_OD = 0b01,
    GPIO_CNF_PUPDOWN_AF_PP = 0b10,
    GPIO_CNF_AF_OD = 0b11,
    GPIO_CNF_MASK = 0b11
};

void gpio_begin();
void gpio_configure(GPIO_TypeDef *port, pin_t pin,
                   enum gpio_mode_e mode, enum gpio_cnf_e cnf);
void gpio_set(GPIO_TypeDef *port, pin_t pin);
void gpio_reset(GPIO_TypeDef *port, pin_t pin);
void gpio_toggle(GPIO_TypeDef *port, pin_t pin);

#endif
