/*
 * uart_queue.h
 *
 *  Created on: Jun 21, 2023
 *      Author: kevinimlay
 */

#ifndef INC_UART_QUEUE_H_
#define INC_UART_QUEUE_H_


#include "serial_message.h"
#include "stdint.h"
#include "stdbool.h"

/*
 * The number of entries in the queue
 */
#define QUEUE_SIZE 4

/*
 * The number of characters in each buffer
 */
#define QUEUE_BUFFER_SIZE SERIAL_MESSAGE_SIZE

/*
 * Queue struct to hold messages and necessary information to operate
 * the queue.
 */
typedef struct {
	bool _isEmpty;
	int _front;
	int _rear;
	char _queue[QUEUE_SIZE][QUEUE_BUFFER_SIZE];
} UART_Queue;

/*
 * Operation codes for this utility.
 */
typedef enum {
	UART_QUEUE_SUCCESS,
	UART_QUEUE_FULL,
	UART_QUEUE_EMPTY
} UART_QUEUE_STATUS;

/*
 * Initializes a UART_Queue structure.
 * Call before attempting to operate on the queue.
 */
void uartQueue_init(UART_Queue* queue);

/*
 * Enqueue a message.
 * Return success if enqueueing successful, or full if the queue is full.
 */
UART_QUEUE_STATUS uartQueue_enqueue(UART_Queue* queue, char message[QUEUE_BUFFER_SIZE]);

/*
 * Dequeue a message.
 * Return success and message if message present, or empty and null if queue
 * is empty.
 */
UART_QUEUE_STATUS uartQueue_dequeue(UART_Queue* queue, char messageBuffer[QUEUE_BUFFER_SIZE]);

#endif /* INC_UART_QUEUE_H_ */
