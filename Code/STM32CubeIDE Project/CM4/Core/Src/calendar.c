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


int32_t compareDateTime(DateTime dateTime_1, DateTime dateTime_2);
uint32_t dateTimeToSeconds(DateTime dateTime);
DateTime getNextAlarm(void);


/*
 *
 */
static CalendarEvent _calendarEvents[MAX_NUM_EVENTS] = {0};
volatile static int _numberEvents = 0;
volatile bool _alarmFired = false;
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

	// get calendar alarm for next alarm in event list relative to now
	nextAlarm = getNextAlarm();

	// set alarm for start of first event in list
	setAlarm_A(nextAlarm.day, nextAlarm.hour, nextAlarm.minute, nextAlarm.second);

	// make sure that alarm fired is cleared/reset
	_alarmFired = false;
}


/*
 *
 */
void calendar_handleAlarm(void) {
	DateTime nextAlarm;

	if (_alarmFired) {
		// get calendar alarm for next alarm in event list relative to now
		nextAlarm = getNextAlarm();

		// set alarm for start of first event in list
		setAlarm_A(nextAlarm.day, nextAlarm.hour, nextAlarm.minute, nextAlarm.second);

		// reset alarm fired flag
		_alarmFired = false;

		// send message for debugging
		char messageBody[UART_MESSAGE_BODY_SIZE] = "\n\nALARM EVENT!\n\n\0";
		uartBasic_TX_IT("TIME", messageBody);
	}

	else {

	}
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
	// set flag that an alarm fired
	_alarmFired = true;
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
DateTime getNextAlarm(void) {
	int eventIdx;
	bool nextAlarmFound;
	DateTime now;
	DateTime nextAlarmDateTime = {0};

	// get the current date and time
	getDateTime(&now.year, &now.month, &now.day, &now.month, &now.minute, &now.second);

	// Traverse over the events list and find where 'now' falls.  This can be before
	// any events, within an event, between events, or after all the events.
	eventIdx = 0;
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
		}

		// traverse to next event
		else {
			eventIdx++;
		}
	}

	// If there is no next alarm, then return a generic alarm for
	// the beginning of time (1/1/99 0:0:0)
	if (!nextAlarmFound) {
		nextAlarmDateTime.year = 99;
		nextAlarmDateTime.month = 1;
		nextAlarmDateTime.day = 1;
		nextAlarmDateTime.hour = 0;
		nextAlarmDateTime.minute = 0;
		nextAlarmDateTime.second = 0;
	}

	// Return the next alarm found.
//	char messageBody[UART_MESSAGE_BODY_SIZE];
//	snprintf(messageBody, UART_MESSAGE_BODY_SIZE, "20%02d/%02d/%02d  %02d:%02d:%02d\n", nextAlarmDateTime.year, nextAlarmDateTime.month, nextAlarmDateTime.day, nextAlarmDateTime.hour, nextAlarmDateTime.minute, nextAlarmDateTime.second);
//	uartBasic_TX_IT("NEXT", messageBody);
	return nextAlarmDateTime;
}
