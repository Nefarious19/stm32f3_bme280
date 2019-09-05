/*
 * i2c.c
 *
 *  Created on: 10.01.2019
 *      Author: rafal
 */

#include "stm32f3xx.h"
#include "../inc/stm32f303_i2c.h"

I2C_deviceFoundCallbackFunctionPtr I2C_addressFoundCallback;

void I2C_init(void)
{
	RCC->CFGR3 |= RCC_CFGR3_I2C1SW;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	GPIOB->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1;
	GPIOB->OTYPER |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7;
	GPIOB->AFR[0] |= (4<<24) | (4<<28);

	I2C1->TIMINGR = (3<<28) |
					(16<<0)  |
					(16<<8)  |
					(2<<20) 	|
					(2<<16) 	;
	I2C1->CR1 |= I2C_CR1_NOSTRETCH;
}

void I2C_registerDeviceFoundCallback(I2C_deviceFoundCallbackFunctionPtr funcPtr)
{
	if(funcPtr)
	{
		I2C_addressFoundCallback = funcPtr;
	}
}

void I2C_Scanner(void)
{
	uint8_t deviceFound;

	for(uint8_t slaveIdx = 0; slaveIdx < 0x7F; slaveIdx++)
	{
		deviceFound = 0;
		I2C1->CR1 |= I2C_CR1_PE;
		I2C1->CR2 = (I2C1->CR2 & 0xFFFFFF00) | (slaveIdx<<1);
		I2C1->CR2 = (I2C1->CR2 & 0xFF00FFFF) | (1<<16);
		I2C1->CR2 &= ~I2C_CR2_RD_WRN;
		I2C1->CR2 |= I2C_CR2_START;
		while(I2C1->CR2 & I2C_CR2_START);
		while((!(I2C1->ISR & I2C_ISR_TXIS)) && (!(I2C1->ISR & I2C_ISR_NACKF)));
		if((I2C1->ISR & I2C_ISR_TXIS))
		{
			deviceFound = 1;
			I2C1->TXDR = 0;
			while(!(I2C1->ISR & I2C_ISR_TXE));
			I2C1->CR2 |= I2C_CR2_STOP;
			while(I2C1->CR2 & I2C_CR2_STOP);
			I2C1->CR1 &= ~I2C_CR1_PE;
		}
		else if(!(I2C1->ISR & I2C_ISR_NACKF))
		{
			I2C1->CR1 &= ~I2C_CR1_PE;
		}

		if(deviceFound)
		{
			if(I2C_addressFoundCallback)
			{
				I2C_addressFoundCallback(slaveIdx<<1);
			}
		}
	}
}

void I2C_writeByte(uint8_t slave, uint8_t data)
{
	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR2 = (I2C1->CR2 & 0xFFFFFF00) | (slave<<1);
	I2C1->CR2 = (I2C1->CR2 & 0xFF00FFFF) | (1<<16);
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;
	while(I2C1->CR2 & I2C_CR2_START);
	I2C1->TXDR = data;
	while(!(I2C1->ISR & I2C_ISR_TXE));
	I2C1->CR2 |= I2C_CR2_STOP;
	while(I2C1->CR2 & I2C_CR2_STOP);
	I2C1->CR1 &= ~I2C_CR1_PE;
}

void I2C_writeNBytes(uint8_t slave, uint8_t * data, uint8_t len)
{
	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR2 = (I2C1->CR2 & 0xFFFFFF00) | (slave<<1);
	I2C1->CR2 = (I2C1->CR2 & 0xFF00FFFF) | (len<<16);
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;
	while(I2C1->CR2 & I2C_CR2_START);
	while(len--)
	{
		while(!(I2C1->ISR & I2C_ISR_TXE));
		I2C1->TXDR = *data++;
	}

	while(!(I2C1->ISR & I2C_ISR_TXE));
	I2C1->CR2 |= I2C_CR2_STOP;
	while(I2C1->CR2 & I2C_CR2_STOP);
	I2C1->CR1 &= ~I2C_CR1_PE;
}

uint8_t I2C_readByte(uint8_t slave)
{
	uint8_t data = 0;
	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR2 = (I2C1->CR2 & 0xFFFFFF00) | (slave<<1);
	I2C1->CR2 = (I2C1->CR2 & 0xFF00FFFF) | (1<<16);
	I2C1->CR2 |= I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;
	while(I2C1->CR2 & I2C_CR2_START);
	while(!(I2C1->ISR & I2C_ISR_RXNE));
	data = I2C1->RXDR;
	I2C1->CR2 |= I2C_CR2_STOP;
	while(I2C1->CR2 & I2C_CR2_STOP);
	I2C1->CR1 &= ~I2C_CR1_PE;
	return data;
}

uint8_t * I2C_readNBytes(uint8_t slave, uint8_t * buffer, uint8_t len)
{
	uint8_t * data = buffer;
	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR2 = (I2C1->CR2 & 0xFFFFFF00) | (slave<<1);
	I2C1->CR2 = (I2C1->CR2 & 0xFF00FFFF) | (len<<16);
	I2C1->CR2 |= I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;

	while(len--)
	{
		while(!(I2C1->ISR & I2C_ISR_RXNE));
		*data++ = I2C1->RXDR;
	}
	I2C1->CR2 |= I2C_CR2_STOP;
	while(I2C1->CR2 & I2C_CR2_STOP);
	I2C1->CR1 &= ~I2C_CR1_PE;
	return buffer;
}


uint8_t * I2C_readNBytesFromAddress(uint8_t slave, uint8_t registerAddress, uint8_t * buffer, uint8_t len)
{
	uint8_t * data = buffer;

	I2C1->CR1 |= I2C_CR1_PE;
	I2C1->CR2 = (I2C1->CR2 & 0xFFFFFF00) | (slave<<1);
	I2C1->CR2 = (I2C1->CR2 & 0xFF00FFFF) | (1<<16);
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;
	while(I2C1->CR2 & I2C_CR2_START);
	while(!(I2C1->ISR & I2C_ISR_TXIS));
	I2C1->TXDR = registerAddress;
	while(!(I2C1->ISR & I2C_ISR_TC));
	I2C1->CR2 |= I2C_CR2_STOP;
	while(I2C1->CR2 & I2C_CR2_STOP);
	I2C1->CR2 = (I2C1->CR2 & 0xFF00FFFF) | (len<<16);
	I2C1->CR2 |= I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;
	while(I2C1->CR2 & I2C_CR2_START);
	while(len--)
	{
		while(!(I2C1->ISR & I2C_ISR_RXNE));
		*data++ = I2C1->RXDR;
	}
	I2C1->CR2 |= I2C_CR2_STOP;
	while(I2C1->CR2 & I2C_CR2_STOP);
	I2C1->CR1 &= ~I2C_CR1_PE;
	return buffer;
}
