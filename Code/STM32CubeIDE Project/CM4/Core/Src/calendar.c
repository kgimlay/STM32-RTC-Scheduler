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


#define CURRENTLY_IN_EVENT (_currentEvent != -1)


int32_t compareDateTime(DateTime dateTime_1, DateTime dateTime_2);
uint32_t dateTimeToSeconds(DateTime dateTime);
bool getNextAlarm(DateTime* dateTime, int* nowEventIdx, bool* inEvent);


/*
 *
 */
static CalendarEvent _calendarEvents[MAX_NUM_EVENTS] = {0};
static int _numberEvents = 0;
static int _currentEvent = -1;
volatile static bool _alarmAFired = false;
RTC_HandleTypeDef* _hrtc = NULL;


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
void calendar_setEvents(CalendarEvent events[MAX_NUM_EVENTS], unsigned int numEvents) {
	memcpy(_calendarEvents, events, sizeof(CalendarEvent)*MAX_NUM_EVENTS);
	_numberEvents = numEvents;
}


/*
 *
 */
void calendar_start(void) {
	DateTime nextAlarm;
	int currentEventIdx;
	bool withinEvent;

	// get calendar alarm for next alarm in event list relative to now
	if (getNextAlarm(&nextAlarm, &currentEventIdx, &withinEvent)) {
		// set alarm for next event transition (start or end of event)
		setAlarm_A(nextAlarm.day, nextAlarm.hour, nextAlarm.minute, nextAlarm.second);

		// if starting within an event, run the start callback
		if (withinEvent) {
			if (_calendarEvents[currentEventIdx].start_callback != NULL)
				(*_calendarEvents[currentEventIdx].start_callback)();
			_currentEvent = currentEventIdx;
		}

		// make sure that alarm fired is cleared/reset
		_alarmAFired = false;
	}

	// if there is no alarm to set, disable the alarm
	else {
		diableAlarm_A();
	}
}


/*
 *
 */
void calendar_handleAlarm(void) {
	DateTime nextAlarm;
	int currentEventIdx;
	bool withinEvent;

	if (_alarmAFired) {
		// send message for debugging
		char messageBody[UART_MESSAGE_BODY_SIZE] = "\nALARM EVENT!\n\n\0";
		uartBasic_TX_Poll("\0\0\0\0", messageBody);

		// get calendar alarm for next alarm in event list relative to now
		if (getNextAlarm(&nextAlarm, &currentEventIdx, &withinEvent)) {
			// set alarm for next event transition (start or end of event)
			setAlarm_A(nextAlarm.day, nextAlarm.hour, nextAlarm.minute, nextAlarm.second);

			// if the current event has changed (event ended or began), then run appropriate
			// callback functions

			// if entering an event from no event
			if (withinEvent && !CURRENTLY_IN_EVENT) {

				// call start event callback
				if (_calendarEvents[currentEventIdx].start_callback != NULL)
					(*_calendarEvents[currentEventIdx].start_callback)();

				// update current event
				_currentEvent = currentEventIdx;
			}

			// if entering an event from another event
			else if (withinEvent && CURRENTLY_IN_EVENT) {
				// call end event callback for event just left
				if (_calendarEvents[_currentEvent].end_callback != NULL)
					(*_calendarEvents[_currentEvent].end_callback)();

				// call start event callback for event just entered
				if (_calendarEvents[currentEventIdx].start_callback != NULL)
					(*_calendarEvents[currentEventIdx].start_callback)();

				// update current event
				_currentEvent = currentEventIdx;

			}

			// if exiting an event into no event
			else if (!withinEvent && CURRENTLY_IN_EVENT) {
				// call end event callback for event just left
				if (_calendarEvents[_currentEvent].end_callback != NULL)
					(*_calendarEvents[_currentEvent].end_callback)();

				// update current event
				_currentEvent = currentEventIdx;

			}

			// else, alarm is just being reset for next month/year

		}

		// if there is no alarm to set, disable the alarm and exit any events
		else {
			diableAlarm_A();

			if (CURRENTLY_IN_EVENT) {
				// call end event callback for event just left
				if (_calendarEvents[_currentEvent].end_callback != NULL)
					(*_calendarEvents[_currentEvent].end_callback)();
			}
		}

		// reset alarm fired flag
		_alarmAFired = false;
	}

	else {

	}
}


