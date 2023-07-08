/*
 * desktop_app_communication.c
 *
 *  Created on: Jun 24, 2023
 *      Author: kevinimlay
 */

#include <uart_basic_com.h>
#include "string.h"


/*
 * Static operation variables
 *
 * There should be only one 'instance' of this communicator (if approaching this like
 * OOP) so it is sufficient to make these variables static to the file rather than a
 * structure that is passed in as a parameter.
 */
static volatile bool _txError = false;
static volatile bool _rxError = false;
static UART_HandleTypeDef* _uartHandle = NULL;
static char _txBuffer[UART_MESSAGE_SIZE] = {0};
static char _rxBuffer[UART_MESSAGE_SIZE] = {0};
static volatile bool _txBuffer_full = false;
static volatile bool _rxBuffer_full = false;


/*  Private Function Prototypes */
void _txMessage_IT(void);
void _rxMessage_IT(void);
void _txMessage_Poll(void);
void _rxMessage_Poll(void);


/* Usage Functions for application interface (API) */


/*
 *
 */
void uartBasic_init(UART_HandleTypeDef* uartHandle)
{
	// initialize structure fields
	_uartHandle = uartHandle;
}


/*
 *
 */
bool uartBasic_TX_IT(char header[UART_MESSAGE_HEADER_SIZE],
		char body[UART_MESSAGE_BODY_SIZE])
{
	// if the buffer is not full
	if (!_txBuffer_full)
	{
		// Compose header and body into one message
		composeMessage(header, body, _txBuffer);
		_txBuffer_full = true;

		// and send
		_txMessage_IT();

		// report successfully queued
		return true;
	}

	else
	{
		// return full buffer
		return false;
	}
}


/*
 *
 */
bool uartBasic_RX_IT(void)
{
	// if the buffer is not full
	if (!_rxBuffer_full)
	{
		// start receiving
		_rxMessage_IT();

		// return success
		return true;
	}

	// rx buffer is full
	else
	{
		// return full buffer
		return false;
	}
}


/*
 *
 */
bool uartBasic_get_RX(char header[UART_MESSAGE_HEADER_SIZE],
		char body[UART_MESSAGE_BODY_SIZE])
{
	char message[UART_MESSAGE_SIZE];

	// if the queue is not empty
	if (_rxBuffer_full)
	{
		// retrieve message from process queue
		memcpy(message, _rxBuffer, UART_MESSAGE_SIZE*sizeof(char));
		_rxBuffer_full = false;

		// decompose header and body from message
		decomposeMessage(header, body, message);

		// report successfully dequeued
		return true;
	}

	// report queue is empty
	else
	{
		// report empty
		return false;
	}
}


/* ISR Functions for handling UART interrupts */


/*
 *
 */
void uartBasic_RX_Complete_ISR(void)
{
	_rxBuffer_full = true;
}


/*
 *
 */
void uartBasic_TX_Complete_ISR(void)
{
	_txBuffer_full = false;
}


/*
 *
 */
void uartBasic_Error_ISR(void)
{
	while (1)
		(void)0; // no operation
}


/* Private Function Definitions */


/*
 *
 */
void _txMessage_IT(void)
{
	if (HAL_UART_Transmit_IT(_uartHandle, (uint8_t*)_txBuffer, UART_MESSAGE_SIZE) == HAL_ERROR)
		_txError = true;
}


/*
 *
 */
void _rxMessage_IT(void)
{
	if (HAL_UART_Receive_IT(_uartHandle, (uint8_t*)_rxBuffer, UART_MESSAGE_SIZE) == HAL_ERROR)
		_rxError = true;
}


/*
 *
 */
void _txMessage_Poll(void)
{
	if (HAL_UART_Transmit(_uartHandle, (uint8_t*)_txBuffer, UART_MESSAGE_SIZE, TX_POLL_TIMEOUT) == HAL_ERROR)
		_txError = true;
}


/*
 *
 */
void _rxMessage_Poll(void)
{
	if (HAL_UART_Receive(_uartHandle, (uint8_t*)_rxBuffer, UART_MESSAGE_SIZE, RX_POLL_TIMEOUT) == HAL_ERROR)
		_rxError = true;
}
