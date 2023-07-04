/*
 * uart_communication.h
 *
 *  Created on: Jun 24, 2023
 *      Author: kevinimlay
 */

#ifndef INC_UART_BASIC_COM_H_
#define INC_UART_BASIC_COM_H_

#include "stdbool.h"
#include "uart_message.h"
#include "stm32wlxx_hal.h"

/*
 *
 */

/*
 * Definitions for polling timeouts
 */
#define TX_POLL_TIMEOUT 250
#define RX_POLL_TIMEOUT 250

/* Usage Functions for application interface (API) */

/*
 * Initializes the module.  Must be called before beginning any communications with
 * the Desktop application.
 */
void uartBasic_init(UART_HandleTypeDef* uartHandle);

/*
 * Start transmitting message over UART.
 */
bool uartBasic_TX_IT(char header[UART_MESSAGE_HEADER_SIZE],
		char body[UART_MESSAGE_BODY_SIZE]);

/*
 * Begins listening for messages over UART.
 */
bool uartBasic_RX_IT(void);

/*
 * Get received message from RX buffer.
 */
bool uartBasic_get_RX(char header[UART_MESSAGE_HEADER_SIZE],
		char body[UART_MESSAGE_BODY_SIZE]);

/* ISR Functions for handling UART interrupts */

/*
 * ISR function for handling UART receive complete (RX).
 */
void uartBasic_RX_Complete_ISR(void);

/*
 * ISR function for handling UART transmit complete (TX).
 */
void uartBasic_TX_Complete_ISR(void);

/*
 * ISR Function for handling UART errors
 */
void uartBasic_Error_ISR(void);

#endif /* INC_UART_BASIC_COM_H_ */
