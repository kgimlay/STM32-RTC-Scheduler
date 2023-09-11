/*
 * com_presentation_layer.c
 *
 *  Created on: Aug 28, 2023
 *      Author: kevinimlay
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "com_presentation_layer.h"
#include "com_datalink_layer.h"


/*
 *
 */
AppActions code_to_appActions(char* code)
{
	// set calendar date/time
	if (!strncmp(code, SET_CALENDAR_DATETIME_CODE, UART_MESSAGE_HEADER_SIZE))
		return SET_CALENDAR_DATETIME;

	// get calendar date/time
	else if (!strncmp(code, GET_CALENDAR_DATETIME_CODE, UART_MESSAGE_HEADER_SIZE))
		return GET_CALENDAR_DATETIME;

	// add calendar event
	else if (!strncmp(code, ADD_CALENDAR_EVENT_CODE, UART_MESSAGE_HEADER_SIZE))
			return ADD_CALENDAR_EVENT;

	// get/view calendar event
	else if (!strncmp(code, GET_CALENDAR_EVENT_CODE, UART_MESSAGE_HEADER_SIZE))
			return GET_CALENDAR_EVENT;

	// remove calendar event
	else if (!strncmp(code, REMOVE_CALENDAR_EVENT_CODE, UART_MESSAGE_HEADER_SIZE))
			return REMOVE_CALENDAR_EVENT;

	// clear all calendar events
	else if (!strncmp(code, CLEAR_CALENDAR_EVENTS_CODE, UART_MESSAGE_HEADER_SIZE))
			return CLEAR_CALENDAR_EVENTS;

	// start calendar
	else if (!strncmp(code, START_CALENDAR_CODE, UART_MESSAGE_HEADER_SIZE))
			return START_CALENDAR;

	// invalid code
	else
		return NO_ACTION;
}


/*
 *
 */
char* appActions_to_code(AppActions action)
{
	// set date/time
	if (SET_CALENDAR_DATETIME)
		return SET_CALENDAR_DATETIME_CODE;

	// get date/time
	else if (GET_CALENDAR_DATETIME)
		return GET_CALENDAR_DATETIME_CODE;

	// add calendar event
	else if (ADD_CALENDAR_EVENT)
			return ADD_CALENDAR_EVENT_CODE;

	// get/view calendar event
	else if (GET_CALENDAR_EVENT)
			return GET_CALENDAR_EVENT_CODE;

	// remove calendar event
	else if (REMOVE_CALENDAR_EVENT)
			return REMOVE_CALENDAR_EVENT_CODE;

	// clear all calendar events
	else if (CLEAR_CALENDAR_EVENTS)
			return CLEAR_CALENDAR_EVENTS_CODE;

	// start calendar
	else if (START_CALENDAR)
			return START_CALENDAR_CODE;

	// invalid code
	else
		return NO_CODE;
}


/*
 *
 */
void parseDateTime(char data[UART_MESSAGE_BODY_SIZE], DateTime* dateTime)
{
	// parse year
	dateTime->year = atoi(strtok(data, ";"));

	// parse month
	dateTime->month = atoi(strtok(NULL, ";"));

	// parse date
	dateTime->day = atoi(strtok(NULL, ";"));

	// parse hour
	dateTime->hour = atoi(strtok(NULL, ";"));

	// parse minute
	dateTime->minute = atoi(strtok(NULL, ";"));

	// parse second
	dateTime->second = atoi(strtok(NULL, ";"));
}


/*
 *
 */
void formatDateTime(char data[UART_MESSAGE_BODY_SIZE], DateTime* dateTime)
{
	memset(data, 0, UART_MESSAGE_BODY_SIZE*sizeof(char));
	snprintf(data, UART_MESSAGE_BODY_SIZE, "%d;%d;%d;%d;%d;%d", dateTime->year, dateTime->month, dateTime->day,
			dateTime->hour, dateTime->minute, dateTime->second);
}


/*
 *
 */
void parseEvent(char data[UART_MESSAGE_BODY_SIZE], CalendarEvent* event)
{
	// parse start year
	event->start.year = atoi(strtok(data, ";"));

	// parse start month
	event->start.month = atoi(strtok(NULL, ";"));

	// parse start date
	event->start.day = atoi(strtok(NULL, ";"));

	// parse start hour
	event->start.hour = atoi(strtok(NULL, ";"));

	// parse start minute
	event->start.minute = atoi(strtok(NULL, ";"));

	// parse start second
	event->start.second = atoi(strtok(NULL, ";"));

	// parse end year
	event->end.year = atoi(strtok(NULL, ";"));

	// parse end month
	event->end.month = atoi(strtok(NULL, ";"));

	// parse end date
	event->end.day = atoi(strtok(NULL, ";"));

	// parse end hour
	event->end.hour = atoi(strtok(NULL, ";"));

	// parse end minute
	event->end.minute = atoi(strtok(NULL, ";"));

	// parse end second
	event->end.second = atoi(strtok(NULL, ";"));
}
