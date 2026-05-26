/*
 * ring-buffer.c
 *
 *  Created on: 26-May-2026
 *      Author: Divyang Abotiyawala
 */


#include "core/ring-buffer.h"



void ring_buffer_setup(ring_buffer_st *rb, uint8_t *buffer, uint8_t size)
{
    rb->buffer = buffer;
    rb->read_index = 0;
    rb->write_index = 0;
    rb->mask = size - 1;
}

uint8_t ring_buffer_empty(ring_buffer_st *rb)
{
    return (rb->read_index == rb->write_index);

}

uint8_t ring_buffer_write(ring_buffer_st *rb, uint8_t byte)
{
    uint8_t local_read_idx = rb->read_index;
    uint8_t local_write_idx = rb->write_index;

    uint8_t next_write_index = (local_write_idx + 1) & rb->mask;

    if(next_write_index == local_read_idx)
    {
        return 0;
    }

    rb->buffer[local_write_idx] = byte;
    rb->write_index = next_write_index;

    return 1;

}

uint8_t ring_buffer_read(ring_buffer_st *rb, uint8_t *byte)
{
    uint8_t local_read_idx = rb->read_index;
    uint8_t local_write_idx = rb->write_index;

    if(local_read_idx == local_write_idx)
    {
        return 0;
    }
    *byte = rb->buffer[local_read_idx];
    local_read_idx = (local_read_idx + 1) & rb->mask;
    rb->read_index = local_read_idx;

    return 1;
}


