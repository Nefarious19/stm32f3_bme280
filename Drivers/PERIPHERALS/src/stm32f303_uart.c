/*
 * stm32f303_uart.c
 *
 *  Created on: 01.09.2019
 *      Author: rafal
 */


#include <stm32f3xx.h>
#include "../inc/stm32f303_uart.h"

void USART1_IRQHandler (void);
void USART2_IRQHandler (void);
void USART3_IRQHandler (void);

#define PERIPH_USART_CIRCULLAR_BUFFER_TX_MASK (PERIPH_USART_CIRCULLAR_BUFFER_TX_SIZE - 1)
#define PERIPH_USART_CIRCULLAR_BUFFER_RX_MASK (PERIPH_USART_CIRCULLAR_BUFFER_RX_SIZE - 1)

PERIPH_USART_Instance_t * usart1ObjectPtr = 0;
PERIPH_USART_Instance_t * usart2ObjectPtr = 0;
PERIPH_USART_Instance_t * usart3ObjectPtr = 0;

void PERIPH_USART_Init(USART_TypeDef * usartNo, PERIPH_USART_Instance_t * object, uint32_t baudrate)
{
	SystemCoreClockUpdate();

	if(usartNo == USART1)
	{
		usart1ObjectPtr = object;
		object->instance = usartNo;
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	}
	else if(usartNo == USART2)
	{
		usart2ObjectPtr = object;
		object->instance = usartNo;
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
}
	else if(usartNo == USART3)
	{
		usart3ObjectPtr = object;
		object->instance = usartNo;
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	}
	else
	{
		return;
	}

	uint32_t USARTDIV = 0;
	USARTDIV = (SystemCoreClock * 2) / baudrate;
	object->instance->CR1 |= (USART_CR1_OVER8 | USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE);
	object->instance->BRR = ( USARTDIV & 0x0000FFF0);
	object->instance->BRR |= ((USARTDIV & 0x0000000F)>>1);
	object->instance->CR1 |= USART_CR1_UE;

	if(usartNo == USART1)
	{
		NVIC_ClearPendingIRQ(USART1_IRQn);
		NVIC_EnableIRQ(USART1_IRQn);
	}
	else if(usartNo == USART2)
	{
		NVIC_ClearPendingIRQ(USART2_IRQn);
		NVIC_EnableIRQ(USART2_IRQn);
	}
	else if(usartNo == USART3)
	{
		NVIC_ClearPendingIRQ(USART3_IRQn);
		NVIC_EnableIRQ(USART3_IRQn);
	}

}

char PERIPH_USART_GetChar(PERIPH_USART_Instance_t * object)
{
	char data = 0;

	if(object->usartCircullarBuffertRxHead != object->usartCircullarBuffertRxTail)
	{
		object->usartCircullarBuffertRxTail = (object->usartCircullarBuffertRxTail + 1) & PERIPH_USART_CIRCULLAR_BUFFER_RX_MASK;
		data = object->usartCircullarBuffertRx[object->usartCircullarBuffertRxTail];
	}

	return data;
}

char * PERIPH_USART_GetString(PERIPH_USART_Instance_t * object, char * inOutBuffer)
{
	if(object->usartnoOfReceivedLines)
	{
		char data = 0;
		char * tempBufPtr = inOutBuffer;
		while((data = PERIPH_USART_GetChar(object)))
		{
			*tempBufPtr++ = data;
		}
		object->usartnoOfReceivedLines--;
	}
	else
	{
		*inOutBuffer = 0;
	}

	return inOutBuffer;
}

void PERIPH_USART_RegisterRxCallback(PERIPH_USART_Instance_t * object , void (*rxCallbackPtr)(void))
{
	if(rxCallbackPtr)
	{
		object->newLineReceivedCallbackPtr = rxCallbackPtr;
	}
}

