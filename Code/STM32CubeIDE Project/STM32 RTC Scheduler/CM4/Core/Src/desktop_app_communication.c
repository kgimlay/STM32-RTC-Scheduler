/*
 * desktop_app_communication.c
 *
 *  Created on: Jun 24, 2023
 *      Author: kevinimlay
 */

#include "desktop_app_communication.h"


/*
 *
 */
void initDesktopCommunication(DESKTOP_COM* desktopComStruct, UART_HandleTypeDef* uartHandle) {
	// initialize structure fields
	desktopComStruct->_uartHandle = uartHandle;
	uartQueue_init(&(desktopComStruct->_process_queue));
	uartQueue_init(&(desktopComStruct->_report_queue));
}


/*
 *
 */
REPORT_QUEUE_STATUS reportToDesktopApp(DESKTOP_COM* desktopComStruct,
		char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE])
{
	UART_QUEUE_STATUS queueStatus;
	char message[UART_MESSAGE_SIZE];

	// Compose header and body into one message
	composeMessage(header, body, message);

	// queue into report queue
	queueStatus = uartQueue_enqueue(&(desktopComStruct->_report_queue), message);

	// report status of queue operation
	if (queueStatus == UART_QUEUE_FULL)
	{
		return REPORT_FULL;}
	else
	{
		return REPORT_QUEUED;
	}
}


/*
 *
 */
PROCESS_QUEUE_STATUS retrieveFromDesktopApp(DESKTOP_COM* desktopComStruct,
		char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE])
{
	UART_QUEUE_STATUS queueStatus;
	char message[UART_MESSAGE_SIZE];

	// retrieve message from process queue
	queueStatus = uartQueue_dequeue(&(desktopComStruct->_process_queue), message);

	// check that queue wasn't empty
	if (queueStatus == UART_QUEUE_EMPTY)
	{
		return PROCESS_EMPTY;
	}
	else
	{
		// decompose header and body from message
		decomposeMessage(header, body, message);

		// report status
		return PROCESS_DEQUEUED;
	}
}


/*
 *
 */
void deskAppRxCompleteISR() {
	UART_QUEUE_STATUS queueStatus;

	// add message to process queue
	queueStatus = uartQueue_enqueue(&rxQueue, rxBuffer);

	// begin receiving again
	if(HAL_UART_Receive_IT(&huart2, (uint8_t*)rxBuffer, RXBUFFERSIZE)== HAL_ERROR)
	{
	  /* Transfer error in transmission process */
	  Error_Handler();
	}
}


/*
 *
 */
void deskAppTxCompleteISR() {

}
