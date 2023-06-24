/*
 * uart_queue.c
 *
 *  Created on: Jun 21, 2023
 *      Author: kevinimlay
 */


#include "uart_queue.h"
#include "string.h"


/*
 * Private helper to calculate next index in circular queue.
 */
uint8_t nextIdx(uint8_t index) {
	return (index + 1) % QUEUE_SIZE;
}

/*
 * Private helper to enqueue a message to the queue.
 */
void _enqueue(UART_Queue* queue, char message[QUEUE_BUFFER_SIZE]) {
	// copy the message to the queue and increment the rear pointer
	memcpy(queue->_queue[queue->_rear], message, QUEUE_BUFFER_SIZE*sizeof(char));
	queue->_rear = nextIdx(queue->_rear);
}

/*
 * Private helper to dequeue a message from the queue.
 */
void _dequeue(UART_Queue* queue, char messageBuffer[QUEUE_BUFFER_SIZE]) {
	// copy message from  the queue and increment the front pointer
	strncpy(messageBuffer, queue->_queue[queue->_front], QUEUE_BUFFER_SIZE);
	queue->_front = nextIdx(queue->_front);

	// set isEmpty flag if necessary
	if (queue->_front == queue->_rear)
		queue->_isEmpty = true;
}

/*
 * Initializes a UART_Queue structure.
 * Call before attempting to operate on the queue.
 */
void uartQueue_init(UART_Queue* queue) {
	// operation variables
	int row, col;

	// reset all operation variables
	queue->_isEmpty = true;
	queue->_front = 0;
	queue->_rear = 0;

	// ensure that the queue messages are empty
	for (row = 0; row < SERIAL_MESSAGE_SIZE; row++)
		for (col = 0; col < SERIAL_MESSAGE_SIZE; col++)
			queue->_queue[row][col] = '\0';
}

/*
 * Enqueue a message.
 * Return success if enqueueing successful, or full if the queue is full.
 */
UART_QUEUE_STATUS uartQueue_enqueue(UART_Queue* queue, char message[QUEUE_BUFFER_SIZE]) {
	// operation variables

	// case that queue is empty
	if (queue->_isEmpty) {
		// set not empty
		queue->_isEmpty = false;

		// and enqueue
		_enqueue(queue, message);
		return UART_QUEUE_OKAY;
	}

	// case that queue is not empty
	else {
		// case that queue is full
		if (queue->_front == queue->_rear)
			return UART_QUEUE_FULL;

		// queue is not full, enqueue
		_enqueue(queue, message);
		return UART_QUEUE_OKAY;
	}
}

/*
 * Dequeue a message.
 * Return success and message if message present, or empty and null if queue
 * is empty.
 */
UART_QUEUE_STATUS uartQueue_dequeue(UART_Queue* queue, char messageBuffer[QUEUE_BUFFER_SIZE]) {
	// operation variables

	// case that queue is empty
	if (queue->_isEmpty) {
		return UART_QUEUE_EMPTY;
	}

	// case that queue is not empty
	else {
		_dequeue(queue, messageBuffer);
		return UART_QUEUE_OKAY;
	}
}
