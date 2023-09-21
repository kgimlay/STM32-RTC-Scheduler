/*
 * calendar.c
 *
 *  Created on: Jul 7, 2023
 *      Author: kevinimlay
 */


#include <desktop_app_session.h>
#include <rtc_calendar_control.h>
#include "calendar.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"


/*
 * Macro function to check if the calendar is currently within an event.
 */
#define CURRENTLY_IN_EVENT (_isInEvent)


/*
 * Static linked-list index for end of list.
 */
#define END_CALENDAR_SLL (-1)


/*
 * Private function prototypes.
 */
int32_t _compareDateTime(DateTime dateTime_1, DateTime dateTime_2);
uint32_t _dateTimeToSeconds(DateTime dateTime);
bool _getNextAlarm(DateTime* dateTime, int* nowEventIdx, bool* inEvent);
bool _addEvent(const CalendarEvent* const event);
void _copyEvent(CalendarEvent* const to, const CalendarEvent* const from);
void _update(void);
void _resetEvents(void);


/*
 * Static Linked List node typedef.
 */
typedef struct calendarNode{
	CalendarEvent event;
	int next;
} struct_CalendarLL;


/*
 * Static operational variables used across function calls within this
 * module.
 */
static bool _isInit = false;		// signals if the module has been initialized
static bool _isRunning = false;		// signals if the calendar is running
static bool _isInEvent = false;		// signals if the calendar is within an event
volatile static bool _alarmAFired = false;	// signals if Alarm A has fired, need to update calendar


/*
 * Static operational variables for events linked list.
 */
static struct_CalendarLL _calendarEvents[MAX_NUM_EVENTS] = {0};	// staticall allocated linked list of events
static int _calendarHead = -1;		// index to head of the linked list
static int _calendarFree = 0;		// index to the head of unused nodes within the linked list
static int _currentEvent = -1;		// index to the current event in the linked list


/* calendar_init
 *
 * Initialize the RTC Calendar Control and reset operational variables for
 * this module.
 *
 * Note: will not reinitialize/reset if already initialized.
 */
CalendarStatus calendar_init(RTC_HandleTypeDef* hrtc)
{
	if (hrtc != NULL && hrtc->Instance != NULL)
	{
		if (!_isInit)
		{
			// pass pointer to alarm control
			rtcCalendarControl_init(hrtc);

			// initialize the calendar
			_resetEvents();

			// set init flag
			_isInit = true;
		}

		return CALENDAR_OKAY;
	}

	else
	{
		return CALENDAR_ERROR;
	}
}


/* calendar_resetEvents
 *
 * Reset the events linked list.
 */
CalendarStatus calendar_resetEvents(void)
{
	if (_isInit)
	{
		_resetEvents();

		return CALENDAR_OKAY;
	}

	else
	{
		return CALENDAR_NOT_INIT;
	}
}


/* calendar_start
 *
 * Starts execution of events.  Sets flag to signal if currently within an event
 * and sets RTC Alarm A for the next event transition (start or end of event).
 * Executes start of event callback if starting within an event.  Ignores past
 * events.
 */
CalendarStatus calendar_start(void)
{
	DateTime nextAlarm;
	int currentEventIdx;
	bool withinEvent;

	if (_isInit)
	{
		if (!_isRunning)
		{
			// get calendar alarm for next alarm in event list relative to now
			if (_getNextAlarm(&nextAlarm, &currentEventIdx, &withinEvent)) {
				// set alarm for next event transition (start or end of event)
				rtcCalendarControl_setAlarm_A(nextAlarm.day, nextAlarm.hour, nextAlarm.minute, nextAlarm.second);

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
				rtcCalendarControl_diableAlarm_A();
			}

			// set is running flag
			_isRunning = true;

			return CALENDAR_OKAY;
		}

		else
		{
			return CALENDAR_RUNNING;
		}
	}

	else
	{
		return CALENDAR_NOT_INIT;
	}
}


/* calendar_pause
 *
 * Pauses execution of the calendar without disabling RTC Alarm A.  This is done
 * so that if pausing within an event, the end of event callback will execute while
 * restarting the calendar.
 */
