/*
 * encoders.c
 *
 *  Created on: Mar 24, 2026
 *      Author: jamesg13
 */

#include "encoders.h"

void Encoder_Init_TIM3(void) {
	//zero out register before we start
	TIM3->CNT = 0;
	//Start TIM3 in encoder mode, need both channels for quadrature decoding
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
}

void Encoder_Init_TIM4(void) {
	TIM4->CNT = 0;
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);

}

int Read_Encoder(Motor_ID MYTIMX) {

	int Encoder_TIM;
	switch (MYTIMX) {

		case (MOTOR_ID_ML):
				//reading raw counter, type cast to interpret as signed in case of backwards movement
				Encoder_TIM = (short)TIM3->CNT;
				//Reset value for next reading
				TIM3->CNT = 0;
				break;

		case (MOTOR_ID_MR):
				Encoder_TIM = (short)TIM4->CNT;
				TIM4->CNT = 0;
				break;

		default: Encoder_TIM = 0;

	}

	return Encoder_TIM;
}
