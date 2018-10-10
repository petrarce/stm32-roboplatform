#ifndef I2C_IF_H
#define I2C_IF_H
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_i2c.h"

extern I2C_HandleTypeDef hi2c2;

HAL_StatusTypeDef I2C2_Init(void);

#endif //I2C_IF_H
