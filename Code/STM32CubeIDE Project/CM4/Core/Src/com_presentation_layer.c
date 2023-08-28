/*
 * com_presentation_layer.c
 *
 *  Created on: Aug 28, 2023
 *      Author: kevinimlay
 */


#include <string.h>
#include "com_presentation_layer.h"
#include "com_datalink_layer.h"


/*
 *
 */
AppActionsMapping code_to_appActionsMapping(char* code)
{
	AppActionsMapping tempAction = NO_ACTION;

	if (!strncmp(code, SET_CALENDAR_DATETIME_CODE, UART_MESSAGE_HEADER_SIZE))
		tempAction = SET_CALENDAR_DATETIME;

	else if (!strncmp(code, GET_CALENDAR_DATETIME_CODE, UART_MESSAGE_HEADER_SIZE))
		tempAction = GET_CALENDAR_DATETIME;

	return tempAction;
}
