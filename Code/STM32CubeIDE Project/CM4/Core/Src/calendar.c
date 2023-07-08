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

	// pass pointer to alarm control
	rtcAlarmControl_init(hrtc);
}


/*
 *
 */
void calendar_start(void) {
	// reset current event index
	_currentEventIdx = 0;

	// set alarm for start of first event in list
	setAlarm_A(_calendarEvents[0].start_day, _calendarEvents[0].start_hour, _calendarEvents[0].start_minute, _calendarEvents[0].start_second);
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
void calendar_setEvents(CalendarEvent events[MAX_NUM_EVENTS]) {
	memcpy(_calendarEvents, events, sizeof(CalendarEvent)*MAX_NUM_EVENTS);
}


/*
 *
 */
void calendar_AlarmA_ISR(void) {

}
