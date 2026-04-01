/*
 * encoders.h
 *
 *  Created on: Mar 24, 2026
 *      Author: jamesg13
 */

#ifndef INC_ENCODERS_H_
#define INC_ENCODERS_H_

#include "main.h"
#include "tim.h"
#include "myenum.h"

void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(Motor_ID MYTIMX);


#endif /* INC_ENCODERS_H_ */
