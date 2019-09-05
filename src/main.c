
#include <stm32f3xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Drivers/PERIPHERALS/inc/stm32f303_uart.h"
#include "../Drivers/PERIPHERALS/inc/stm32f303_i2c.h"
#include "../BME280/bme280_defs.h"

#include "utils.h"

volatile uint32_t timer = 0;

void SysTick_Handler (void)
{
	if(timer) timer --;
}

PERIPH_USART_Instance_t usart1 = {0};

void USART1_RX_Callback (void)
{
	char * ptr = 0;
	ptr = malloc(PERIPH_USART_CIRCULLAR_BUFFER_RX_SIZE);
	memset(ptr, 0, PERIPH_USART_CIRCULLAR_BUFFER_RX_SIZE);
	PERIPH_USART_GetString(&usart1,ptr);
	PERIPH_USART_puts(&usart1, ptr);
	free(ptr);
}

void i2c_cllback (uint8_t address)
{
	printf("Device found @ addr: 0x%x\r\n", address);
}

uint8_t buffer[0x5E] = { 0 };

int main(void)
{

  RCC_Init();
  GPIO_init();
  SysTick_Config(72000000UL / 1000);
  PERIPH_USART_Init(USART1, &usart1, 115200);
  PERIPH_USART_RegisterRxCallback(&usart1,USART1_RX_Callback);

  I2C_init();
  I2C_registerDeviceFoundCallback(i2c_cllback);

  I2C_readNBytesFromAddress(BME280_DEVICE_I2C_ADDRESS,0xD0,buffer,1);

  while (1)
  {
	  PERIPH_USART_RXService();

	  if(!timer)
	  {
		  static uint32_t shift = 0x0100;
		  timer = 100;
		  GPIOE->ODR &= 0x00FF;
		  GPIOE->ODR |= shift;
		  shift <<= 1;
		  if(shift == 0x10000) shift = 0x0100;

	  }
  }
}



