

/**
 * main.c
 */

#include "cmsis/tm4c123gh6pm.h"
#include "core/bsp.h"

int main(void)
{
    SCB->VTOR = 0x8000; /* vector offset */
    BSP_init();

    uint64_t start_time = get_tick();
    while(1)
    {
        if(get_tick() - start_time >= 1000U)
        {
            GPIOF->DATA_Bits[LED_BLUE] ^= LED_BLUE;
            start_time = get_tick();
        }
    }

	return 0;
}
