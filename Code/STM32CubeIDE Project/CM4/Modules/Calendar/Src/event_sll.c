/*
 * Author: Kevin Imlay
 * Date: September, 2023
 */


#include "event_sll.h"
#include "string.h"


/*
 * Static linked-list index for end of list.
 */
#define EVENTS_SLL_NO_EVENT (-1)


/*
 *
 */
void _copyEvent2(struct CalendarEvent* const to, const struct CalendarEvent* const from);
int32_t _compareDateTime2(DateTime dateTime_1, DateTime dateTime_2);


/*
 *
 */
bool eventSLL_reset(Event_SLL* const sll)
{
	int idx;

	sll->inProgress = EVENTS_SLL_NO_EVENT;
	sll->freeHead = 0;
	sll->usedHead = EVENTS_SLL_NO_EVENT;
	sll->count = 0;

	memset(sll->events, 0, sizeof(struct EventSLL_Node) * MAX_NUM_EVENTS);
	for (idx = 0; idx < MAX_NUM_EVENTS - 1; idx++)
	{
		sll->events[idx].id = idx;
		sll->events[idx].next = idx + 1;
	}
	sll->events[idx].next = EVENTS_SLL_NO_EVENT;

	return true;
}


/*
 *
 */
bool eventSLL_insert(Event_SLL* const sll, const struct CalendarEvent event)
{
	int prevToInsertIdx;
	int toInsertIdx;
	int tempIdx;

	// if list is not full
	if (sll->count < MAX_NUM_EVENTS)
	{
		// if the list is empty, insert at beginning
		if (sll->usedHead == EVENTS_SLL_NO_EVENT)
		{
			// set used and free heads
			sll->usedHead = sll->freeHead;							// point head of used to head of free
			sll->freeHead = sll->events[sll->freeHead].next;		// point head of free to next of free
			toInsertIdx = sll->usedHead;
			sll->events[toInsertIdx].next = EVENTS_SLL_NO_EVENT;	// point head of used to none
		}

		// the list is not empty
		// iterate over list and insert where appropriate for monotonic ordering
		else if (sll->count < MAX_NUM_EVENTS)
		{
			// if inserting at start
			if (_compareDateTime2(event.start, sll->events[sll->usedHead].event.start) < 0)
			{
				// take from head of free nodes and move to start of used nodes
				tempIdx = sll->usedHead;							// store head of used in temp
				sll->events[sll->usedHead].next = sll->freeHead;	// point head of used to head of free
				sll->freeHead = sll->events[sll->freeHead].next;	// point head of free to next of free
				toInsertIdx = sll->usedHead;
				sll->events[toInsertIdx].next = tempIdx;			// point head of used to stored in temp
			}

			// if inserting not at the start
			else
			{
				// find node previous to where to insert
				prevToInsertIdx = sll->usedHead;
				// while insert event's start time is less than each event's start time
				// already in the list, iterate list
				// if the start times are equal, then inserting after the current iteration
				// does not care about end times of events
				while (_compareDateTime2(event.start, sll->events[prevToInsertIdx].event.start) < 0)
					prevToInsertIdx = sll->events[prevToInsertIdx].next;

				// perform insert
				tempIdx = sll->events[prevToInsertIdx].next;		// store previous to insert in temp
				sll->events[prevToInsertIdx].next = sll->freeHead;	// point next to insert to head of free
				sll->freeHead = sll->events[sll->freeHead].next;	// point head of free to next of free
				toInsertIdx = sll->events[prevToInsertIdx].next;
				sll->events[toInsertIdx].next = tempIdx;			// point new node to stored in temp
			}
		}

		// copy event into new node
		_copyEvent2(&(sll->events[toInsertIdx].event), &event);

		// increment count
		(sll->count)++;

		return true;
	}

	// if the list is full
	else
	{
		return false;
	}
}


/*
 *
 */
bool eventSLL_remove(Event_SLL* const sll, const int id)
{
	int prevToRemoveIdx;
	int toRemoveIdx;
	int tempIdx;

	// if list is not empty
	if (sll->count < MAX_NUM_EVENTS)
	{
		// if removing from beginning
		if (id == sll->usedHead)
		{
			// move from front of used to front of free
			tempIdx = sll->events[sll->usedHead].next;			// store next to first used in temp
			sll->events[sll->usedHead].next = sll->freeHead;	// point fist used to first of free
			sll->freeHead = sll->usedHead;						// point head of free to first used
			sll->usedHead = tempIdx;							// point head of used to temp
		}

		// if removing from end or middle
		else
		{
			// find the previous to where to remove
			prevToRemoveIdx = sll->usedHead;
			toRemoveIdx = sll->events[prevToRemoveIdx].next;
			// iterate until found
			while (sll->events[toRemoveIdx].id != id)
			{
				prevToRemoveIdx = toRemoveIdx;
				toRemoveIdx = sll->events[prevToRemoveIdx].next;
			}

			// perform removal
			tempIdx = sll->freeHead;									// store first free in temp
			sll->freeHead = toRemoveIdx;								// point free head to remove
			sll->events[prevToRemoveIdx].next = EVENTS_SLL_NO_EVENT;	// point previous to remove to none
			sll->events[toRemoveIdx].next = tempIdx;					// point remove next to temp

		}

		// decrement count
		(sll->count)--;

		return true;
	}

	// if list is empty
	else
	{
		return false;
	}
}


/*
 *
 */
struct CalendarEvent eventSLL_peekIdx(Event_SLL* const sll, const int index)
{
	struct CalendarEvent returnEvent;

	_copyEvent2(&returnEvent, &(sll->events[index].event));

	return returnEvent;
}


/*
 *
 */
bool eventSLL_updateNow(Event_SLL* const sll, const DateTime now)
{

}


/*
 *
 */
bool eventSLL_nextAlarm(Event_SLL* const sll)
{

}


/* _copyEvent
 *
 * Copy the contents of one CalenderEvent into another.
 */
void _copyEvent2(struct CalendarEvent* const to, const struct CalendarEvent* const from)
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


/* _compareDateTime
 *
 * Find the time difference in seconds of dateTime1 and dateTime2.
 *
 * Note: Does not account for leap years.
 */
int32_t _compareDateTime2(DateTime dateTime_1, DateTime dateTime_2)
{
	uint32_t dateTimeSeconds_1, dateTimeSeconds_2;

	dateTimeSeconds_1 = _dateTimeToSeconds(dateTime_1);
	dateTimeSeconds_2 = _dateTimeToSeconds(dateTime_2);

	// return net comparison (date time 1 - date time 2)
	return dateTimeSeconds_1 - dateTimeSeconds_2;
}
