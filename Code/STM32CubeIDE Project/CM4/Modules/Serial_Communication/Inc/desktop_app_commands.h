/*
 * Author:  Kevin Imlay
 * Date:  September, 2023
 *
 * Purpose:
 *		Helper functions for connecting MCU's main application with the desktop
 *	application through messages passed by the Desktop Application Communication
 *	Session Manager (desktop_app_session).  Provides mappings of numerical codes
 *	(enumeration), that can be easily used in flow control without string
 *	comparisons, to message header code strings.  Also provides helper functions
 *	for formatting and parsing the body (or payload) of messages.
 */

#ifndef INC_DESKTOP_APP_COMMANDS_H_
#define INC_DESKTOP_APP_COMMANDS_H_


#include "calendar.h"
#include <stdbool.h>

/*
 * Message header codes to define actions that the MCU can perform.
 * String codes.
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
 * Numerical codes for easier flow control in MCU's main loop.
 * Mapped to message header codes through code_to_appActions() and
 * appActions_to_code().
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

/* code_to_appActions
 *
 * Function:
 *	Gives the appropriate numerical AppAction code for the given char
 *	array message header code.
 *
 * Parameters:
 *	code - char array pointer to header code.
 *
 * Return:
 *	AppActions - appropriate mapped code for the char array passed in,
 *			or NO_ACTION if the code is not defined.
 */
AppActions code_to_appActions(const char* code);

/* appActions_to_code
 *
 * Function:
 *	Gives the appropriate char array message header code for the given
 *	AppAction numerical code.
 *
 * Parameters:
 *	action - AppActions code.
 *
 * Return:
 *	char array pointer to string literal mapped to the AppAction passed
 *	in.
 */
const char* appActions_to_code(AppActions action);

/* parseDateTime
 *
 * Function:
 *	Parse a message body (payload) string into a DateTime structure received
 *	from the desktop application.
 *
 * Parameters:
 *	dateTime - DateTime pointer destination for parsing.
 *	date - char array pointer of string to parse.
 *
 * Return:
 * 	bool - true if parsing successful, otherwise false.
 *
 * Note:
 * 	No error checking is performed for properly formatted body string.
 * 	this is a point for future development using sscanf().
 */
bool parseDateTime(DateTime* dateTime, char data[UART_PACKET_PAYLOAD_SIZE]);

/* formatDateTime
 *
 * Function:
 *	Formats a DateTime structure into a message body (payload) for transmission
 *	to the desktop application.
 *
 * Parameters:
 *	data - char array pointer destination.
 *	dateTime - pointer to DateTime structure to format from.
 *
 * Return:
 * 	bool - true if formatting successful, otherwise false.
 *
 * Note:
 * 	No error checking is performed for properly formatted DateTime fields.
 */
bool formatDateTime(char data[UART_PACKET_PAYLOAD_SIZE], const DateTime* dateTime);

/* parseEvent
 *
 * Function:
 *	Parse a message body (payload) string into a CalendarEvent structure received
 *	from the desktop application.
 *
 * Parameters:
 *	event - CalendarEvent pointer destination.
 *	data - char array pointer of string to parse.
 *
 * Return:
 * 	bool - true if parsing successful, otherwise false.
 *
 * Note:
 * 	No error checking is performed for properly formatted body string.
 * 	this is a point for future development using sscanf().
 */
bool parseEvent(CalendarEvent* event, char data[UART_PACKET_PAYLOAD_SIZE]);

/* formatEvent
 *
 * Function:
 *	Formats a CalendarEvent structure into a message body (payload) for transmission
 *	to the desktop application.
 *
 * Parameters:
 *	data - char array pointer destination.
 *	event - CalendarEvent pointer to structure for format from.
 *
 * Return:
 * 	bool - true if formatting successful, otherwise false.
 *
 * Note:
 * 	Point of future development, not implemented.
 */
bool formatEvent(char data[UART_PACKET_PAYLOAD_SIZE], const CalendarEvent* event);


#endif /* INC_DESKTOP_APP_COMMANDS_H_ */
