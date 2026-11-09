/*
 * delay.c
 *
 *  Created on: Nov 9, 2026
 *      Author: jamesg13
 */

#include "delay.h"
#include "main.h"

#define CYCLES_PER_US 72


void delay_init() {

	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	DWT->CYCCNT = 0;

}


void delay_us(uint32_t us) {
	uint32_t start = DWT->CYCCNT;

	// CYCCNT ticks 72 per 1 us
	uint32_t ticks = us * CYCLES_PER_US;

	while ((DWT->CYCCNT - start) < ticks) {
		}


}