void PERIPH_USART_RXService(void)
{
	if(usart1ObjectPtr && (usart1ObjectPtr->usartnoOfReceivedLines != 0) && (usart1ObjectPtr->newLineReceivedCallbackPtr != 0))
	{
		usart1ObjectPtr->newLineReceivedCallbackPtr();
	}
	if(usart2ObjectPtr && (usart2ObjectPtr->usartnoOfReceivedLines != 0) && (usart2ObjectPtr->newLineReceivedCallbackPtr != 0))
	{
		usart2ObjectPtr->newLineReceivedCallbackPtr();
	}
	if(usart3ObjectPtr && (usart3ObjectPtr->usartnoOfReceivedLines != 0) && (usart3ObjectPtr->newLineReceivedCallbackPtr != 0))
	{
		usart3ObjectPtr->newLineReceivedCallbackPtr();
	}
}

void PERIPH_USART_putc(PERIPH_USART_Instance_t * object, char data)
{
	uint32_t tempHead = (object->usartCircullarBuffertTxHead + 1) & PERIPH_USART_CIRCULLAR_BUFFER_TX_MASK;
	if(tempHead == object->usartCircullarBuffertTxTail) return;
	else
	{
		object->usartCircullarBuffertTx[tempHead] = data;
		object->usartCircullarBuffertTxHead = tempHead;
		object->instance->CR1 |= USART_CR1_TXEIE;
	}
}

void PERIPH_USART_puts(PERIPH_USART_Instance_t * object, char * dataBuffer)
{
	char data = 0;
	while((data = *dataBuffer++))
	{
		PERIPH_USART_putc(object,data);
	}
}

void USART1_IRQHandler (void)
{
	if(usart1ObjectPtr)
	{
		if(usart1ObjectPtr->instance->ISR & USART_ISR_RXNE)
		{
			char receivedChar = usart1ObjectPtr->instance->RDR;
			uint32_t tempHead = (usart1ObjectPtr->usartCircullarBuffertRxHead + 1) & PERIPH_USART_CIRCULLAR_BUFFER_RX_MASK;
			if(tempHead == usart1ObjectPtr->usartCircullarBuffertRxTail )
			{
				return;
			}
			else
			{
				switch(receivedChar)
				{
					case('\r'):
					case('\n'):
					{
						receivedChar = 0;
						usart1ObjectPtr->usartnoOfReceivedLines++;
					}
					default:
						usart1ObjectPtr->usartCircullarBuffertRxHead = tempHead;
						usart1ObjectPtr->usartCircullarBuffertRx[usart1ObjectPtr->usartCircullarBuffertRxHead] = receivedChar;
					break;
				}
			}

		}
		if(usart1ObjectPtr->instance->ISR & USART_ISR_TXE)
		{
			if( usart1ObjectPtr->usartCircullarBuffertTxTail != usart1ObjectPtr->usartCircullarBuffertTxHead )
			{
				usart1ObjectPtr->usartCircullarBuffertTxTail = (usart1ObjectPtr->usartCircullarBuffertTxTail + 1) & PERIPH_USART_CIRCULLAR_BUFFER_TX_MASK;
				usart1ObjectPtr->instance->TDR  = usart1ObjectPtr->usartCircullarBuffertTx[usart1ObjectPtr->usartCircullarBuffertTxTail];
			}
			else
			{
				usart1ObjectPtr->instance->CR1 &= ~USART_CR1_TXEIE;
				usart1ObjectPtr->instance->CR1 |= USART_CR1_TCIE;
			}
		}
		if(usart1ObjectPtr->instance->ISR & USART_ISR_TC)
		{
			usart1ObjectPtr->instance->CR1 &= ~USART_CR1_TCIE;
			usart1ObjectPtr->instance->ICR |= USART_ICR_TCCF;
		}
	}
}

void USART2_IRQHandler (void)
{
	asm("NOP");
}

void USART3_IRQHandler (void)
{
	asm("NOP");
}
