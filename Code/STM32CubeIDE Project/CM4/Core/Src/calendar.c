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

#include "stdio.h"
#include "com_session_layer.h"


#define CURRENTLY_IN_EVENT (_isInEvent)
#define END_CALENDAR_LL (-1)


int32_t compareDateTime(DateTime dateTime_1, DateTime dateTime_2);
uint32_t dateTimeToSeconds(DateTime dateTime);
bool getNextAlarm(DateTime* dateTime, int* nowEventIdx, bool* inEvent);
bool _addEvent(CalendarEvent* event);
void _copyEvent(CalendarEvent* to, CalendarEvent* from);


/*
 *
 */
typedef struct calendarNode{
	CalendarEvent event;
	int next;
} CalendarLL;


/*
 *
 */
static bool _isInit = false;
volatile static bool _alarmAFired = false;
RTC_HandleTypeDef* _hrtc = NULL;
CalendarLL _calendarEvents[MAX_NUM_EVENTS] = {0};
static int _calendarHead = -1;
static int _calendarFree = 0;
static int _currentEvent = -1;
static bool _isInEvent = false;



/*
 *
 */
void calendar_init(RTC_HandleTypeDef* hrtc)
{
	// store pointer to rtc structure
	_hrtc = hrtc;

	// pass pointer to alarm control
	rtcAlarmControl_init(hrtc);

	// initialize the calendar
	_calendarHead = -1;
	_calendarFree = 0;
	_currentEvent = -1;
	_isInEvent = false;
	for (int idx = 0; idx < MAX_NUM_EVENTS - 1; idx++)
	{
		_calendarEvents[idx].next = idx + 1;
	}
	_calendarEvents[MAX_NUM_EVENTS - 1].next = -1;

	// set init flag
	_isInit = true;
}


/*
 *
 */
