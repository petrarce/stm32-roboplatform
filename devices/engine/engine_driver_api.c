#include "engine_driver_api.h"

/*Current engine is aimed to work with L298N step engine driver. 
	STM32F407VG layout for driver:
		Pins 0,2,4 of GPIOA belongs engine B of L298N controller
		Pins 4,5,6 of GPIOA belongs to engine A of L298N controller
*/
/*Timer configurations*/
#define TIMERS_PRESCALOR 100
#define TIMERS_PERIOD 500
#define TIMERS_MAX_PULS_WIDTH 250

/*private defines*/
#define ENGINE_RIGHT_TRANSMITION_FW() \
	do{ \
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); \
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); \
	}while(0)

#define ENGINE_RIGHT_TRANSMITION_BW() \
	do{ \
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); \
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); \
	}while(0)

#define ENGINE_RIGHT_TRANSMITION_STOP() \
	do{ \
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); \
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); \
	}while(0)

	
#define ENGINE_LEFT_TRANSMITION_FW() \
	do{ \
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); \
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET); \
	}while(0)

#define ENGINE_LEFT_TRANSMITION_BW() \
	do{ \
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); \
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); \
	}while(0)
	
#define ENGINE_LEFT_TRANSMITION_STOP() \
	do{ \
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); \
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET); \
	}while(0)





GPIO_InitTypeDef GPIO_InitStructure;

TIM_HandleTypeDef hTim1, hTim3;
TIM_OC_InitTypeDef sConfigOC;
/*TODO: need to make all functions threadsafe*/
static void ENGINE_timer_tnit(void) 
{
		volatile HAL_StatusTypeDef  status;

		//ENABLE Timer 1 for engines 1 and 2
		__TIM1_CLK_ENABLE();
    hTim1.Instance = TIM1;
    hTim1.Init.Prescaler = TIMERS_PRESCALOR;
    hTim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    hTim1.Init.Period = TIMERS_PERIOD;
    hTim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    status = HAL_TIM_PWM_Init(&hTim1);
	
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
    sConfigOC.Pulse = TIMERS_MAX_PULS_WIDTH;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
	  HAL_TIM_PWM_ConfigChannel(&hTim1, &sConfigOC, TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&hTim1, TIM_CHANNEL_1); // Trying to start the base counter
	
		__TIM3_CLK_ENABLE();
    hTim3.Instance = TIM3;
    hTim3.Init.Prescaler = TIMERS_PRESCALOR;
    hTim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    hTim3.Init.Period = TIMERS_PERIOD;
    hTim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&hTim3);
	
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
    sConfigOC.Pulse = TIMERS_MAX_PULS_WIDTH;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
	  HAL_TIM_PWM_ConfigChannel(&hTim3, &sConfigOC, TIM_CHANNEL_3);

    HAL_TIM_PWM_Start(&hTim3, TIM_CHANNEL_3); // Trying to start the base counter

}


static void ENGINE_gpio_init()
{
  GPIO_InitTypeDef GPIO_InitStruct;
	/* Peripheral clock enable */
	__GPIOA_CLK_ENABLE();
	
	/*configuratie pins for engine 1 and 2*/
	/*Configurate PWM pin for engine 1*/
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
	
	/*configuratie pins for engine 1 and 2*/
	/*Configurate PWM pin */
	__GPIOC_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	

}

static void ENGINE_timers_pulse_width(uint16_t pulse)
{
	HAL_TIM_PWM_Stop(&hTim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&hTim3, TIM_CHANNEL_3);
	
	sConfigOC.Pulse = pulse;
	
	HAL_TIM_PWM_ConfigChannel(&hTim1, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&hTim3, &sConfigOC, TIM_CHANNEL_3);
	
	HAL_TIM_PWM_Start(&hTim1, TIM_CHANNEL_1); // Trying to start the base counter
  HAL_TIM_PWM_Start(&hTim3, TIM_CHANNEL_3); // Trying to start the base counter

}

void ENGINE_init_driver()
{
	ENGINE_gpio_init();
	ENGINE_timer_tnit();
}

void ENGINE_run_forvard_sm(float speed)
{
	float local_speed = 1;
	if(speed > 0 && speed <= 1)
		local_speed = speed;
	ENGINE_LEFT_TRANSMITION_STOP();
	ENGINE_RIGHT_TRANSMITION_STOP();
	ENGINE_timers_pulse_width(local_speed * TIMERS_MAX_PULS_WIDTH);
	ENGINE_LEFT_TRANSMITION_FW();
	ENGINE_RIGHT_TRANSMITION_FW();

}

void ENGINE_run_forvard()
{
	ENGINE_LEFT_TRANSMITION_STOP();
	ENGINE_RIGHT_TRANSMITION_STOP();
	ENGINE_timers_pulse_width(TIMERS_MAX_PULS_WIDTH);
	ENGINE_LEFT_TRANSMITION_FW();
	ENGINE_RIGHT_TRANSMITION_FW();

}
	
void ENGINE_run_backvard_sm(float speed)
{
	float local_speed = 1;
	if(speed > 0 && speed <= 1)
		local_speed = speed;
	ENGINE_LEFT_TRANSMITION_STOP();
	ENGINE_RIGHT_TRANSMITION_STOP();
	ENGINE_timers_pulse_width(local_speed * TIMERS_MAX_PULS_WIDTH);
	ENGINE_LEFT_TRANSMITION_BW();
	ENGINE_RIGHT_TRANSMITION_BW();

}

void ENGINE_run_backvard()
{
	ENGINE_LEFT_TRANSMITION_STOP();
	ENGINE_RIGHT_TRANSMITION_STOP();
	ENGINE_timers_pulse_width(TIMERS_MAX_PULS_WIDTH);
	ENGINE_LEFT_TRANSMITION_BW();
	ENGINE_RIGHT_TRANSMITION_BW();

}
void ENGINE_stop()
{
	ENGINE_LEFT_TRANSMITION_STOP();
	ENGINE_RIGHT_TRANSMITION_STOP();
}

void ENGINE_rotate_clockvice_ang(uint32_t angle)
{
  /*run ENGINE_rotate_clockvice*/
	/*check rotation angle*/
	/*if angle reached - stop rotation by calling ENGINE_stop*/
}

void ENGINE_rotate_clockvice()
{
	ENGINE_LEFT_TRANSMITION_FW();
	ENGINE_RIGHT_TRANSMITION_BW();
}
	
void ENGINE_rotate_unticlockvice_ang(uint32_t angle)
{
	/*run ENGINE_rotate_unticlockvice*/
	/*check rotation angle*/
	/*if angle reached - stop rotation by calling ENGINE_stop*/

}

void ENGINE_rotate_unticlockvice()
{
	ENGINE_LEFT_TRANSMITION_BW();
	ENGINE_RIGHT_TRANSMITION_FW();

}
