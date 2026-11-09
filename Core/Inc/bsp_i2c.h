/*
 * bsp_i2c.h
 *
 *  Created on: Nov 9, 2026
 *      Author: jamesg13
 */

#ifndef INC_BSP_I2C_H_
#define INC_BSP_I2C_H_

void SCL_high(void);
void SCL_low(void);
void SDA_high(void);
void SDA_low(void);
uint8_t SDA_read(void);


void i2c_start(void);
void i2c_stop(void);

#endif /* INC_BSP_I2C_H_ */
