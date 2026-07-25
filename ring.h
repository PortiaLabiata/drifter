#ifndef RING_H
#define RING_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"

#define ring_buffer_size 140
typedef struct {
    uint8_t buffer[ring_buffer_size];
    size_t size;
    size_t head;
    size_t tail;
    
    bitflag_t overrun_flag;
    bitflag_t underrun_flag;
} ring_buffer_s;

void ring_buffer_reset(ring_buffer_s *rb);

bool ring_buffer_push(ring_buffer_s *rb, uint8_t byte);
bool ring_buffer_pop(ring_buffer_s *rb, uint8_t *byte);
size_t ring_buffer_write(ring_buffer_s *rb, uint8_t *bytes, size_t size);
size_t ring_buffer_read(ring_buffer_s *rb, uint8_t *bytes, size_t size);

void ring_buffer_clear_errors(ring_buffer_s *rb);

bool ring_buffer_full(ring_buffer_s *rb);
size_t ring_buffer_available(ring_buffer_s *rb);

#endif
