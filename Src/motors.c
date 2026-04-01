/*
 * motors.c
 *
 *  Created on: Mar 24, 2026
 *      Author: jamesg13
 */
#include "motors.h"

void Motor_Init(void)
{
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}

void Motor_Set(int left, int right) {

	// CLampming PWM
	if (left > PWM_MAX) left = PWM_MAX;
	if (left < -PWM_MAX) left = -PWM_MAX;
	if (right > PWM_MAX) right = PWM_MAX;
	if (right < -PWM_MAX) right = -PWM_MAX;

	// Left motor
	if (left > 0) {
	    L_PWMA = left;
	    L_PWMB = 0;
	} else {
	    L_PWMA = 0;
	    L_PWMB = -left;  // or abs(left), same thing for negatives
	}

	// Right motor (reversed due to physical mounting)
	if (right > 0) {
	    R_PWMA = 0;
	    R_PWMB = right;
	} else {
	    R_PWMA = -right;
	    R_PWMB = 0;
	}

}
