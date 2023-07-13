/*
 * calendar.c
 *
 *  Created on: Jul 7, 2023
 *      Author: kevinimlay
 */


#include "calendar.h"
#include "rtc_alarm_control.h"
#include "stdbool.h"
#include "string.h"

int compareDateTime(DateTime dateTime_1, DateTime dateTime_2);
int dateTimeToSeconds(DateTime dateTime);


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
	setAlarm_A(_calendarEvents[0].start.day, _calendarEvents[0].start.hour, _calendarEvents[0].start.minute, _calendarEvents[0].start.second);
}



/*
 *
 */
void calendar_updateEvents(void) {

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

		// if alarm triggered at correct month/year
		// this line can break if it takes 1 or more seconds from when the alarm interrupt fires
		if (compareDateTime(now, _calendarEvents[_currentEventIdx].start) == 0) {

			_inEvent = true;

			// set alarm for end of event
			setAlarm_A(_calendarEvents[_currentEventIdx].end.day, _calendarEvents[_currentEventIdx].end.hour, _calendarEvents[_currentEventIdx].end.minute, _calendarEvents[_currentEventIdx].end.second);
		}

		// else not at start of event, restart alarm
		else {
			setAlarm_A(_calendarEvents[_currentEventIdx].start.day, _calendarEvents[_currentEventIdx].start.hour, _calendarEvents[_currentEventIdx].start.minute, _calendarEvents[_currentEventIdx].start.second);
		}
	}

	// if in event
	else {
		// if alarm triggered at correct month/year
		// this line can break if it takes 1 or more seconds from when the alarm interrupt fires
		if (compareDateTime(now, _calendarEvents[_currentEventIdx].end) == 0) {
			_currentEventIdx++;

			// set alarm for start of next event
			setAlarm_A(_calendarEvents[_currentEventIdx].start.day, _calendarEvents[_currentEventIdx].start.hour, _calendarEvents[_currentEventIdx].start.minute, _calendarEvents[_currentEventIdx].start.second);

			_inEvent = false;
		}

		// else not at start of event, restart alarm
		else {
			setAlarm_A(_calendarEvents[_currentEventIdx].end.day, _calendarEvents[_currentEventIdx].end.hour, _calendarEvents[_currentEventIdx].end.minute, _calendarEvents[_currentEventIdx].end.second);
		}
	}
}


/* Find the time difference in seconds of dateTime1 and dateTime2.
 *
 * Note: Does not account for leap years.
 */
int compareDateTime(DateTime dateTime_1, DateTime dateTime_2) {
  // return net comparison (date time 1 - date time 2)
  return dateTimeToSeconds(dateTime_1) - dateTimeToSeconds(dateTime_2);
}


/*
 *
 */
int dateTimeToSeconds(DateTime dateTime) {
	// Convert to seconds. Note: assumes 30 days in a month and
	// no leap years, it is not needed for the calculation because
	// they are used for relative comparisons, not absolute values.
	return (dateTime.second
			+ (dateTime.minute * 60)
			+ (dateTime.hour * 3600)
			+ ((dateTime.day - 1) * 86400)
			+ ((dateTime.month - 1) * 2592000)
			+ (dateTime.year * 31104000));
}
