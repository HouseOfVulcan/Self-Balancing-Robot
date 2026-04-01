#ifndef __BSP_OLED_I2C_H_
#define __BSP_OLED_I2C_H_

//#include "AllHeader.h"
#include <stdint.h>

void OLED_I2C_Init(void);



//IICËùÓÐ²Ù×÷º¯Êý IIC all operation functions
void OLED_IIC_Init(void);                  //³õÊ¼»¯IICµÄIO¿Ú Initialize the IIC IO port
int OLED_IIC_Start(void);                  //·¢ËÍIIC¿ªÊ¼ÐÅºÅ Send IIC start signal
void OLED_IIC_Stop(void);                  //·¢ËÍIICÍ£Ö¹ÐÅºÅ Send IIC stop signal
void OLED_IIC_Send_Byte(uint8_t txd);           //IIC·¢ËÍÒ»¸ö×Ö½Ú IIC sends a byte
uint8_t OLED_IIC_Read_Byte(unsigned char ack);  //IIC¶ÁÈ¡Ò»¸ö×Ö½Ú IIC reads a byte
int OLED_IIC_Wait_Ack(void);               //IICµÈ´ýACKÐÅºÅ IIC waits for ACK signal
void OLED_IIC_Ack(void);                   //IIC·¢ËÍACKÐÅºÅ IIC sends ACK signal
void OLED_IIC_NAck(void);                  //IIC²»·¢ËÍACKÐÅºÅ IIC does not send ACK signal

void OLED_IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t OLED_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);
unsigned char OLED_I2C_Readkey(unsigned char I2C_Addr);

unsigned char OLED_I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char OLED_IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
uint8_t OLED_IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
uint8_t OLED_IICwriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data);
uint8_t OLED_IICwriteBit(uint8_t dev,uint8_t reg,uint8_t bitNum,uint8_t data);
uint8_t OLED_IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);

int OLED_i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int OLED_i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);


#endif

