#ifndef DRIVER_USART_H
#define DRIVER_USART_H
#include <stdint.h>
#include <stdlib.h>

#include <ch32v00x.h>
#include <core_riscv.h>
#include <system_ch32v00x.h>
#include "common.h"

typedef uint32_t speed_t;

typedef struct PACKED ALIGNED {
    uint32_t reserved : 28;
    bitflag_t overload : 1;
    bitflag_t noise : 1;
    bitflag_t framing : 1;
    bitflag_t parity : 1;
} usart_errors_s;

typedef union {
    struct PACKED ALIGNED {
        BITFIELD(sbk);
        BITFIELD(rwu);
        BITFIELD(re);
        BITFIELD(te);
        BITFIELD(idleie);
        BITFIELD(rxneie);
        BITFIELD(tcie);
        BITFIELD(txeie);
        BITFIELD(peie);
        BITFIELD(ps);
        BITFIELD(pce);
        BITFIELD(wake);
        BITFIELD(m);
        BITFIELD(ue);
        uint8_t reserved1 : 2;
        uint16_t resered2;
    } flags;
    register_value_t register_value;
} usart_ctlr1_u;

typedef union {
    struct PACKED ALIGNED {
        BITFIELD(pe);
        BITFIELD(fe);
        BITFIELD(ne);
        BITFIELD(ore);
        BITFIELD(idle);
        BITFIELD(rxne);
        BITFIELD(tc);
        BITFIELD(txe);
        BITFIELD(lbd);
        BITFIELD(cts);
        uint8_t reserved1 : 6;
        uint16_t resered2;
    } flags;        
    register_value_t register_value;
} usart_statr_u;    

enum usart_parity_e {
    USART_PARITY_DISABLED,
    USART_PARITY_EVEN,
    USART_PARITY_ODD,
};

enum usart_word_length_e {
    USART_WORD_LENGTH_8BIT,
    USART_WORD_LENGTH_9BIT,
};  

typedef struct {
    speed_t speed;
    enum usart_word_length_e word_length;
    enum usart_parity_e parity;
} usart_config_s;    

void usart_begin();
void usart_configure(usart_config_s *config);

void usart_write_blocking(uint8_t *bytes, size_t size);
void usart_read_blocking(uint8_t *bytes, size_t size);

size_t usart_write(uint8_t *bytes, size_t size);
bool usart_read(uint8_t *byte);

const usart_errors_s *usart_errors();

#endif
