/*
 * uart_communication.h
 *
 *  Created on: Jun 24, 2023
 *      Author: kevinimlay
 */

#ifndef INC_UART_COMMUNICATION_H_
#define INC_UART_COMMUNICATION_H_

/*
 * Structure to hold process and report queues (rx and tx respectively)
 * as well as operaiton variables
 */
typedef struct
{
	UART_Queue _process_queue;
	UART_Queue _report_queue;
} UART_COM;

#endif /* INC_UART_COMMUNICATION_H_ */
