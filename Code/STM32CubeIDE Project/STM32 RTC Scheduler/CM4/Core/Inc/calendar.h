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
  uint8_t start_year;
  uint8_t start_month;
  uint8_t start_day;
  uint8_t start_hour;
  uint8_t start_minute;
  uint8_t start_second;
  void* start_callback;
  uint8_t end_year;
  uint8_t end_month;
  uint8_t end_day;
  uint8_t end_hour;
  uint8_t end_minute;
  uint8_t end_second;
  void* end_callback;
} CalendarEvent;


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
void setEvents(CalendarEvent events[MAX_NUM_EVENTS]);


/*
 *
 */
void calendar_AlarmA_ISR(void);


#endif /* INC_CALENDAR_H_ */
