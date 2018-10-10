#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include "usart_if.h"
#include "../../common/msg-srv/msg_srv.h"

UART_HandleTypeDef UartHandle;
UART_BufStr uart2_gl_buf;
static char usart2_lc_buf[1];
char tmp_msg[10];


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

static int8_t  parseInput(char* str, uint8_t strsize)
{
	char dest = str[0];
	char* tokstr;
	tokstr = strtok(str, "~");
	tokstr = strtok(NULL, "~");
	strcpy(str, tokstr);
	memset(str + strlen(str), 0, strsize - strlen(str));
	switch(dest){
		case 'e':
			return MSGSRV_ENGINE_CLNT;
		case 'a':
			return MSGSRV_MPU6050_CLNT;
		default:
			return -1;			
	}	
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
	__HAL_UART_ENABLE_IT(UartHandle, UART_IT_TC);
	
	UartHandle->Instance        = UARTx;
	UartHandle->Init.BaudRate   = UART_BDR;
	UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle->Init.StopBits   = UART_STOPBITS_1;
	UartHandle->Init.Parity     = UART_PARITY_NONE;
	UartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle->Init.Mode       = UART_MODE_TX_RX;
	HAL_UART_Init(UartHandle);	
	
	//configure NVIC priority
	HAL_NVIC_SetPriority(USART2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
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
	int8_t client;
  /* Turn LED1 on: Transfer in transmission process is correct */
	switch(usart2_lc_buf[0]){
		
		case '<': 
			//end of transmittion. Need to put message into a global queue
			client = parseInput(uart2_gl_buf.buf, sizeof(uart2_gl_buf.buf));
			memcpy(tmp_msg, uart2_gl_buf.buf, sizeof(uart2_gl_buf.buf));
			MSGSRV_send(client, tmp_msg);
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

