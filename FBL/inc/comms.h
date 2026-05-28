/*
 * comms.h
 *
 *  Created on: 27-May-2026
 *      Author: Divyang Abotiyawala
 */

#ifndef INC_COMMS_H_
#define INC_COMMS_H_

#include <stdint.h>

#define PACKET_LENGTH_BYTE      (1U)
#define PACKET_DATA_LENGTH      (16U)
#define PACKET_CRC_BYTE         (1U)

#define PACKET_LENGTH           (PACKET_LENGTH_BYTE + PACKET_DATA_LENGTH + PACKET_CRC_BYTE)


#define PACKET_RTEX_DATA0       (0x19U)
#define PACKET_ACK_DATA0        (0x15U)

#define PACKET_BUFFER_LENGTH    (8U)

typedef struct
{
    uint8_t length;
    uint8_t data[PACKET_DATA_LENGTH];
    uint8_t crc;
}comms_packet_st;

void comms_setup(void);
void comms_update(void);
uint8_t comms_packet_available(void);
void comms_write(comms_packet_st *packet);
void comms_read(comms_packet_st *packet);
uint8_t comms_compute_crc(comms_packet_st *packet);
uint8_t comms_is_single_byte_packet(comms_packet_st *packet, uint8_t byte);
void comms_create_single_byte_packet(comms_packet_st *packet, uint8_t byte);

#endif /* INC_COMMS_H_ */
