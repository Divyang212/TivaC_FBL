/*
 * bsp.h
 *
 *  Created on: 24-May-2026
 *      Author: Divyang Abotiyawala
 */

#ifndef SHARED_INC_CORE_BSP_H_
#define SHARED_INC_CORE_BSP_H_

/** Board support package for tm4c123gh6pm **/

/* on board LED */
#define LED_RED             (1U << 1)
#define LED_BLUE            (1U << 2)
#define LED_GREEN           (1U << 3)

void systemClock_Config(void);
void BSP_init(void);
uint64_t get_tick(void);
void system_delay_ms(uint32_t);

#endif /* SHARED_INC_CORE_BSP_H_ */


