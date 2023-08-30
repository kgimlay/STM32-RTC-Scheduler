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
	if (!strncmp(code, SET_CALENDAR_DATETIME_CODE, UART_MESSAGE_HEADER_SIZE))
		return SET_CALENDAR_DATETIME;

	else if (!strncmp(code, GET_CALENDAR_DATETIME_CODE, UART_MESSAGE_HEADER_SIZE))
		return GET_CALENDAR_DATETIME;

	else
		return NO_ACTION;
}


/*
 *
 */
char* appActions_to_code(AppActions action)
{
	if (SET_CALENDAR_DATETIME)
		return SET_CALENDAR_DATETIME_CODE;

	else if (GET_CALENDAR_DATETIME)
		return GET_CALENDAR_DATETIME_CODE;

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
