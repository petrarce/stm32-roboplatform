#include "usart_if.h"
#include <stdint.h>
#include <string.h>

UART_HandleTypeDef UartHandle;
UART_BufStr uart2_gl_buf;
char usart2_lc_buf[1];

//local functions and varicables
static void UART_clear_buf()
{
	memset(uart2_gl_buf.buf, 0, sizeof(uart2_gl_buf.buf));
	uart2_gl_buf.bufPos = 0;
}

static UART_OpCode UART_push_buf(char* str, int size)
{
	if(size + uart2_gl_buf.bufPos > sizeof(uart2_gl_buf.buf))
		return UART_NOK;
	
	memcpy(uart2_gl_buf.buf + uart2_gl_buf.bufPos, str, size);
	uart2_gl_buf.bufPos += size;
	return UART_OK;
}

//global uart apis
void UART_Init(UART_HandleTypeDef* UartHandle)
{
	//enable clock for uart and gpio
  __USART2_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
    
	//configure gpio for uart
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.Pin = GPIO_PIN_2;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//configure UART
	__HAL_UART_ENABLE_IT(UartHandle, UART_IT_RXNE);
	
	UartHandle->Instance        = UARTx;
	UartHandle->Init.BaudRate   = UART_BDR;
	UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle->Init.StopBits   = UART_STOPBITS_1;
	UartHandle->Init.Parity     = UART_PARITY_NONE;
	UartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle->Init.Mode       = UART_MODE_TX_RX;
	HAL_UART_Init(UartHandle);	
	
	//configure NVIC priority
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	
	UART_clear_buf();
	HAL_UART_Receive_IT(UartHandle, (uint8_t*) usart2_lc_buf, sizeof(usart2_lc_buf));
}


HAL_StatusTypeDef UART_printf(char* str)
{
	return HAL_UART_Transmit_IT(&UartHandle, (uint8_t*) str, strlen(str));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED1 on: Transfer in transmission process is correct */
	switch(usart2_lc_buf[0]){
		
		case '<': 
			//end of transmittion. Need to put message into a global queue
			UART_clear_buf();
			break;
		
		default:
			if(UART_NOK == UART_push_buf(usart2_lc_buf, 1))
				UART_clear_buf();
			break;
	}
	HAL_UART_Receive_IT(UartHandle, (uint8_t*) usart2_lc_buf, sizeof(usart2_lc_buf));
}

void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(& UartHandle);
}
