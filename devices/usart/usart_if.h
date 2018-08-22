#ifndef USART_IF_H
#define USART_IF_H

#include "../../main.h"

#define UARTx USART2
#define UART_TO 1000  //uart receive/transmit timeout
#define UART_BDR 9600 //uart baudrate
#define USART_BUF_LEGTH 10

typedef enum  {
	UART_OK,
	UART_NOK
} UART_OpCode;
typedef struct {
	char buf[USART_BUF_LEGTH];
	uint16_t bufPos;
} UART_BufStr;

extern UART_HandleTypeDef UartHandle;

void UART_Init(UART_HandleTypeDef* UartHandle);
HAL_StatusTypeDef UART_printf(char* buff);



#endif /*USART_IF_H*/
