/*
 * uart_messge.c
 *
 *  Created on: Jun 26, 2023
 *      Author: kevinimlay
 */


#include <com_datalink_layer.h>
#include "string.h"


/*
 *
 */
void composeMessage(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE],
		char message[UART_MESSAGE_SIZE])
{
	memcpy(message, header, UART_MESSAGE_HEADER_SIZE*sizeof(char));
	memcpy(message+UART_MESSAGE_HEADER_SIZE, body, UART_MESSAGE_BODY_SIZE*sizeof(char));
}


/*
 *
 */
void decomposeMessage(char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE],
		char message[UART_MESSAGE_SIZE])
{
	memcpy(header, message, UART_MESSAGE_HEADER_SIZE*sizeof(char));
	memcpy(body, message+UART_MESSAGE_HEADER_SIZE, UART_MESSAGE_BODY_SIZE*sizeof(char));
}
