

/**
 * main.c
 */
#include <stdint.h>
#include "cmsis/tm4c123gh6pm.h"
#include "core/bsp.h"


#define FLASH_BASE              (0x00000000U)
#define BOOTLOADER_SIZE         (0x8000U)       /* 32K */
#define MAIN_APP_START_ADDRESS  (FLASH_BASE + BOOTLOADER_SIZE)

typedef void (*fptr) (void);

void jump_to_app(void)
{
    uint32_t *reset_vector_entry = (((uint32_t *)MAIN_APP_START_ADDRESS) + 1U);
    uint32_t *reset_handler = (uint32_t *)*reset_vector_entry;
    fptr jump_fun = (fptr)reset_handler;

    jump_fun();
}

int main(void)
{
    jump_to_app();

    /* should never return */
	return 0;
}
