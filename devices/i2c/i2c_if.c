#include "i2c_if.h"

I2C_HandleTypeDef hi2c2;

static HAL_StatusTypeDef Init_I2C_Local()
{
		/* Initialize I2C */	
	hi2c2.Instance 									= I2C2;
	hi2c2.Init.ClockSpeed 						=	400000;
	hi2c2.Init.AddressingMode 				= I2C_ADDRESSINGMODE_7BIT;
	
	__I2C2_CLK_ENABLE();
	return HAL_I2C_Init(&hi2c2);
}

static void Init_GPIO_Local()
{
	//Init GPIO for I2C
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
  
	__GPIOB_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

HAL_StatusTypeDef I2C2_Init()
{
HAL_StatusTypeDef ret;
	if((ret = Init_I2C_Local()) != HAL_OK){
		return  ret;
	}
	Init_GPIO_Local();
	return ret;

}