void calendar_start(void)
{
	DateTime nextAlarm;
	int currentEventIdx;
	bool withinEvent;

	// get calendar alarm for next alarm in event list relative to now
	if (getNextAlarm(&nextAlarm, &currentEventIdx, &withinEvent)) {
		// set alarm for next event transition (start or end of event)
		setAlarm_A(nextAlarm.day, nextAlarm.hour, nextAlarm.minute, nextAlarm.second);

		// if starting within an event, run the start callback
		if (withinEvent) {
			if (_calendarEvents[currentEventIdx].event.start_callback != NULL)
				(*_calendarEvents[currentEventIdx].event.start_callback)();
			_currentEvent = currentEventIdx;
			_isInEvent = true;
		}
		else
		{
			_isInEvent = false;
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
void calendar_setDateTime(DateTime dateTime)
{
	setDateTime(dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second);
}


/*
 *
 */
void calendar_getDateTime(DateTime* dateTime)
{
	getDateTime(&(dateTime->year), &(dateTime->month), &(dateTime->day), &(dateTime->hour), &(dateTime->minute), &(dateTime->second));
}


/*
 *
 */
CalendarStatus calendar_addEvent(CalendarEvent *event)
{
	// add only if the calendar has been initialized
	if (_isInit)
	{
		if (_addEvent(event))
		{
			return CALENDAR_OKAY;
		}

		else
		{
			return CALENDAR_FULL;
		}
	}

	// the calendar has not been initialized
	else
	{
		return CALENDER_NOT_INIT;
	}
}


/*
 *
 */
void calendar_getEvent(DateTime* dateTime)
{

}


/*
 *
 */
void calendar_removeEvent(unsigned int index)
{

}


/*
 *
 */
void calendar_handleAlarm(void)
{
	DateTime nextAlarm;
	int currentEventIdx;
	bool withinEvent;

	if (_alarmAFired) {
		// get calendar alarm for next alarm in event list relative to now
		if (getNextAlarm(&nextAlarm, &currentEventIdx, &withinEvent)) {
			// set alarm for next event transition (start or end of event)
			setAlarm_A(nextAlarm.day, nextAlarm.hour, nextAlarm.minute, nextAlarm.second);

			// if the current event has changed (event ended or began), then run appropriate
			// callback functions

			// if entering an event from no event
			if (withinEvent && !CURRENTLY_IN_EVENT) {

				// call start event callback
				if (_calendarEvents[currentEventIdx].event.start_callback != NULL)
					(*_calendarEvents[currentEventIdx].event.start_callback)();
			}

			// if entering an event from another event
			else if (withinEvent && CURRENTLY_IN_EVENT) {
				// call end event callback for event just left
				if (_calendarEvents[_currentEvent].event.end_callback != NULL)
					(*_calendarEvents[_currentEvent].event.end_callback)();

				// call start event callback for event just entered
				if (_calendarEvents[currentEventIdx].event.start_callback != NULL)
					(*_calendarEvents[currentEventIdx].event.start_callback)();

			}

			// if exiting an event into no event
			else if (!withinEvent && CURRENTLY_IN_EVENT) {
				// call end event callback for event just left
				if (_calendarEvents[_currentEvent].event.end_callback != NULL)
					(*_calendarEvents[_currentEvent].event.end_callback)();

			}

			// else, alarm is just being reset for next month/year


			// update current event
			_currentEvent = currentEventIdx;
			if (withinEvent)
			{
				_isInEvent = true;
				// for testing purposes
				char body[UART_MESSAGE_BODY_SIZE] = {0};
				snprintf(body, UART_MESSAGE_SIZE, "IN EVENT %d", _currentEvent+1);
				tell("MESG", body);
				session_cycle();
			}
			else
			{
				_isInEvent = false;
				// for testing purposes
				char body[UART_MESSAGE_BODY_SIZE] = {0};
				snprintf(body, UART_MESSAGE_SIZE, "NOT IN EVENT");
				tell("MESG", body);
				session_cycle();
			}

		}

		// if there is no alarm to set, disable the alarm and exit any events
		else {
//			diableAlarm_A();

			if (CURRENTLY_IN_EVENT) {
				// call end event callback for event just left
				if (_calendarEvents[_currentEvent].event.end_callback != NULL)
					(*_calendarEvents[_currentEvent].event.end_callback)();
			}
		}

		// reset alarm fired flag
		_alarmAFired = false;
	}
}


/*
 *
 */
void calendar_AlarmA_ISR(void)
{
	// set flag that an alarm fired
	_alarmAFired = true;
}


/* Find the time difference in seconds of dateTime1 and dateTime2.
 *
 * Note: Does not account for leap years.
 */
int32_t compareDateTime(DateTime dateTime_1, DateTime dateTime_2)
{
	uint32_t dateTimeSeconds_1, dateTimeSeconds_2;

	dateTimeSeconds_1 = dateTimeToSeconds(dateTime_1);
	dateTimeSeconds_2 = dateTimeToSeconds(dateTime_2);

	// return net comparison (date time 1 - date time 2)
	return dateTimeSeconds_1 - dateTimeSeconds_2;
}


/*
 *
 */
uint32_t dateTimeToSeconds(DateTime dateTime)
{
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
bool getNextAlarm(DateTime* dateTime, int* nowEventIdx, bool* inEvent)
{
	int eventIdx = 0;
	bool nextAlarmFound = false;
	DateTime now = {0};
	DateTime nextAlarmDateTime = {0};

	// get the current date and time
	getDateTime(&now.year, &now.month, &now.day, &now.hour, &now.minute, &now.second);

	// Traverse over the events list and find where 'now' falls.  This can be before
	// any all the events, within an event, between events, or after all the events.
	eventIdx = _calendarHead;
	nextAlarmFound = false;
	if (eventIdx != -1)
	{
		while (eventIdx != -1 && !nextAlarmFound) {
			// test if before event
			if (compareDateTime(now, _calendarEvents[eventIdx].event.start) < 0)
			{
				// then the next alarm is the beginning of this event
				nextAlarmDateTime.year = _calendarEvents[eventIdx].event.start.year;
				nextAlarmDateTime.month = _calendarEvents[eventIdx].event.start.month;
				nextAlarmDateTime.day = _calendarEvents[eventIdx].event.start.day;
				nextAlarmDateTime.hour = _calendarEvents[eventIdx].event.start.hour;
				nextAlarmDateTime.minute = _calendarEvents[eventIdx].event.start.minute;
				nextAlarmDateTime.second = _calendarEvents[eventIdx].event.start.second;

				// set found
				nextAlarmFound = true;
				*inEvent = false;
			}

			// test if within event
			else if(compareDateTime(now, _calendarEvents[eventIdx].event.start) >= 0
					&& compareDateTime(now, _calendarEvents[eventIdx].event.end) < 0)
			{
				// then the next alarm is the end of this event
				nextAlarmDateTime.year = _calendarEvents[eventIdx].event.end.year;
				nextAlarmDateTime.month = _calendarEvents[eventIdx].event.end.month;
				nextAlarmDateTime.day = _calendarEvents[eventIdx].event.end.day;
				nextAlarmDateTime.hour = _calendarEvents[eventIdx].event.end.hour;
				nextAlarmDateTime.minute = _calendarEvents[eventIdx].event.end.minute;
				nextAlarmDateTime.second = _calendarEvents[eventIdx].event.end.second;

				// set found
				nextAlarmFound = true;
				*inEvent = true;
			}

			// traverse to next event
			else {
				eventIdx = _calendarEvents[eventIdx].next;
			}
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
		*nowEventIdx = eventIdx;
		return true;
	}
}


/*
 *
 */
bool _addEvent(CalendarEvent* event)
{
	int tempEndOfHead;

	// if the calendar is full, return early
	if (_calendarFree == -1)
	{
		return false;
	}

	// if the calendar is empty, insert at front
	if (_calendarHead == -1)
	{
		// copy event into start of free linked list
		_copyEvent(&(_calendarEvents[_calendarFree].event), event);

		// set the head
		_calendarHead = _calendarFree;

		// set free to next free
		_calendarFree = _calendarEvents[_calendarFree].next;

		// set end of head list
		_calendarEvents[_calendarHead].next = -1;
	}

	// if the calendar is not empty (or full), insert at end of list
	else
	{
		// find end of head
		tempEndOfHead = _calendarHead;
		while(_calendarEvents[tempEndOfHead].next != -1)
			tempEndOfHead = _calendarEvents[tempEndOfHead].next;

		// copy events into start of free linked list
		_copyEvent(&(_calendarEvents[_calendarFree].event), event);

		// link into head list
		_calendarEvents[tempEndOfHead].next = _calendarFree;
		tempEndOfHead = _calendarFree;

		// set free to next free
			_calendarFree = _calendarEvents[_calendarFree].next;

		// set end of head list
		_calendarEvents[tempEndOfHead].next = -1;
	}

	// return okay
	return true;
}


/*
 *
 */
void _copyEvent(CalendarEvent* to, CalendarEvent* from)
{
	to->start.year = from->start.year;
	to->start.month = from->start.month;
	to->start.day = from->start.day;
	to->start.hour = from->start.hour;
	to->start.minute = from->start.minute;
	to->start.second = from->start.second;
	to->start_callback = from->start_callback;
	to->end.year = from->end.year;
	to->end.month = from->end.month;
	to->end.day = from->end.day;
	to->end.hour = from->end.hour;
	to->end.minute = from->end.minute;
	to->end.second = from->end.second;
	to->end_callback = from->end_callback;
}
