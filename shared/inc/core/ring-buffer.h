/*
 * ring-buffer.h
 *
 *  Created on: 26-May-2026
 *      Author: Divyang Abotiyawala
 */

#ifndef SHARED_INC_CORE_RING_BUFFER_H_
#define SHARED_INC_CORE_RING_BUFFER_H_

#include <stdint.h>

typedef struct
{
    uint8_t *buffer;
    uint8_t read_index;
    uint8_t write_index;
    uint8_t mask;
}ring_buffer_st;

void ring_buffer_setup(ring_buffer_st *rb, uint8_t *buffer, uint8_t size);
uint8_t ring_buffer_empty(ring_buffer_st *rb);
uint8_t ring_buffer_write(ring_buffer_st *rb, uint8_t byte);
uint8_t ring_buffer_read(ring_buffer_st *rb, uint8_t *byte);





#endif /* SHARED_INC_CORE_RING_BUFFER_H_ */
