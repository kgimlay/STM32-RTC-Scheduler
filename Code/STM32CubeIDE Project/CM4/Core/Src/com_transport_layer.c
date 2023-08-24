/*
 * uart_phy_wrapper.c
 *
 *  Created on: Aug 23, 2023
 *      Author: kevinimlay
 */


#include <com_transport_layer.h>
#include "string.h"


#define IS_LAYER_INIT (_uartHandle != NULL)


static UART_HandleTypeDef* _uartHandle = NULL;
static char _txBuffer[UART_MESSAGE_SIZE] = {0};
static char _rxBuffer[UART_MESSAGE_SIZE] = {0};
static volatile bool _txBuffer_full = false;
static volatile bool _rxBuffer_full = false;


/*
 *
 */
bool com_transport_init(UART_HandleTypeDef* huart)
{
	// If the programmer didn't pass in a null pointer
	if (huart != NULL)
	{
		_uartHandle = huart;
		_txBuffer_full = false;
		_rxBuffer_full = false;
		return true;
	}

	else
	{
		return false;
	}
}


/*
 *
 */
TRANSPORT_STATUS queue_tx(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE])
{
	if (IS_LAYER_INIT)
	{
		if (_txBuffer_full)
		{
			return TRANSPORT_TX_FULL;
		}

		else
		{
			// Compose header and body into one message
			composeMessage(header, body, _txBuffer);
			_txBuffer_full = true;

			return TRANSPORT_OKAY;
		}
	}

	else
	{
		return TRANSPORT_NOT_INIT;
	}
}


/*
 *
 */
TRANSPORT_STATUS dequeue_rx(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE])
{
	char message[UART_MESSAGE_SIZE];

	if (IS_LAYER_INIT)
	{
		if (!_rxBuffer_full)
		{
			return TRANSPORT_RX_EMPTY;
		}

		else
		{
			// retrieve message from buffer
			memcpy(message, _rxBuffer, UART_MESSAGE_SIZE*sizeof(char));
			_rxBuffer_full = false;

			// decompose header and body from message
			decomposeMessage(header, body, message);

			return TRANSPORT_OKAY;
		}
	}

	else
	{
		return TRANSPORT_NOT_INIT;
	}
}


/*
 *
 */
TRANSPORT_STATUS tx(uint32_t timeout_ms)
{
	HAL_StatusTypeDef hal_status;

	if (IS_LAYER_INIT)
	{
		// only transmit if a message has been queued
		if (!_txBuffer_full)
		{
			return TRANSPORT_TX_EMPTY;
		}

		// transmit the message
		hal_status = HAL_UART_Transmit(_uartHandle, (uint8_t*)_txBuffer, UART_MESSAGE_SIZE, timeout_ms);

		// alias the has status with transport layer status
		if (hal_status == HAL_ERROR)
		{
			return TRANSPORT_ERROR;
		}
		else if (hal_status == HAL_TIMEOUT)
		{
			return TRANSPORT_TIMEOUT;
		}
		else if (hal_status == HAL_BUSY)
		{
			return TRANSPORT_BUSY;
		}
		else
		{
			_txBuffer_full = false;
			return TRANSPORT_OKAY;
		}
	}

	else
	{
		return TRANSPORT_NOT_INIT;
	}
}


/*
 *
 */
TRANSPORT_STATUS rx(uint32_t timeout_ms)
{
	HAL_StatusTypeDef hal_status;

	if (IS_LAYER_INIT)
	{
		// only receive if the buffer is empty
		if (_rxBuffer_full)
		{
			return TRANSPORT_RX_FULL;
		}

		// receive a message
		hal_status = HAL_UART_Receive(_uartHandle, (uint8_t*)_rxBuffer, UART_MESSAGE_SIZE, timeout_ms);

		// alias the has status with transport layer status
		if (hal_status == HAL_ERROR)
		{
			return TRANSPORT_ERROR;
		}
		else if (hal_status == HAL_TIMEOUT)
		{
			return TRANSPORT_TIMEOUT;
		}
		else if (hal_status == HAL_BUSY)
		{
			return TRANSPORT_BUSY;
		}
		else
		{
			_rxBuffer_full = true;
			return TRANSPORT_OKAY;
		}
	}

	else
	{
		return TRANSPORT_NOT_INIT;
	}
}

