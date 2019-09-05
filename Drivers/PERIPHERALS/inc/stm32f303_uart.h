/*
 * stm32f303_uart.h
 *
 *  Created on: 01.09.2019
 *      Author: rafal
 */

#ifndef PERIPHERALS_INC_STM32F303_UART_H_
#define PERIPHERALS_INC_STM32F303_UART_H_

#define PERIPH_USART_CIRCULLAR_BUFFER_TX_SIZE 256
#define PERIPH_USART_CIRCULLAR_BUFFER_RX_SIZE 256

void USART1_IRQHandler (void);
void USART2_IRQHandler (void);
void USART3_IRQHandler (void);

typedef struct
{
	USART_TypeDef * instance;
	char usartCircullarBuffertTx[PERIPH_USART_CIRCULLAR_BUFFER_TX_SIZE];
	char usartCircullarBuffertRx[PERIPH_USART_CIRCULLAR_BUFFER_RX_SIZE];
	uint32_t usartCircullarBuffertTxHead;
	uint32_t usartCircullarBuffertTxTail;
	uint32_t usartCircullarBuffertRxHead;
	uint32_t usartCircullarBuffertRxTail;
	uint32_t usartnoOfReceivedLines;
	void ( * newLineReceivedCallbackPtr )(void);
}PERIPH_USART_Instance_t;

void PERIPH_USART_Init(USART_TypeDef * usartNo, PERIPH_USART_Instance_t * object, uint32_t baudrate);
char PERIPH_USART_GetChar(PERIPH_USART_Instance_t * object);
char * PERIPH_USART_GetString(PERIPH_USART_Instance_t * object, char * inOutBuffer);
void PERIPH_USART_RegisterRxCallback(PERIPH_USART_Instance_t * object , void (*rxCallbackPtr)(void));
void PERIPH_USART_RXService(void);
void PERIPH_USART_putc(PERIPH_USART_Instance_t * object, char data);
void PERIPH_USART_puts(PERIPH_USART_Instance_t * object, char * dataBuffer);

#endif /* PERIPHERALS_INC_STM32F303_UART_H_ */
