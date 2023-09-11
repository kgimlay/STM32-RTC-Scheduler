/*
 * serial_message.h
 *
 *  Created on: Jun 21, 2023
 *      Author: kevinimlay
 */

#ifndef INC_COM_DATALINK_LAYER_H_
#define INC_COM_DATALINK_LAYER_H_

#include "stdint.h"

/*
 * Sizes for SerialMessages
 */
#define UART_MESSAGE_SIZE 64
#define UART_MESSAGE_HEADER_SIZE 4
#define UART_MESSAGE_BODY_SIZE (UART_MESSAGE_SIZE - UART_MESSAGE_HEADER_SIZE)

/*
 * A SerialMessage is made up of a header and a body. The header represents
 * a type for the message, that is, the command type or response type, and
 * the body contains the information that is necessary for the message type.
 */
typedef struct {
	char header[UART_MESSAGE_HEADER_SIZE];
	char body[UART_MESSAGE_BODY_SIZE];
} SerialMessage;

/*
 *
 */
void composeMessage(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE],
		char message[UART_MESSAGE_SIZE]);

/*
 *
 */
void decomposeMessage(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE],
		char message[UART_MESSAGE_SIZE]);

#endif /* INC_COM_DATALINK_LAYER_H_ */
