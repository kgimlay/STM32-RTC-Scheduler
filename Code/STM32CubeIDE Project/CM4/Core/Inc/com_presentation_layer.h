/*
 * com_presentation_layer.h
 *
 *  Created on: Aug 28, 2023
 *      Author: kevinimlay
 */

#ifndef INC_COM_PRESENTATION_LAYER_H_
#define INC_COM_PRESENTATION_LAYER_H_


#define SET_CALENDAR_DATETIME_CODE "STDT\0"
#define GET_CALENDAR_DATETIME_CODE "GTDT\0"


typedef enum {
	NO_ACTION,
	SET_CALENDAR_DATETIME,
	GET_CALENDAR_DATETIME
} AppActionsMapping;


AppActionsMapping code_to_appActionsMapping(char* code);


#endif /* INC_COM_PRESENTATION_LAYER_H_ */
