#include "common.h"
#include "configuration.h"
#include "driver_gpio.h"

static inline uint32_t _mode_to_mask(pin_t pin, enum gpio_mode_e mode,
                              enum gpio_cnf_e cnf) {
    return ((uint32_t)mode | ((uint32_t)cnf << 2)) << (4 * pin);
}

static void _assert_ioline(GPIO_TypeDef *port, pin_t pin) {
    assert(pin <= 7);
    assert((port == GPIOA) || (port == GPIOC) || (port == GPIOD));
}

void gpio_begin() {
#if IOPA_EN
    RCC->APB2PCENR |= RCC_IOPAEN;
#endif
#if IOPC_EN
    RCC->APB2PCENR |= RCC_IOPCEN;
#endif
#if IOPD_EN
    RCC->APB2PCENR |= RCC_IOPDEN;
#endif    
}    

void gpio_configure(GPIO_TypeDef *port, pin_t pin, enum gpio_mode_e mode,
                   enum gpio_cnf_e cnf) {
    _assert_ioline(port, pin);
    volatile uint32_t *cfglr = &port->CFGLR;
    *cfglr &= ~_mode_to_mask(pin, GPIO_MODE_MASK, GPIO_CNF_MASK);
    *cfglr |= _mode_to_mask(pin, mode, cnf);
}

void gpio_set(GPIO_TypeDef *port, pin_t pin) {
    _assert_ioline(port, pin);
    port->BSHR |= (1 << pin);
}

void gpio_reset(GPIO_TypeDef *port, pin_t pin) {
    _assert_ioline(port, pin);
    port->BSHR |= (1 << (pin + 16));
}

void gpio_toggle(GPIO_TypeDef *port, pin_t pin) {
    _assert_ioline(port, pin);
    port->OUTDR ^= (1 << pin);
}    
