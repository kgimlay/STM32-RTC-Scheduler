/*
 * Author:  Kevin Imlay
 * Date:  September, 2023
 */


#include <desktop_app_session.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>


static bool _sessionOpen = false;
static bool _sessionInit = false;
static char _messageCommand[UART_PACKET_HEADER_SIZE];
static char _messageData[UART_PACKET_PAYLOAD_SIZE];
static bool _messageReady = false;


DesktopComSessionStatus _handshake(unsigned int timeout_ms);
DesktopComSessionStatus _session_cycle(void);
DesktopComSessionStatus _listen(void);
DesktopComSessionStatus _tell(void);


/*
 *
 */
bool desktopAppSession_init(UART_HandleTypeDef* huart)
{
	// initialize transport layer
	if (uartTransport_init(huart))
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
DesktopComSessionStatus desktopAppSession_start(void)
{
	DesktopComSessionStatus handshakeStatus;

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
DesktopComSessionStatus desktopAppSession_pause(void)
{
	return SESSION_OKAY;
}


/*
 *
 */
DesktopComSessionStatus desktopAppSession_update(void)
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
DesktopComSessionStatus desktopAppSession_enqueueMessage(char header[UART_PACKET_HEADER_SIZE],
		char body[UART_PACKET_PAYLOAD_SIZE])
{
	TransportStatus transportStatus;

	if (_sessionInit)
	{
		if (_sessionOpen)
		{
			// enqueue message
			if (uartTransport_bufferTx((uint8_t*)header, (uint8_t*)body) != TRANSPORT_OKAY)
			{
				return SESSION_BUSY;
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
DesktopComSessionStatus _handshake(unsigned int timeout_ms)
{
	unsigned int state = 0;
	bool error = false;
	bool success = false;
	TransportStatus transportStatus;
	char messageHeader[UART_PACKET_HEADER_SIZE] = {0};
	char messageBody[UART_PACKET_PAYLOAD_SIZE] = {0};

	while (!success && !error)
	{
		// state 0:  receive message
		if (state == 0)
		{
			transportStatus = uartTransport_rx_polled(timeout_ms); // handshake timeout until start of handshake
		}
		// state 1: message received, dequeue
		else if (state == 1)
		{
			transportStatus = uartTransport_debufferRx((uint8_t*)messageHeader, (uint8_t*)messageBody);
		}
		// state 2: check if sync
		else if (state == 2)
		{
			if (strncmp(messageHeader, HANDSHAKE_HEADER_SYNC, UART_PACKET_HEADER_SIZE))
			{
				error = true;
			}
		}
		// state 3: sync received, queue ack
		else if (state == 3)
		{
			memset(messageBody,0,UART_PACKET_PAYLOAD_SIZE);
			transportStatus = uartTransport_bufferTx((uint8_t*)HANDSHAKE_HEADER_ACKN, (uint8_t*)messageBody);
		}
		// state 4: send ack
		else if (state == 4)
		{
			transportStatus = uartTransport_tx_polled(SEND_TIMEOUT_MS);
		}
		// state 5: ack sent, receive message
		else if (state == 5)
		{
			transportStatus = uartTransport_rx_polled(RECEIVE_TIMEOUT_MS);
		}
		// state 6: dequeue message
		else if (state == 6)
		{
			transportStatus = uartTransport_debufferRx((uint8_t*)messageHeader, (uint8_t*)messageBody);
		}
		// state 7: message received, check if syn ack
		else // if (state == 7)
		{
			if (strncmp(messageHeader, HANDSHAKE_HEADER_SYNACK, UART_PACKET_HEADER_SIZE))
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
DesktopComSessionStatus _session_cycle(void)
{
	char messageHeader[UART_PACKET_HEADER_SIZE] = {0};
	char messageBody[UART_PACKET_PAYLOAD_SIZE] = {0};
	DesktopComSessionStatus status;

	// Perform Tx message phase of session cycle.
	status = _tell();

	// Perform Rx message phase of session cycle.
	status = _listen();
	if (status == SESSION_ERROR)
	{
		return SESSION_ERROR;
	}

	// If a message was received while listening.
	else if (status == SESSION_OKAY)
	{
		// dequeue received message
		uartTransport_debufferRx((uint8_t*)messageHeader, (uint8_t*)messageBody);

		// Check if disconnection handshake message was received.
		// If so, set session open flag to false.
		if (!strncmp(messageHeader, HANDSHAKE_HEADER_DISC, UART_PACKET_HEADER_SIZE))
		{
			_sessionOpen = false;
		}

		// Check if echo command.
		else if (!strncmp(messageHeader, ECHO_HEADER, UART_PACKET_HEADER_SIZE))
		{
			desktopAppSession_enqueueMessage(messageHeader, messageBody);
		}

		// Else, buffer for processing by the application
		else
		{
			memcpy(_messageCommand, messageHeader, UART_PACKET_HEADER_SIZE*sizeof(char));
			memcpy(_messageData, messageBody, UART_PACKET_PAYLOAD_SIZE*sizeof(char));
			_messageReady = true;
		}
	}

	return SESSION_OKAY;
}


/*
 *
 */
DesktopComSessionStatus _listen(void)
{
	TransportStatus transportStatus;
	char messageBody[UART_PACKET_PAYLOAD_SIZE] = {0};

	// CTS Window
	// Tx the CTS message to signal to desktop that the MCU is about to be ready to
	// receive a message.
	memset(messageBody,0,UART_PACKET_PAYLOAD_SIZE);
	snprintf(messageBody, UART_PACKET_PAYLOAD_SIZE, "Clear to send!\n");
	transportStatus = uartTransport_bufferTx((uint8_t*)CTS_HEADER,(uint8_t*) messageBody);

	if (transportStatus != TRANSPORT_OKAY)
	{
		return SESSION_ERROR;
	}

	transportStatus = uartTransport_tx_polled(SEND_TIMEOUT_MS);

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
	transportStatus = uartTransport_rx_polled(RECEIVE_TIMEOUT_MS);

	if (transportStatus == TRANSPORT_TIMEOUT)
	{
		return SESSION_TIMEOUT;
	}
	else if (transportStatus != TRANSPORT_OKAY)
	{
		return SESSION_ERROR;
	}

	return SESSION_OKAY;
}


/*
 *
 */
DesktopComSessionStatus _tell(void)
{
	TransportStatus transportStatus;

	transportStatus = uartTransport_tx_polled(SEND_TIMEOUT_MS);

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


/*
 *
 */
DesktopComSessionStatus desktopAppSession_dequeueMessage(char header[UART_PACKET_HEADER_SIZE], char body[UART_PACKET_PAYLOAD_SIZE])
{
	if (_messageReady)
	{
		memcpy(header, _messageCommand, UART_PACKET_HEADER_SIZE*sizeof(char));
		memcpy(body, _messageData, UART_PACKET_PAYLOAD_SIZE*sizeof(char));
		_messageReady = false;

		return SESSION_OKAY;
	}

	else
	{
		return SESSION_ERROR;
	}
}
