/*
 * com_session_layer.c
 *
 *  Created on: Aug 23, 2023
 *      Author: kevinimlay
 */


#include <string.h>
#include <stdio.h>
#include "com_session_layer.h"


static bool _sessionOpen = false;
static bool _sessionInit = false;


SESSION_STATUS _handshake(unsigned int timeout_ms);
SESSION_STATUS _session_cycle(void);
SESSION_STATUS _message_phase(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE], bool* messageReceived);


/*
 *
 */
bool com_session_init(UART_HandleTypeDef* huart)
{
	// initialize transport layer
	if (com_transport_init(huart))
	{
		_sessionOpen = false;
		_sessionInit = true;
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
SESSION_STATUS start_session(void)
{
	SESSION_STATUS handshakeStatus;

	if (_sessionInit)
	{
		if (!_sessionOpen)
		{
			handshakeStatus = _handshake(SESSION_START_TIMEOUT_MS);
			if (handshakeStatus == SESSION_OKAY)
				_sessionOpen = true;
			return handshakeStatus;
		}

		else
		{
			return SESSION_OKAY;
		}
	}

	else
	{
		return SESSION_NOT_INIT;
	}
}


/*
 *
 */
SESSION_STATUS session_cycle(void)
{
	if (_sessionInit)
	{
		if (_sessionOpen)
		{
			return _session_cycle();
		}

		else
		{
			return SESSION_NOT_OPEN;
		}
	}

	else
	{
		return SESSION_NOT_INIT;
	}
}


/*
 *
 */
SESSION_STATUS tell(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE])
{
	TRANSPORT_STATUS transportStatus;

	if (_sessionInit)
	{
		if (_sessionOpen)
		{
			// enqueue message
			if (queue_tx(header, body) != TRANSPORT_OKAY)
			{
				return SESSION_BUSY;
			}

			// send message
			transportStatus = tx(TX_TIMEOUT_MS);

			if (transportStatus == TRANSPORT_OKAY)
			{
				return SESSION_OKAY;
			}
			else if (transportStatus == TRANSPORT_TIMEOUT)
			{
				return SESSION_TIMEOUT;
			}
			else // if (transportStatus == TRANSPORT_ERROR || transportStatus == TRANSPORT_BUSY)
			{
				return SESSION_ERROR;
			}
		}

		else
		{
			return SESSION_NOT_OPEN;
		}
	}

	else
	{
		return SESSION_NOT_INIT;
	}
}


/*
 *
 */
SESSION_STATUS listen(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE])
{
	TRANSPORT_STATUS transportStatus;

	if (_sessionInit)
	{
		if (_sessionOpen)
		{
			// receive message
			transportStatus = rx(TX_TIMEOUT_MS);

			if (transportStatus == TRANSPORT_OKAY)
			{
				dequeue_rx(header, body);
				return SESSION_OKAY;
			}
			else if (transportStatus == TRANSPORT_TIMEOUT)
			{
				return SESSION_TIMEOUT;
			}
			else // if (transportStatus == TRANSPORT_ERROR || transportStatus == TRANSPORT_BUSY)
			{
				return SESSION_ERROR;
			}
		}

		else
		{
			return SESSION_NOT_OPEN;
		}
	}

	else
	{
		return SESSION_NOT_INIT;
	}
}


/*
 *
 */
