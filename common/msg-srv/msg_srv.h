#ifndef MSG_SRV_H
#define MSG_SRV_H

#include "stdint.h"



#define MSGSRV_MAX_CLIENTS 		10
#define MSGSRV_MAX_QUEUE_OBJ 	100
#define MSGSRV_MAX_MSG_SIZE 	100
#define MSGSRV_PUT_TO 				1000

typedef enum {
	MSGSRV_OK,
	MSGSRV_NOK,
	MSGSRV_QUEUE_ALLOC_FAILED, 		//registration of client failed
	MSGSRV_MAX_CLIENT_ID_FAILED, 	//unregister of client failed
	MSGSRV_CLIENT_ALREADY_REGISTERED, 						//message send timeouted
	MSGSRV_QUEUE_FULL
} MSGSRV_Status;
typedef void (*MSGSRV_client_hook_t)(void*);
typedef struct {
	uint8_t targetId;
	void* args;
} MSGSRV_queue_obj;



enum MSGSRV_CLNT_LIST{
	MSGSRV_UART_CLNT,
	MSGSRV_ENGINE_CLNT,
	MSGSRV_MPU6050_CLNT
};

MSGSRV_Status MSGSRV_init(void);
MSGSRV_Status MSGSRV_send(uint8_t targetId, void* msg);
MSGSRV_Status MSGSRV_register_client(uint8_t, MSGSRV_client_hook_t);
MSGSRV_Status MSGSRV_unregister_client(uint8_t);
void MSGSRV_main(void* args);

#endif