CalendarStatus calendar_pause(void)
{
	if (_isInit)
	{
		if (_isRunning)
		{
			_isRunning = false;

			return CALENDAR_OKAY;
		}

		else
		{
			return CALENDAR_PAUSED;
		}
	}

	else
	{
		return CALENDAR_NOT_INIT;
	}
}


/* calendar_setDateTime
 *
 * Set the date/time within the RTC.
 */
CalendarStatus calendar_setDateTime(const DateTime dateTime)
{
	if (_isInit)
	{
		rtcCalendarControl_setDateTime(dateTime.year, dateTime.month, dateTime.day,
				dateTime.hour, dateTime.minute, dateTime.second);

		return CALENDAR_OKAY;
	}

	else
	{
		return CALENDAR_NOT_INIT;
	}
}


/* calendar_getDateTime
 *
 * Get the date/time within the RTC.
 */
CalendarStatus calendar_getDateTime(DateTime* const dateTime)
{
	if (_isInit)
	{
		rtcCalendarControl_getDateTime(&(dateTime->year), &(dateTime->month),
				&(dateTime->day), &(dateTime->hour), &(dateTime->minute),
				&(dateTime->second));

		return CALENDAR_OKAY;
	}

	else
	{
		return CALENDAR_NOT_INIT;
	}
}


/* calendar_addEvent
 *
 * Add an event to the calendar's event linked list.
 *
 * Note: does not sort events in monotonic order, nor does it check for
 * 	overlapping events.
 */
CalendarStatus calendar_addEvent(const CalendarEvent* const event)
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
		return CALENDAR_NOT_INIT;
	}
}


/* calendar_peekEvent
 *
 * Gets info on the event at the provided index within the linked list.
 * Index from iterating across linkages, not index of array.
 */
CalendarStatus calendar_peekEvent(unsigned int index)
{
	if (_isInit)
	{
		// todo: implement

		return CALENDAR_ERROR;
	}

	else
	{
		return CALENDAR_NOT_INIT;
	}
}


/* calendar_removeEvent
 *
 * Removes the event at the provided index within the linked list.
 * Index from iterating across linkages, not index of array.
 */
CalendarStatus calendar_removeEvent(unsigned int index)
{
	if (_isInit)
	{
		// todo: implement

		return CALENDAR_ERROR;
	}

	else
	{
		return CALENDAR_NOT_INIT;
	}
}


/* calendar_update
 *
 * Update loop.  Updates the state of the calendar (changing events) if the
 * RTC Alarm A has fired to signal an event change.
 *
 * Dependency on _update().
 *
 * Note:
 * 	Will not run if the module has not been initialized and if the calendar
 * 	is not running.
 */
CalendarStatus calendar_update(void)
{
	if (_isInit)
	{
		if (_isRunning)
		{
			if (_alarmAFired) {
				_update();
			}

			return CALENDAR_OKAY;
		}

		else
		{
			return CALENDAR_PAUSED;
		}
	}

	else
	{
		return CALENDAR_NOT_INIT;
	}
}


/* calendar_AlarmA_ISR
 *
 * RTC Alarm A interrupt service routine.  To only be called within the
 * RTC Alarm A ISR (HAL_RTC_AlarmAEventCallback()).
 */
void calendar_AlarmA_ISR(void)
{
	// set flag that an alarm fired
	_alarmAFired = true;
}


/* _compareDateTime
 *
 * Find the time difference in seconds of dateTime1 and dateTime2.
 *
 * Note: Does not account for leap years.
 */
int32_t _compareDateTime(DateTime dateTime_1, DateTime dateTime_2)
{
	uint32_t dateTimeSeconds_1, dateTimeSeconds_2;

	dateTimeSeconds_1 = _dateTimeToSeconds(dateTime_1);
	dateTimeSeconds_2 = _dateTimeToSeconds(dateTime_2);

	// return net comparison (date time 1 - date time 2)
	return dateTimeSeconds_1 - dateTimeSeconds_2;
}


