/*
 * serial_message.h
 *
 *  Created on: Jun 21, 2023
 *      Author: kevinimlay
 */

#ifndef INC_SERIAL_MESSAGE_H_
#define INC_SERIAL_MESSAGE_H_

#include "stdint.h"

/*
 * Sizes for SerialMessages
 */
#define SERIAL_MESSAGE_SIZE 64
#define SERIAL_HEADER_SIZE 4
#define SERIAL_BODY_SIZE (SERIAL_MESSAGE_SIZE - SERIAL_HEADER_SIZE)

/*
 * A SerialMessage is made up of a header and a body. The header represents
 * a type for the message, that is, the command type or response type, and
 * the body contains the information that is necessary for the message type.
 */
typedef struct {
	char header[SERIAL_HEADER_SIZE];
	char body[SERIAL_BODY_SIZE];
} SerialMessage;

/*
 *
 */

#endif /* INC_SERIAL_MESSAGE_H_ */
