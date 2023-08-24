/*
 * uart_phy_wrapper.h
 *
 *  Created on: Aug 23, 2023
 *      Author: kevinimlay
 */

#ifndef INC_COM_TRANSPORT_LAYER_H_
#define INC_COM_TRANSPORT_LAYER_H_


#include <com_datalink_layer.h>
#include <stdbool.h>
#include "stm32wlxx_hal.h"


/* Public Types */
typedef enum {
	TRANSPORT_OKAY,
	TRANSPORT_TIMEOUT,
	TRANSPORT_ERROR,
	TRANSPORT_BUSY,
	TRANSPORT_TX_FULL,
	TRANSPORT_TX_EMPTY,
	TRANSPORT_RX_EMPTY,
	TRANSPORT_RX_FULL,
	TRANSPORT_NOT_INIT
} TRANSPORT_STATUS;


/* Public Function Prototypes */

/*
 *
 */
bool com_transport_init(UART_HandleTypeDef* huart);

/*
 *
 */
TRANSPORT_STATUS queue_tx(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE]);

/*
 *
 */
TRANSPORT_STATUS dequeue_rx(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE]);

/*
 *
 */
TRANSPORT_STATUS tx(uint32_t timeout_ms);

/*
 *
 */
TRANSPORT_STATUS rx(uint32_t timeout_ms);


#endif /* INC_COM_TRANSPORT_LAYER_H_ */
