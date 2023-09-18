/*
 * Author:  Kevin Imlay
 * Date:  September, 2023
 *
 * Purpose:
 *		A
 */

#ifndef INC_DESKTOP_APP_SESSION_LAYER_H_
#define INC_DESKTOP_APP_SESSION_LAYER_H_


#include <desktop_app_commands.h>
#include <stdbool.h>
#include <uart_packet_helpers.h>
#include <uart_transport_layer.h>


/*
 *
 */
#define RECEIVE_TIMEOUT_MS 250
#define SEND_TIMEOUT_MS 100
#define SESSION_START_TIMEOUT_MS 1000

/*
 *
 */
#define HANDSHAKE_HEADER_SYNC "SYNC\0"
#define HANDSHAKE_HEADER_ACKN "ACKN\0"
#define HANDSHAKE_HEADER_SYNACK "SYNA\0"
#define HANDSHAKE_HEADER_DISC "DISC\0"
#define CTS_HEADER "CTS\0\0"
#define ECHO_HEADER "ECHO\0"

/*
 *
 */
typedef enum {
	SESSION_OKAY,
	SESSION_TIMEOUT,
	SESSION_ERROR,
	SESSION_NOT_INIT,
	SESSION_NOT_OPEN,
	SESSION_BUSY
} DesktopComSessionStatus;


/*
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
bool desktopAppSession_init(UART_HandleTypeDef* huart);

/*
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
DesktopComSessionStatus desktopAppSession_start(void);

/*
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
DesktopComSessionStatus desktopAppSession_pause(void);

/*
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
DesktopComSessionStatus desktopAppSession_update(void);

/*
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
DesktopComSessionStatus desktopAppSession_enqueueMessage(char header[UART_PACKET_HEADER_SIZE], char body[UART_PACKET_PAYLOAD_SIZE]);

/*
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
DesktopComSessionStatus desktopAppSession_dequeueMessage(char header[UART_PACKET_HEADER_SIZE], char body[UART_PACKET_PAYLOAD_SIZE]);


#endif /* INC_DESKTOP_APP_SESSION_LAYER_H_ */
