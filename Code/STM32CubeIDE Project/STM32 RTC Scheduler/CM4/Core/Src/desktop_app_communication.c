/*
 * desktop_app_communication.c
 *
 *  Created on: Jun 24, 2023
 *      Author: kevinimlay
 */

#include "desktop_app_communication.h"


/*
 * Static operation variables
 *
 * There should be only one 'instance' of this communicator (if approaching this like
 * OOP) so it is sufficient to make these variables static to the file rather than a
 * structure that is passed in as a parameter.
 */
static bool _isInit = false;
static volatile bool _txError = false;
static volatile bool _rxError = false;
static UART_HandleTypeDef* _uartHandle = NULL;
static UART_Queue _process_queue;
static UART_Queue _report_queue;
static char _txBuffer[UART_MESSAGE_SIZE] = {0};
static char _rxBuffer[UART_MESSAGE_SIZE] = {0};


/*  Private Function Prototypes */
void _txMessage_IT(void);
void _rxMessage_IT(void);
void _txMessage_Poll(void);
void _rxMessage_Poll(void);


/* Usage Functions for application interface (API) */


/*
 *
 */
void initDesktopCommunication(UART_HandleTypeDef* uartHandle) {
	// initialize structure fields
	_uartHandle = uartHandle;
	uartQueue_init(&(_process_queue));
	uartQueue_init(&(_report_queue));

	// set isInit flag
	_isInit = true;
}


/*
 *
 */
REPORT_QUEUE_STATUS reportToDesktopApp(char header[UART_MESSAGE_HEADER_SIZE],
		char body[UART_MESSAGE_BODY_SIZE])
{
	UART_QUEUE_STATUS queueStatus;
	char message[UART_MESSAGE_SIZE];

	// Compose header and body into one message
	composeMessage(header, body, message);

	// disable IRQs from UART to prevent race condition
	if (_uartHandle->Instance == USART1)
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	else if (_uartHandle->Instance == USART2)
		HAL_NVIC_DisableIRQ(USART2_IRQn);

	// queue into report queue
	queueStatus = uartQueue_enqueue(&(_report_queue), message);

	// enable UART IRQs
	if (_uartHandle->Instance == USART1)
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	else if (_uartHandle->Instance == USART2)
		HAL_NVIC_EnableIRQ(USART2_IRQn);

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
PROCESS_QUEUE_STATUS retrieveFromDesktopApp(char header[UART_MESSAGE_HEADER_SIZE],
		char body[UART_MESSAGE_BODY_SIZE])
{
	UART_QUEUE_STATUS queueStatus;
	char message[UART_MESSAGE_SIZE];

	// disable IRQs from UART to prevent race condition
	if (_uartHandle->Instance == USART1)
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	else if (_uartHandle->Instance == USART2)
		HAL_NVIC_DisableIRQ(USART2_IRQn);

	// retrieve message from process queue
	queueStatus = uartQueue_dequeue(&(_process_queue), message);

	// enable UART IRQs
	if (_uartHandle->Instance == USART1)
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	else if (_uartHandle->Instance == USART2)
		HAL_NVIC_EnableIRQ(USART2_IRQn);

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
bool checkRxTxError(void) {
	return (_rxError || _txError);
}


/*
 *
 */
void flushReportQueue(void) {
	// while the report queue is not empty
	while (!uartQueue_isEmpty(&_report_queue))
	{
		// pop message from report queue and place in tx buffer
		uartQueue_dequeue(&_report_queue, _txBuffer);

		// send message to computer
		_txMessage_Poll();
	}
}


/*
 *
 */
void startDesktopAppCommunication(void) {
	_rxMessage_IT();
}


/* ISR Functions for handling UART interrupts */


/*
 *
 */
void deskAppRxCompleteISR(void) {
	// check if queue is not full
	if (!uartQueue_isFull(&_process_queue))
	{
		// add message to process queue
		uartQueue_enqueue(&_process_queue, _rxBuffer);
	}

	// queue is full, report to desktop application to pause transmissions
	else
	{
		// todo: replace with something better
		strncpy(_txBuffer, "\nBUFFER FULL!\n\n", UART_MESSAGE_SIZE);
		_txMessage_IT();
	}

	// begin receiving again
	_rxMessage_IT();
}


/*
 *
 */
void deskAppTxCompleteISR(void) {

}


/* Private Function Definitions */


/*
 *
 */
void _txMessage_IT(void) {
	if (HAL_UART_Transmit_IT(_uartHandle, (uint8_t*)_txBuffer, UART_MESSAGE_SIZE) == HAL_ERROR)
		_txError = true;
}


/*
 *
 */
void _rxMessage_IT(void) {
	if (HAL_UART_Receive_IT(_uartHandle, (uint8_t*)_rxBuffer, UART_MESSAGE_SIZE) == HAL_ERROR)
		_rxError = true;
}


/*
 *
 */
void _txMessage_Poll(void) {
	if (HAL_UART_Transmit(_uartHandle, (uint8_t*)_txBuffer, UART_MESSAGE_SIZE, TX_POLL_TIMEOUT) == HAL_ERROR)
		_txError = true;
}


/*
 *
 */
void _rxMessage_Poll(void) {
	if (HAL_UART_Receive(_uartHandle, (uint8_t*)_rxBuffer, UART_MESSAGE_SIZE, RX_POLL_TIMEOUT) == HAL_ERROR)
		_rxError = true;
}
