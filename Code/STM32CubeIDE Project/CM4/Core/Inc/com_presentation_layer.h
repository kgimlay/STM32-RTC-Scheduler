/*
 * com_presentation_layer.h
 *
 *  Created on: Aug 28, 2023
 *      Author: kevinimlay
 */

#ifndef INC_COM_PRESENTATION_LAYER_H_
#define INC_COM_PRESENTATION_LAYER_H_


#include "calendar.h"


#define NO_CODE "\0\0\0\0"
#define SET_CALENDAR_DATETIME_CODE "STDT\0"
#define GET_CALENDAR_DATETIME_CODE "GTDT\0"
#define ADD_CALENDAR_EVENT_CODE "AEVT\0"
#define GET_CALENDAR_EVENT_CODE "GEVT\0"
#define REMOVE_CALENDAR_EVENT_CODE "REVT\0"
#define CLEAR_CALENDAR_EVENTS_CODE "CEVT\0"
#define START_CALENDAR_CODE "SCAL\0"


typedef enum {
	NO_ACTION,
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
 */
AppActions code_to_appActions(char* code);

/*
 *
 */
char* appActions_to_code(AppActions action);

/*
 *
 */
void parseDateTime(char data[UART_MESSAGE_BODY_SIZE], DateTime* dateTime);

/*
 *
 */
void formatDateTime(char data[UART_MESSAGE_BODY_SIZE], DateTime* dateTime);

/*
 *
 */
void parseEvent(char data[UART_MESSAGE_BODY_SIZE], CalendarEvent* event);


#endif /* INC_COM_PRESENTATION_LAYER_H_ */
