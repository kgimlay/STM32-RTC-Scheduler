/*
 * uart_communication.h
 *
 *  Created on: Jun 24, 2023
 *      Author: kevinimlay
 */

#ifndef INC_DESKTOP_APP_COMMUNICATION_H_
#define INC_DESKTOP_APP_COMMUNICATION_H_

#include "uart_message.h"
#include "uart_queue.h"
#include "stm32wlxx_hal.h"

/*
 * Public API for module.  It is encouraged that only these functions are used
 * for communication, as all standards for the uart communication with the
 * desktop application are implemented within.
 *
 * There are two main operations - send to desktop application and receive from
 * Desktop application.  These are labeled as report and process respectively
 * since the messages in the rx queue from the desktop are data to be processed
 * by the application, and the messages in the tx queue are reports from the
 * MCU on the state of operation or data collected.
 *
 * Each message is made up of a header and a body where the header denotes how
 * to handle the data in the body.  A simple example of this is a message
 * received from the desktop application that asks the MCU to echo the message
 * back.  In this case the header would have the echo command and the body is
 * what is echoed back to the desktop.  The MCU then needs to replace the echo
 * command with an appropriate header when reporting to the computer.
 *
 * Messages are queued for the sake of allowing the MCU to continue any tasks it
 * is operating on until it has reached a stopping point and can handle the new
 * command.  Similarly, this buffer in communication allows the MCU to continue
 * operation and queue messages without the need to wait for the desktop
 * application to be listening, which would pose as a bottleneck for the MCU's
 * operation.
 */

/*
 * Definitions for message parameters and queues
 */
#define PROCESS_QUEUE_SIZE QUEUE_SIZE
#define REPORT_QUEUE_SIZE QUEUE_SIZE

/*
 * Structure to hold process and report queues (rx and tx respectively)
 * as well as operaiton variables
 */
typedef struct
{
	UART_HandleTypeDef* _uartHandle;
	UART_Queue _process_queue;
	UART_Queue _report_queue;
	char _txBuffer[UART_MESSAGE_SIZE];
	char _rxBuffer[UART_MESSAGE_SIZE];
} DESKTOP_COM;

/*
 * Status codes for report queue.
 */
typedef enum {
	REPORT_QUEUED,
	REPORT_FULL
} REPORT_QUEUE_STATUS;

/*
 * Status codes for process queue.
 */
typedef enum {
	PROCESS_DEQUEUED,
	PROCESS_EMPTY
} PROCESS_QUEUE_STATUS;

/*
 * Initializes the DESKTOP_COM structure.
 */
void initDesktopCommunication(DESKTOP_COM* desktopComStruct, UART_HandleTypeDef* uartHandle);

/*
 * Report to the desktop application.
 * Essentially, enqueues a message to be sent to the desktop application at the
 * next possible convenience.
 *
 * Returns REPORT_QUEUE_STATUS:
 * 		REPORT_QUEUED if the message was successfully queued, or
 * 		REPORT_FULL if the queue is full and the application needs to wait for
 * 			it to empty.
 */
REPORT_QUEUE_STATUS reportToDesktopApp(DESKTOP_COM* desktopComStruct,
		char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE]);

/*
 * Check for a message from the desktop application to process.
 * Essentially, dequeues the first unread message from the desktop application.
 *
 * Returns PROCESS_QUEUE_STATUS:
 * 		PROCESS_DEQUEUED if a message was present from the desktop application, or
 * 		PROCESS_EMPTY if there were no messages present.
 */
PROCESS_QUEUE_STATUS retrieveFromDesktopApp(DESKTOP_COM* desktopComStruct,
		char header[UART_MESSAGE_HEADER_SIZE], char body[UART_MESSAGE_BODY_SIZE]);

/*
 * ISR function for handling UART receive complete (RX).
 */
void deskAppRxCompleteISR();

/*
 * ISR function for handling UART transmit complete (TX).
 */
void deskAppTxCompleteISR();

#endif /* INC_DESKTOP_APP_COMMUNICATION_H_ */