SESSION_STATUS _handshake(unsigned int timeout_ms)
{
	unsigned int state = 0;
	bool error = false;
	bool success = false;
	TRANSPORT_STATUS transportStatus;
	char messageHeader[UART_MESSAGE_HEADER_SIZE] = {0};
	char messageBody[UART_MESSAGE_BODY_SIZE] = {0};

	while (!success && !error)
	{
		// state 0:  receive message
		if (state == 0)
		{
			transportStatus = rx(timeout_ms); // handshake timeout until start of handshake
		}
		// state 1: message received, dequeue
		else if (state == 1)
		{
			transportStatus = dequeue_rx(messageHeader, messageBody);
		}
		// state 2: check if sync
		else if (state == 2)
		{
			if (strncmp(messageHeader, HANDSHAKE_HEADER_SYNC, UART_MESSAGE_HEADER_SIZE))
			{
				error = true;
			}
		}
		// state 3: sync received, queue ack
		else if (state == 3)
		{
			memset(messageBody,0,UART_MESSAGE_BODY_SIZE);
			transportStatus = queue_tx(HANDSHAKE_HEADER_ACKN, messageBody);
		}
		// state 4: send ack
		else if (state == 4)
		{
			transportStatus = tx(TX_TIMEOUT_MS);
		}
		// state 5: ack sent, receive message
		else if (state == 5)
		{
			transportStatus = rx(RX_TIMEOUT_MS);
		}
		// state 6: dequeue message
		else if (state == 6)
		{
			transportStatus = dequeue_rx(messageHeader, messageBody);
		}
		// state 7: message received, check if syn ack
		else // if (state == 7)
		{
			if (strncmp(messageHeader, HANDSHAKE_HEADER_SYNACK, UART_MESSAGE_HEADER_SIZE))
			{
				error = true;
			}
		}


		// catch status codes and move through state machine
		if (transportStatus == TRANSPORT_OKAY)
		{
			if (state == 0)
				state = 1;
			else if (state == 1)
				state = 2;
			else if (state == 2)
				state = 3;
			else if (state == 3)
				state = 4;
			else if (state == 4)
				state = 5;
			else if (state == 5)
				state = 6;
			else if (state == 6)
				state = 7;
			else // if (state == 7)
				success = true;
		}
		else
		{
			error = true;
		}
	}


	// report status of handshake
	if (success && !error)
	{
		return SESSION_OKAY;
	}
	else
	{
		if (transportStatus == TRANSPORT_TIMEOUT)
		{
			return SESSION_TIMEOUT;
		}
		else
		{
			return SESSION_ERROR;
		}
	}
}


/*
 *
 */
SESSION_STATUS _session_cycle(void)
{
	char messageHeader[UART_MESSAGE_HEADER_SIZE] = {0};
	char messageBody[UART_MESSAGE_BODY_SIZE] = {0};
	bool* isMessage = false;

	// Perform message phase of session cycle.
	return _message_phase(messageHeader, messageBody, isMessage);
}


/*
 *
 */
SESSION_STATUS _message_phase(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE], bool* messageReceived)
{
	TRANSPORT_STATUS transportStatus;
	char messageBody[UART_MESSAGE_BODY_SIZE] = {0};

	// CTS Window
	// Tx the CTS message to signal to desktop that the MCU is about to be ready to
	// receive a message.
	memset(messageBody,0,UART_MESSAGE_BODY_SIZE);
	snprintf(messageBody, UART_MESSAGE_BODY_SIZE, "Clear to send!\n");
	transportStatus = queue_tx(CTS_HEADER, messageBody);

	if (transportStatus != TRANSPORT_OKAY)
	{
		return SESSION_ERROR;
	}

	transportStatus = tx(TX_TIMEOUT_MS);

	if (transportStatus == TRANSPORT_TIMEOUT)
	{
		return SESSION_TIMEOUT;
	}
	else if (transportStatus != TRANSPORT_OKAY)
	{
		return SESSION_ERROR;
	}

	// Message Window
	// Rx to receive a packet from the desktop.
	transportStatus = rx(RX_TIMEOUT_MS);

	if (transportStatus == TRANSPORT_TIMEOUT)
	{
		return SESSION_TIMEOUT;
	}
	else if (transportStatus != TRANSPORT_OKAY)
	{
		return SESSION_ERROR;
	}

	transportStatus = dequeue_rx(header, body);
	*messageReceived = true;

	return SESSION_OKAY;
}
