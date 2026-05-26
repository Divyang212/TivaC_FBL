/*
 * uart.h
 *
 *  Created on: 26-May-2026
 *      Author: Divyang Abotiyawala
 */

#ifndef SHARED_INC_CORE_UART_H_
#define SHARED_INC_CORE_UART_H_

#include <stdint.h>

void uart_setup(void);
void uart_write(uint8_t *data, const uint32_t length);
void uart_write_byte(uint8_t data);
void uart_read(uint8_t *data, const uint32_t length);
uint8_t uart_read_byte(void);
uint8_t is_uart_data_available(void);

#endif /* SHARED_INC_CORE_UART_H_ */
