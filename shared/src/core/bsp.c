/*
 * bsp.c
 *
 *  Created on: 24-May-2026
 *      Author: Divyang Abotiyawala
 */

#include <stdint.h>
#include "cmsis/tm4c123gh6pm.h"
#include "core/bsp.h"

__attribute__((naked)) void assert_failed (char const *file, int line) {
    /* TBD: damage control */
    NVIC_SystemReset(); /* reset the system */
}

volatile uint64_t ticks;

void SysTick_Handler(void)
{
    ++ticks;

}

uint64_t get_tick(void)
{
    return ticks;
}

void systemClock_Config(void)
{
    SYSCTL->RCC &= ~(1 << 4); /* select main OSC as a source */
    SYSCTL->RCC &= ~0x7C0;
    SYSCTL->RCC |= (0x15 << 6); /* XTAL = 16MHz */
    SYSCTL->RCC &= ~(1 << 13); /* PLL is operating normally */

    SYSCTL->RCC &= ~(0xF << 23); /* clear SYSDIV field */
    SYSCTL->RCC |= (0x09 << 23); /* 20MHz frequency */
    SYSCTL->RCC |= (1 << 22); /* Enabled system clock divider*/

    while(!((SYSCTL->RIS) & (1 << 6))); /* wait */

    SYSCTL->RCC &= ~(1 << 11); /* Enabled use of the PLL */

}

void systic_Configure(void)
{
    SysTick->CTRL |= (1 << 2) | (1 << 1) | (1 << 0); /* configure systic timer */
    SysTick->LOAD |= (20000U - 1); /* systic handler trigger every 1ms */
}

void BSP_init(void)
{
    systemClock_Config();
    systic_Configure();

    SYSCTL->RCGCGPIO |= (1 << 5);
    GPIOF->DIR |= LED_RED | LED_GREEN | LED_BLUE;
    GPIOF->DEN |= LED_RED | LED_GREEN | LED_BLUE;

}

void system_delay_ms(uint32_t tmp_ms)
{
    while(tmp_ms)
    {
        while((SysTick->CTRL & (1 << 16)) == 0); /* wait till counter reach to zero */
        --tmp_ms;
    }
}