/* _dateTimeToSeconds
 *
 * Converts a date and time to seconds since the start of the century.  Use only
 * for for comparing date/time in seconds, not for absolute date/time in seconds.
 *
 * Note: assumes 30 days in a month and no leap years, it is not needed for the
 * calculation because they are used for relative comparisons, not absolute values.
 */
uint32_t _dateTimeToSeconds(DateTime dateTime)
{
	// Convert to seconds
	return (dateTime.second
			+ (dateTime.minute * 60)
			+ (dateTime.hour * 3600)
			+ ((dateTime.day - 1) * 86400)
			+ ((dateTime.month - 1) * 2592000)
			+ (dateTime.year * 31104000));
}


/* _getNextAlarm
 *
 * Iterate over the calendar events list and find the next alarm to set.  Also
 * returns if the current time is within an event and which event.
 */
bool _getNextAlarm(DateTime* dateTime, int* nowEventIdx, bool* inEvent)
{
	int eventIdx = 0;
	bool nextAlarmFound = false;
	DateTime now = {0};
	DateTime nextAlarmDateTime = {0};

	// get the current date and time
	rtcCalendarControl_getDateTime(&now.year, &now.month, &now.day, &now.hour, &now.minute, &now.second);

	// Traverse over the events list and find where 'now' falls.  This can be before
	// any all the events, within an event, between events, or after all the events.
	eventIdx = _calendarHead;
	nextAlarmFound = false;
	if (eventIdx != -1)
	{
		while (eventIdx != -1 && !nextAlarmFound) {
			// test if before event
			if (_compareDateTime(now, _calendarEvents[eventIdx].event.start) < 0)
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
			else if(_compareDateTime(now, _calendarEvents[eventIdx].event.start) >= 0
					&& _compareDateTime(now, _calendarEvents[eventIdx].event.end) < 0)
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


/* _addEvent
 *
 * Add an event to the linked list of calendar events.
 *
 * Note:  does not add in monotonic ordering or check for overlaps.
 */
bool _addEvent(const CalendarEvent* const event)
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


/* _copyEvent
 *
 * Copy the contents of one CalenderEvent into another.
 */
void _copyEvent(CalendarEvent* const to, const CalendarEvent* const from)
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


/* _update
 *
 * Update loop for module.  If an alarm to signal an event start/end has fired,
 * then this loop will call the callback functions for ending and starting events
 * appropriately.
 *
 * Also handles reseting the alarm for events that occur in a following month/year.
 */
void _update(void)
{
	DateTime nextAlarm;
	int currentEventIdx;
	bool withinEvent;

	// get calendar alarm for next alarm in event list relative to now
	if (_getNextAlarm(&nextAlarm, &currentEventIdx, &withinEvent)) {
		// set alarm for next event transition (start or end of event)
		rtcCalendarControl_setAlarm_A(nextAlarm.day, nextAlarm.hour, nextAlarm.minute, nextAlarm.second);

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
			char body[UART_PACKET_PAYLOAD_SIZE] = {0};
			snprintf(body, UART_PACKET_SIZE, "IN EVENT %d", _currentEvent+1);
			desktopAppSession_enqueueMessage("MESG", body);
			desktopAppSession_update();
		}
		else
		{
			_isInEvent = false;
			// for testing purposes
			char body[UART_PACKET_PAYLOAD_SIZE] = {0};
			snprintf(body, UART_PACKET_SIZE, "NOT IN EVENT");
			desktopAppSession_enqueueMessage("MESG", body);
			desktopAppSession_update();
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


/* _resetEvents
 *
 * Resets/clears the static linked list of calendar events.
 */
void _resetEvents(void)
{
	rtcCalendarControl_diableAlarm_A();

	_calendarHead = -1;
	_calendarFree = 0;
	_currentEvent = -1;
	_isInEvent = false;
	for (int idx = 0; idx < MAX_NUM_EVENTS - 1; idx++)
	{
		_calendarEvents[idx].next = idx + 1;
	}
	_calendarEvents[MAX_NUM_EVENTS - 1].next = -1;

	_isRunning = false;
}
