#ifndef ENGINE_DRIVER_API_H
#define ENGINE_DRIVER_API_H
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "../../common/msg-srv/msg_srv.h"
#include "../usart/usart_if.h"


void ENGINE_init_driver(void);
void ENGINE_main(void* );
	#endif /*ENGINE_DRIVER_API_H*/
