/*
 * utils.c
 *
 *  Created on: 4 Sep 2019
 *      Author: rato
 */

#include <stm32f3xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Drivers/PERIPHERALS/inc/stm32f303_uart.h"

extern PERIPH_USART_Instance_t usart1;

int _write(int file, char *data, int len)
{
	while(len--)
	{
		PERIPH_USART_putc(&usart1, *data++);
	}

	return 0;
}

void GPIO_init()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOE->MODER |= 0x55550000;
	GPIOE->OSPEEDR |= 0xFFFF0000;
	GPIOE->ODR |= 0xFF00;

	//USART1 gpio configuration
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1;
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR4;
	GPIOC->AFR[0] &= ~GPIO_AFRL_AFRL7;
	GPIOC->AFR[0] |= (7<<16) | (7<<20);


}

void RCC_Init(void)
{
	RCC->CR &= ~RCC_CR_HSEON;
	RCC->CR |= RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	RCC->CFGR |= RCC_CFGR_PLLMUL9;
	RCC->CFGR2 &= ~RCC_CFGR2_PREDIV;
	RCC->CFGR |= (1<<16);
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));
	FLASH->ACR |= FLASH_ACR_LATENCY_2;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
}