/*
 *
 */
void calendar_AlarmA_ISR(void) {
	// set flag that an alarm fired
	_alarmAFired = true;
}


/* Find the time difference in seconds of dateTime1 and dateTime2.
 *
 * Note: Does not account for leap years.
 */
int32_t compareDateTime(DateTime dateTime_1, DateTime dateTime_2) {
	uint32_t dateTimeSeconds_1, dateTimeSeconds_2;

	dateTimeSeconds_1 = dateTimeToSeconds(dateTime_1);
	dateTimeSeconds_2 = dateTimeToSeconds(dateTime_2);

	// return net comparison (date time 1 - date time 2)
	return dateTimeSeconds_1 - dateTimeSeconds_2;
}


/*
 *
 */
uint32_t dateTimeToSeconds(DateTime dateTime) {
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


/*
 *
 */
bool getNextAlarm(DateTime* dateTime, int* nowEventIdx, bool* inEvent) {
	int eventIdx = 0;
	int currentIdx = 0;
	bool nextAlarmFound = false;
	DateTime now = {0};
	DateTime nextAlarmDateTime = {0};

	// get the current date and time
	getDateTime(&now.year, &now.month, &now.day, &now.hour, &now.minute, &now.second);

	// Traverse over the events list and find where 'now' falls.  This can be before
	// any all the events, within an event, between events, or after all the events.
	eventIdx = 0;
	currentIdx = eventIdx - 1;
	nextAlarmFound = false;
	while (eventIdx < _numberEvents && !nextAlarmFound) {
		// test if before event
		if (compareDateTime(now, _calendarEvents[eventIdx].start) < 0)
		{
			// then the next alarm is the beginning of this event
			nextAlarmDateTime.year = _calendarEvents[eventIdx].start.year;
			nextAlarmDateTime.month = _calendarEvents[eventIdx].start.month;
			nextAlarmDateTime.day = _calendarEvents[eventIdx].start.day;
			nextAlarmDateTime.hour = _calendarEvents[eventIdx].start.hour;
			nextAlarmDateTime.minute = _calendarEvents[eventIdx].start.minute;
			nextAlarmDateTime.second = _calendarEvents[eventIdx].start.second;

			// set found
			nextAlarmFound = true;
			currentIdx = eventIdx - 1;
			*inEvent = false;
		}

		// test if within event
		else if(compareDateTime(now, _calendarEvents[eventIdx].start) >= 0
				&& compareDateTime(now, _calendarEvents[eventIdx].end) < 0) {
			// then the next alarm is the end of this event
			nextAlarmDateTime.year = _calendarEvents[eventIdx].end.year;
			nextAlarmDateTime.month = _calendarEvents[eventIdx].end.month;
			nextAlarmDateTime.day = _calendarEvents[eventIdx].end.day;
			nextAlarmDateTime.hour = _calendarEvents[eventIdx].end.hour;
			nextAlarmDateTime.minute = _calendarEvents[eventIdx].end.minute;
			nextAlarmDateTime.second = _calendarEvents[eventIdx].end.second;

			// set found
			nextAlarmFound = true;
			currentIdx = eventIdx;
			*inEvent = true;
		}

		// traverse to next event
		else {
			eventIdx++;
		}
	}

	// If there is no next alarm, then return no alarm
	if (!nextAlarmFound) {
		dateTime = NULL;
		*nowEventIdx = -1;
		return false;
	}

	// Return the next alarm found.
	else {
		*dateTime = nextAlarmDateTime;
		*nowEventIdx = currentIdx;
		return true;
	}
}
