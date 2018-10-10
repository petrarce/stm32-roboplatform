/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  *
  * @note    modified by ARM
  *          The modifications allow to use this file as User Code Template
  *          within the Device Family Pack.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017-2018 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "debug.h"
#include "main.h"
#include "devices/engine/engine_driver_api.h"
#include "devices/usart/usart_if.h"
#include "devices/accelerometer/tm_stm32f4_lis302dl_lis3dsh.h"
#include "devices/i2c/i2c_if.h"
#include "devices/mpu6050/sd_hal_mpu6050.h"
#include "common/msg-srv/msg_srv.h"

//**********************************FREERTOS HEADERS****************************
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "croutine.h"
//******************************************************************************


TaskStatus_t		pxTaskStatusArray[configTASKS_COUNT];
uint32_t				totalRunTime;



int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */

	
  HAL_Init();
	
	
  /*It is recommended to assign all the priority bits to be preempt priority bits, 
	leaving no priority bits as subpriority bits. Any other configuration complicates 
	the otherwise direct relationship between the configMAX_SYSCALL_INTERRUPT_PRIORITY 
	setting and the priority assigned to individual 
	peripheral interrupts.*/
 	HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );

	//devices initialisation section
	UART_Init(&UartHandle);
	ENGINE_init_driver();
	I2C2_Init();
	//common components initialisation section
	MSGSRV_init();
	
	//create new threads
	xTaskCreate( MSGSRV_main, 				"MSGSRV", 		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate( ENGINE_main, 				"ENGINE", 		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate( MPU6050_main_thread,	"MPU6050", 		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate( MPU6050_poll_thread,	"MPU6050", 		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

		
	vTaskStartScheduler();

	while(1){}
}

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName )
																		
{
	UART_printf("FATAL: stack overflow hook is started");
	uxTaskGetSystemState(pxTaskStatusArray, sizeof(pxTaskStatusArray), NULL);
	while(1){}
}

void vApplicationMallocFailedHook( TaskHandle_t xTask,
                                    signed char *pcTaskName )
{
		UART_printf("FATAL: malloc failed hook entered");
		uxTaskGetSystemState(pxTaskStatusArray, sizeof(pxTaskStatusArray), NULL);
		while(1){}
																		
}

void vApplicationIdleHook()
{
			uxTaskGetSystemState(pxTaskStatusArray, sizeof(pxTaskStatusArray), NULL);
}