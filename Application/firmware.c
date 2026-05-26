

/**
 * main.c
 */

#include "cmsis/tm4c123gh6pm.h"
#include "core/bsp.h"
#include "core/uart.h"

int main(void)
{
    SCB->VTOR = 0x8000; /* vector offset */
    BSP_init();
    uart_setup();

    uint64_t start_time = get_tick();
    while(1)
    {
        if(get_tick() - start_time >= 1000U)
        {
            GPIOF->DATA_Bits[LED_BLUE] ^= LED_BLUE;
            start_time = get_tick();
        }

        if(is_uart_data_available())
        {
            uint8_t data = uart_read_byte();
            uart_write_byte(data + 1);
        }

        system_delay_ms(1000U);
    }

	return 0;
}
