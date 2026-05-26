/*
 * uart.c
 *
 *  Created on: 26-May-2026
 *      Author: Divyang Abotiyawala
 */

#include "cmsis/tm4c123gh6pm.h"
#include "core/bsp.h"
#include "core/uart.h"

static uint8_t buffer;
uint8_t data_available = 0U;

void UART0_IRQHandler(void)
{
    buffer = UART0->DR;
    data_available = 1U;
    UART0->ICR |= (1 << 4);
}

void uart_setup(void)
{
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

void uart_read(uint8_t *data, const uint32_t length)
{
    for(uint32_t i = 0; i < length; i++)
    {
        data[i] = uart_read_byte();
    }

}

uint8_t uart_read_byte(void)
{
    data_available = 0;
    return buffer;

}

uint8_t is_uart_data_available(void)
{
    return data_available;
}
