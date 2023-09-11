/*
 * calendar.h
 *
 *  Created on: Jul 7, 2023
 *      Author: kevinimlay
 */

#ifndef INC_CALENDAR_H_
#define INC_CALENDAR_H_


#include <com_datalink_layer.h>
#include <stdint.h>
#include "stm32wlxx_hal.h"


#define MAX_NUM_EVENTS 10


/*
 *
 */
typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} DateTime;


/*
 *
 */
typedef struct {
  DateTime start;
  void (*start_callback)(void);
  DateTime end;
  void (*end_callback)(void);
} CalendarEvent;


/*
 *
 */
typedef enum {
	CALENDAR_OKAY = 0,
	CALENDER_NOT_INIT,
	CALENDAR_FULL,
	CALENDAR_EMPTY
} CalendarStatus;


/*
 *
 */
void calendar_init(RTC_HandleTypeDef* hrtc);


/*
 *
 */
void calendar_start(void);


/*
 *
 */
void calendar_setDateTime(DateTime dateTime);


/*
 *
 */
void calendar_getDateTime(DateTime* dateTime);


/*
 *
 */
CalendarStatus calendar_addEvent(CalendarEvent* event);


/*
 *
 */
void calendar_getEvent(DateTime* dateTime);


/*
 *
 */
void calendar_removeEvent(unsigned int index);


/*
 *
 */
void calendar_handleAlarm(void);


/*
 *
 */
void calendar_AlarmA_ISR(void);


#endif /* INC_CALENDAR_H_ */
