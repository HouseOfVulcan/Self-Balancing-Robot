/*
 * motors.h
 *
 *  Created on: Mar 24, 2026
 *      Author: jamesg13
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "main.h"
#include "tim.h"

void Motor_Init(void);
void Motor_Set(int left, int right);

#define PWM_MAX 5000
#define ARR_MAX 7199
#define L_PWMA TIM8->CCR1
#define L_PWMB TIM8->CCR2
#define R_PWMA TIM8->CCR3
#define R_PWMB TIM8->CCR4


#endif /* INC_MOTORS_H_ */
