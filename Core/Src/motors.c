/*
 * motors.c
 *
 *  Created on: Nov 9, 2026
 *      Author: jamesg13
 */
#include "motors.h"
#include "tim.h"

#define LEFT_MIN   50
#define RIGHT_MIN  83

void Motor_Left(int speed) {
	// Forward
	if (speed > 0) {
		if (speed < LEFT_MIN) speed = LEFT_MIN;
		if (speed > 1800) speed = 1800;

		 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, speed);
		 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 0);
	}
	// reverse
	else if (speed < 0) {
		speed = -speed;

		if (speed < LEFT_MIN) speed = LEFT_MIN;
		if (speed > 1800) speed = 1800;

		 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, speed);
		 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
	}

	else {
	 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
	 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 0);
	}

}

void Motor_Right(int speed) {
	// Forward
	if (speed > 0) {
		if (speed < RIGHT_MIN) speed = RIGHT_MIN;
		if (speed > 1800) speed = 1800;

		 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, speed);
		 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 0);
	}
	// Reverse
	else if (speed < 0) {
		speed = -speed;

		if (speed < RIGHT_MIN) speed = RIGHT_MIN;
		if (speed > 1800) speed = 1800;

		 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, speed);
		 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 0);
	}

	else {
	 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 0);
	 __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 0);
	}

}
