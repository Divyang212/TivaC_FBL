/*
 * comms.c
 *
 *  Created on: 27-May-2026
 *      Author: Divyang Abotiyawala
 */

#include <string.h>
#include "comms.h"
#include "core/uart.h"
#include "core/crc8.h"

typedef enum
{
    CommsState_Length,
    CommsState_Data,
    CommsState_CRC
}comms_state_en;

static comms_state_en state = CommsState_Length;
static uint8_t data_byte_count = 0;
static comms_packet_st temp_packet = {.length = 0, .data = {0}, .crc = 0};
static comms_packet_st retx_packet, ack_packet = {.length = 0, .data = {0}, .crc = 0};
static comms_packet_st last_transmitted_packet = {.length = 0, .data = {0}, .crc = 0};

static comms_packet_st packet_buffer[PACKET_BUFFER_LENGTH];
static uint8_t packet_read_index = 0;
static uint8_t packet_write_index = 0;
static uint8_t packet_mask = PACKET_BUFFER_LENGTH - 1;

void comms_setup(void)
{
    comms_create_single_byte_packet(&retx_packet, PACKET_RTEX_DATA0);
    comms_create_single_byte_packet(&ack_packet, PACKET_ACK_DATA0);
}

void comms_update(void)
{
    while(uart_data_available())
    {
        switch(state)
        {
        case CommsState_Length : {
            temp_packet.length = uart_read_byte();
            state = CommsState_Data;
        }break;

        case CommsState_Data : {
            temp_packet.data[data_byte_count ++] = uart_read_byte();
            if(data_byte_count >= PACKET_DATA_LENGTH)
            {
                data_byte_count = 0;
                state = CommsState_CRC;
            }
        }break;

        case CommsState_CRC : {
            temp_packet.crc = uart_read_byte();
            if(temp_packet.crc != comms_compute_crc(&temp_packet))
            {
                comms_write(&retx_packet);
                state = CommsState_Length;
                break;
            }
            if(comms_is_single_byte_packet(&temp_packet, PACKET_RTEX_DATA0))
            {
                comms_write(&last_transmitted_packet);
                state = CommsState_Length;
                break;
            }
            if(comms_is_single_byte_packet(&temp_packet, PACKET_ACK_DATA0))
            {
                state = CommsState_Length;
                break;
            }

            uint8_t next_packet_write_index = (packet_write_index + 1) & packet_mask;
            memcpy(&packet_buffer[packet_write_index], &temp_packet, sizeof(comms_packet_st));
            packet_write_index = next_packet_write_index;
            comms_write(&ack_packet);
            state = CommsState_Length;
        }break;

        default : {
            state = CommsState_Length;
        }

        }
    }

}

uint8_t comms_packet_available(void)
{
    return packet_write_index |= packet_read_index;
}

void comms_write(comms_packet_st *packet)
{
    uart_write((uint8_t *)packet, PACKET_LENGTH);
    memcpy(&last_transmitted_packet, packet, sizeof(comms_packet_st));

}

void comms_read(comms_packet_st *packet)
{
    memcpy(packet, &packet_buffer[packet_read_index], sizeof(comms_packet_st));
    packet_read_index = (packet_read_index + 1) & packet_mask;

}

uint8_t comms_compute_crc(comms_packet_st *packet)
{
    return crc8((uint8_t *)packet, (PACKET_LENGTH - PACKET_CRC_BYTE));

}

uint8_t comms_is_single_byte_packet(comms_packet_st *packet, uint8_t byte)
{
    if(packet->length != 1)
    {
        return 0;
    }
    if(packet->data[0] != byte)
    {
        return 0;
    }
    for(uint8_t i = 1; i < PACKET_DATA_LENGTH; i++)
    {
        if(packet->data[i] != 0xff)
        {
            return 0;
        }
    }
    return 1;
}



void comms_create_single_byte_packet(comms_packet_st *packet, uint8_t byte)
{
    memset(packet, 0xff, sizeof(comms_packet_st));
    packet->length = 1;
    packet->data[0] = byte;
    packet->crc = comms_compute_crc(packet);
}
