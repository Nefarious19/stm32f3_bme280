/*
 * stm32f303_i2c.h
 *
 *  Created on: 4 Sep 2019
 *      Author: rato
 */

#ifndef PERIPHERALS_INC_STM32F303_I2C_H_
#define PERIPHERALS_INC_STM32F303_I2C_H_

void I2C_init(void);
void I2C_writeByte(uint8_t slave, uint8_t data);
void I2C_writeNBytes(uint8_t slave, uint8_t * data, uint8_t len);
uint8_t I2C_readByte(uint8_t slave);
uint8_t * I2C_readNBytes(uint8_t slave, uint8_t * buffer, uint8_t len);

#endif /* PERIPHERALS_INC_STM32F303_I2C_H_ */
