/*
 * calendar.h
 *
 *  Created on: Jul 7, 2023
 *      Author: kevinimlay
 */

#ifndef INC_CALENDAR_H_
#define INC_CALENDAR_H_


#include <stdint.h>
#include "stm32wlxx_hal.h"

#include "uart_message.h"
#include "uart_basic_com.h"


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
  void* start_callback;
  DateTime end;
} CalendarEvent;


/*
 *
 */
void calendar_init(RTC_HandleTypeDef* hrtc);


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
void calendar_setEvents(CalendarEvent events[MAX_NUM_EVENTS], unsigned int numEvents);


/*
 *
 */
void calendar_start(void);


/*
 *
 */
void calendar_handleAlarm(void);


/*
 *
 */
void calendar_AlarmA_ISR(void);


#endif /* INC_CALENDAR_H_ */
