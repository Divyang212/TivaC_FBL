/*
 * uart.c
 *
 *  Created on: 26-May-2026
 *      Author: Divyang Abotiyawala
 */

#include "cmsis/tm4c123gh6pm.h"
#include "core/bsp.h"
#include "core/uart.h"
#include "core/ring-buffer.h"

#define RING_BUFFER_SIZE    (128U)

static uint8_t uart_buffer[RING_BUFFER_SIZE] = {0U};
static ring_buffer_st rb = {0U};


void UART0_IRQHandler(void)
{

    ring_buffer_write(&rb, (uint8_t)UART0->DR);
    UART0->ICR |= (1 << 4);
    /*
    buffer = UART0->DR;
    data_available = 1U;
    UART0->ICR |= (1 << 4);
    */
}

void uart_setup(void)
{
    ring_buffer_setup(&rb, uart_buffer, RING_BUFFER_SIZE);

    SYSCTL->RCGCGPIO |= (1 << 0); /* enable clock for GPIOA */
    SYSCTL->RCGCUART |= (1 << 0); /* enable clock for UART0 */
    GPIOA->AFSEL |= (0x03U << 0);

    GPIOA->DEN |= (UART_RX | UART_TX);

    UART0->CTL &= (1 << 0);
    UART0->IBRD = 10U;
    UART0->FBRD = 54U;
    UART0->LCRH = (0x03U << 5);
    UART0->CTL |= (1 << 0) | (1 << 8) | (1 << 9);
    UART0->IM |= (1 << 4);
    NVIC->ISER[0] |= (1 << 5);
}

void uart_write(uint8_t *data, const uint32_t length)
{
    for(uint32_t i = 0; i< length; i++)
    {
        uart_write_byte(data[i]);
    }

}

void uart_write_byte(uint8_t data)
{
    UART0->RSR = 0x00;
    while((UART0->FR & (1 << 5)) != 0);
    UART0->DR = data;

}

uint8_t uart_read(uint8_t *data, const uint32_t length)
{
    if(length < 0)
    {
        return 0;
    }
    for(uint8_t byte_read = 0; byte_read < length; byte_read++)
    {
        if(!ring_buffer_read(&rb, &data[byte_read]))
        {
            return byte_read;
        }
    }
    return length;

}

uint8_t uart_read_byte(void)
{
    uint8_t byte = 0;
    (void)uart_read(&byte, 1);
    return byte;
    /*
    data_available = 0;
    return buffer;
    */
}

uint8_t uart_data_available(void)
{
    return !ring_buffer_empty(&rb);
}
