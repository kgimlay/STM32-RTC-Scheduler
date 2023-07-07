/*
 * calendar.c
 *
 *  Created on: Jul 7, 2023
 *      Author: kevinimlay
 */


#include "calendar.h"
#include "rtc_alarm_control.h"


/*
 *
 */
static CalendarEvent _calendarEvents[MAX_NUM_EVENTS] = {0};
static int _currentEventIdx = 0;
RTC_HandleTypeDef* _hrtc;


/*
 *
 */
void calendar_init(RTC_HandleTypeDef* hrtc) {
	// store pointer to rtc structure
	_hrtc = hrtc;
}


/*
 *
 */
void calendar_setDateTime(DateTime dateTime) {
	setDateTime(dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second);
}


/*
 *
 */
void calendar_getDateTime(DateTime* dateTime) {
	getDateTime(&(dateTime->year), &(dateTime->month), &(dateTime->day), &(dateTime->hour), &(dateTime->minute), &(dateTime->second));
}


/*
 *
 */
void setEvents(CalendarEvent events[MAX_NUM_EVENTS]) {

}


/*
 *
 */
void updateCalendar(void) {

}
