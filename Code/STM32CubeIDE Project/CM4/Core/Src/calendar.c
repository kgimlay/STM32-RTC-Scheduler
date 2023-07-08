/*
 * calendar.c
 *
 *  Created on: Jul 7, 2023
 *      Author: kevinimlay
 */


#include "calendar.h"
#include "rtc_alarm_control.h"
#include "stdbool.h"


/*
 *
 */
static CalendarEvent _calendarEvents[MAX_NUM_EVENTS] = {0};
volatile static int _currentEventIdx = 0;
volatile static bool _inEvent = false;
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
void calendar_start(void) {
	// reset current event index
	_currentEventIdx = 0;

	// set alarm for start of first event in list
	setAlarm_A(_calendarEvents[0].start_day, _calendarEvents[0].start_hour, _calendarEvents[0].start_minute, _calendarEvents[0].start_second);
}


/*
 *
 */
void calendar_AlarmA_ISR(void) {
	DateTime now;

	// get the current time
	getDateTime(&now.year, &now.month, &now.day, &now.month, &now.minute, &now.second);

	// if not in event, event started
	if (!_inEvent) {
		_inEvent = true;

		// set alarm for end of event
		setAlarm_A(_calendarEvents[_currentEventIdx].end_day, _calendarEvents[_currentEventIdx].end_hour, _calendarEvents[_currentEventIdx].end_minute, _calendarEvents[_currentEventIdx].end_second);
	}

	// if in event
	else {
		_inEvent = false;

		_currentEventIdx++;

		// set alarm for start of next event
		setAlarm_A(_calendarEvents[_currentEventIdx].start_day, _calendarEvents[_currentEventIdx].start_hour, _calendarEvents[_currentEventIdx].start_minute, _calendarEvents[_currentEventIdx].start_second);
	}
}
