/*
 * Author:  Kevin Imlay
 * Date:  September, 2023
 *
 * Purpose:
 *		A
 */

#ifndef INC_DESKTOP_APP_COMMANDS_H_
#define INC_DESKTOP_APP_COMMANDS_H_


#include "calendar.h"

/*
 *
 */
#define NO_CODE 					"\0\0\0\0\0"
#define SET_CALENDAR_DATETIME_CODE 	"STDT\0"
#define GET_CALENDAR_DATETIME_CODE 	"GTDT\0"
#define ADD_CALENDAR_EVENT_CODE 	"AEVT\0"
#define GET_CALENDAR_EVENT_CODE 	"GEVT\0"
#define REMOVE_CALENDAR_EVENT_CODE 	"REVT\0"
#define CLEAR_CALENDAR_EVENTS_CODE 	"CEVT\0"
#define START_CALENDAR_CODE 		"SCAL\0"

/*
 *
 */
typedef enum AppActions {
	NO_ACTION = 0,
	SET_CALENDAR_DATETIME,
	GET_CALENDAR_DATETIME,
	ADD_CALENDAR_EVENT,
	GET_CALENDAR_EVENT,
	REMOVE_CALENDAR_EVENT,
	CLEAR_CALENDAR_EVENTS,
	START_CALENDAR
} AppActions;

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
AppActions code_to_appActions(char* code);

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
const char* appActions_to_code(AppActions action);

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
void parseDateTime(struct_DateTime* dateTime, char data[UART_PACKET_PAYLOAD_SIZE]);

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
void formatDateTime(char data[UART_PACKET_PAYLOAD_SIZE], struct_DateTime* dateTime);

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
void parseEvent(CalendarEvent* event, char data[UART_PACKET_PAYLOAD_SIZE]);

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
void formatEvent(char data[UART_PACKET_PAYLOAD_SIZE], CalendarEvent* event);


#endif /* INC_DESKTOP_APP_COMMANDS_H_ */
