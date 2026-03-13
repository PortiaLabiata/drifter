#include <string.h>
#include "driver_usart.h"

USART_TypeDef *usart = USART1;
usart_errors_s _errors;

void usart_begin() {
    RCC->APB2PCENR |= RCC_USART1EN;
    memset(&_errors, 0, sizeof(_errors));
}
void usart_configure(usart_config_s *config) {
    assert(config->speed <= 115200); // Assert for reasonable speeds

    SystemCoreClockUpdate();
    USART1->BRR = (SystemCoreClock + (config->speed/2)) / config->speed;
    usart_ctlr1_u *cr = (usart_ctlr1_u*)&USART1->CTLR1;
    if (config->word_length == USART_WORD_LENGTH_9BIT) {
        cr->flags.m = 1;
    } else {
        cr->flags.m = 0;
    }

    if (config->parity != USART_PARITY_DISABLED) {
        cr->flags.pce = 1;
        if (config->parity == USART_PARITY_EVEN) {
            cr->flags.ps = 0;
        } else {
            cr->flags.ps = 1;
        }
    } else {
        cr->flags.pce = 0;
    }

    cr->flags.txeie = 1;
    cr->flags.rxneie = 1;

    cr->flags.ue = 1;
    cr->flags.te = 1;
    cr->flags.re = 1;

    NVIC_SetPriority(USART1_IRQn, 0);
    NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_IRQHandler() {
    usart_statr_u *sr = (usart_statr_u*)&USART1->STATR;
    if (sr->flags.txe) {
        // Continue transmission
    }
    if (sr->flags.rxne) {
        // Call reception hook
    }
    memcpy(&_errors, &sr->register_value, sizeof(_errors));
}

void usart_write_blocking(uint8_t *bytes, size_t size) {
    usart_statr_u *sr = (usart_statr_u*)&USART1->STATR;
    for (size_t i = 0; i < size; i++) {
        while (!sr->flags.txe)
            ;
        USART1->DATAR = bytes[i];
    }
    while (!sr->flags.tc)
        ;
}

void usart_read_blocking(uint8_t *bytes, size_t size) {
    usart_statr_u *sr = (usart_statr_u *)&USART1->STATR;
    for (size_t i = 0; i < size; i++) {
        while (!sr->flags.rxne)
            ;
        bytes[i] = USART1->DATAR;
    }        
}    

const usart_errors_s *usart_errors() { return &_errors; }
