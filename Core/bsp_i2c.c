/*
 * bsp_i2c.c
 *
 *  Created on: Nov 9, 2026
 *      Author: jamesg13
 */


// SDA = PB9
// SCL = PB8

#include "main.h"
#include "delay.h"

#define I2C_DELAY_US 5
#define SCL_PIN GPIO_PIN_8
#define SDA_PIN GPIO_PIN_9

void SCL_high(void) {
	HAL_GPIO_WritePin(GPIOB, SCL_PIN, GPIO_PIN_SET);
}

void SCL_low(void) {
	HAL_GPIO_WritePin(GPIOB, SCL_PIN, GPIO_PIN_RESET);
}

void SDA_high(void) {
	HAL_GPIO_WritePin(GPIOB, SDA_PIN, GPIO_PIN_SET);
}

void SDA_low(void) {
	HAL_GPIO_WritePin(GPIOB, SDA_PIN, GPIO_PIN_RESET);
}

uint8_t SDA_read(void) {
	uint8_t val = HAL_GPIO_ReadPin(GPIOB, SDA_PIN);
	return val;
}


// START = SDA high -> low while SCL is high
void i2c_start(void) {
    SDA_high();    		//SDA needs to change first
    SCL_high();
    delay_us(I2C_DELAY_US);
    SDA_low();     		// START
    delay_us(I2C_DELAY_US);
    SCL_low();
}

// STOP = SDA low -> high while SCL is high
void i2c_stop(void) {
    SCL_low();     // Bring SCL low so we can change SDA safely
    SDA_low();
    delay_us(I2C_DELAY_US);
    SCL_high();
    delay_us(I2C_DELAY_US);
    SDA_high();    // STOP
    delay_us(I2C_DELAY_US);
}


uint8_t i2c_send_byte(uint8_t byte) {
	uint8_t mask = 0x80;

	SCL_low();

	for (int i = 0; i < 8; i++) {
		if (byte & mask) {
			SDA_high();
			delay_us(I2C_DELAY_US);

		} else {
			SDA_low();
			delay_us(I2C_DELAY_US);
		}
		// move the byte
		SCL_high();
		delay_us(I2C_DELAY_US);
		SCL_low();

		byte <<= 1;
	}

	// read ACK
	SDA_high();
	delay_us(I2C_DELAY_US);
	SCL_high();
	delay_us(I2C_DELAY_US);
	uint8_t ack = SDA_read();
	SCL_low();

	return ack;
}
