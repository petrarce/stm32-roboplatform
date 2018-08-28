#include <string.h>
#include <stdlib.h>
#include "msg_srv.h"
#include "../../devices/usart/usart_if.h"

//**********************************FREERTOS HEADERS****************************
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "croutine.h"
//******************************************************************************

MSGSRV_client_hook_t 	MSGSRV_clientHooks[MSGSRV_MAX_CLIENTS];
QueueHandle_t 				MSGSRV_queue;


void MSGSRV_main(void *argument);

MSGSRV_Status MSGSRV_init()
{
	
	//create queue
	MSGSRV_queue = xQueueCreate(MSGSRV_MAX_QUEUE_OBJ, sizeof(MSGSRV_queue_obj));
	if(!MSGSRV_queue)
		return MSGSRV_QUEUE_ALLOC_FAILED;
	
	//create new thread
	xTaskCreate( MSGSRV_main, "MSGSRV main thread", 
	STACK_SIZE_MIN, NULL, tskIDLE_PRIORITY, NULL );

	return MSGSRV_OK;
	
}

MSGSRV_Status MSGSRV_send(uint8_t targetId, void* msg)
{
	MSGSRV_queue_obj tmp = {targetId, msg};
	
	if(xQueueSendFromISR(MSGSRV_queue, &tmp, 0) == errQUEUE_FULL)
		return MSGSRV_QUEUE_FULL;
	
	return MSGSRV_OK;
}

MSGSRV_Status MSGSRV_register_client(uint8_t clntId, MSGSRV_client_hook_t hook)
{
	if(clntId > MSGSRV_MAX_CLIENTS)
		return MSGSRV_MAX_CLIENT_ID_FAILED;	//exceeded maximum of registration clients
	
	if(MSGSRV_clientHooks[clntId])
		return MSGSRV_CLIENT_ALREADY_REGISTERED; //client already registered
	
	MSGSRV_clientHooks[clntId] = hook;
	return MSGSRV_OK;
}

MSGSRV_Status MSGSRV_unregister_client(uint8_t clntId)
{
	if(clntId < MSGSRV_MAX_CLIENTS)
		MSGSRV_clientHooks[clntId] = NULL;

	return MSGSRV_OK;
}

void MSGSRV_main(void *argument)
{
  MSGSRV_queue_obj msg;
  while (1) {
    ; // Insert thread code here...
		//TODO: add freertos wait message queue API
		xQueueReceive(MSGSRV_queue, &msg, portMAX_DELAY);

		if(MSGSRV_clientHooks[msg.targetId])
        MSGSRV_clientHooks[msg.targetId](msg.args);
  }
}
