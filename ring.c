#include "ring.h"

void ring_buffer_reset(ring_buffer_s *rb) {
    rb->size = 0;
    rb->head = 0;
    rb->tail = 0;
}

bool ring_buffer_full(ring_buffer_s *rb) {
    return rb->size >= ring_buffer_size;
}

size_t ring_buffer_available(ring_buffer_s *rb) {
    return rb->size;
}    

bool ring_buffer_push(ring_buffer_s *rb, uint8_t byte) {
    if (ring_buffer_full(rb)) {
        return false;
    }
    rb->buffer[rb->head] = byte;
    rb->head = (rb->head + 1) % ring_buffer_size;
    rb->size++;
    return true;
}

bool ring_buffer_pop(ring_buffer_s *rb, uint8_t *byte) {
    if (!ring_buffer_available(rb)) {
        return false;
    }
    *byte = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % ring_buffer_size;
    rb->size--;
    return true;
}    

size_t ring_buffer_write(ring_buffer_s *rb, uint8_t *bytes, size_t size) {
    size_t i = 0;
    for (; i < size; i++) {
        if (!ring_buffer_push(rb, bytes[i])) {
            rb->overrun_flag = 1;
            break;
        }
    }
    return i;
}

size_t ring_buffer_read(ring_buffer_s *rb, uint8_t *bytes, size_t size) {
    size_t i = 0;
    for (; i < size; i++) {
        if (!ring_buffer_pop(rb, bytes + i)) {
            rb->underrun_flag = 1;
            break;
        }            
    }        
    return i;
}

void ring_buffer_clear_errors(ring_buffer_s *rb) {
    rb->overrun_flag = 0;
    rb->underrun_flag = 0;
}    
