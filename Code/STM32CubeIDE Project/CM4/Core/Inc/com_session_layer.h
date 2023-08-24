/*
 * com_session_layer.h
 *
 *  Created on: Aug 23, 2023
 *      Author: kevinimlay
 */

#ifndef INC_COM_SESSION_LAYER_H_
#define INC_COM_SESSION_LAYER_H_


#include <stdbool.h>
#include "com_transport_layer.h"
#include "com_datalink_layer.h"


#define RX_TIMEOUT_MS 250
#define TX_TIMEOUT_MS 250
#define SESSION_START_TIMEOUT_MS 1000

#define HANDSHAKE_HEADER_SYNC "SYNC\0"
#define HANDSHAKE_HEADER_ACKN "ACKN\0"
#define HANDSHAKE_HEADER_SYNACK "SYNA\0"


/* Public Types */
typedef enum {
	SESSION_OKAY,
	SESSION_TIMEOUT,
	SESSION_ERROR,
	SESSION_NOT_INIT,
	SESSION_NO_HANDSHAKE,
	SESSION_BUSY
} SESSION_STATUS;


/* Public Function Definitions */


/*
 *
 */
bool com_session_init(UART_HandleTypeDef* huart);

/*
 *
 */
SESSION_STATUS start_session(void);

/*
 *
 */
SESSION_STATUS end_session(void);

/*
 *
 */
SESSION_STATUS tell(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE]);

/*
 *
 */
SESSION_STATUS listen(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE]);


#endif /* INC_COM_SESSION_LAYER_H_ */
