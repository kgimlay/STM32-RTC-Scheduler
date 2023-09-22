/*
 * Author: Kevin Imlay
 * Date: September, 2023
 */

#ifndef CALENDAR_INC_EVENT_SLL_H_
#define CALENDAR_INC_EVENT_SLL_H_


#include <stdint.h>
#include <stdbool.h>

/*
 * Size of the CalendarEvent queue.
 */
#define MAX_NUM_EVENTS 32

/*
 * Static linked-list index for end of list.
 */
#define EVENTS_SLL_NO_EVENT (-1)

/*
 * Structure to hold a date and time.
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
 * Structure to hold the start and end DateTime of an event
 * along with callback function pointers to execute when an
 * event starts and ends.
 */
typedef struct CalendarEvent {
  DateTime start;
  DateTime end;
  void (*start_callback)(void);
  void (*end_callback)(void);
} CalendarEvent;

/*
 *
 */
struct EventSLL_Node {
	struct CalendarEvent event;
	int id;
	int next;
};

/*
 *
 */
typedef struct {
	struct EventSLL_Node events[MAX_NUM_EVENTS];		// array to hold statically-allocated nodes
	int usedHead;			// index to first node that is "in use"
	int freeHead;			// index to first node that is "not in use"
	int inProgress;			// index to the event currently in progress or CALENDAR_SLL_NO_EVENT
	unsigned int count;		// counter to keep track of how many events nodes are used;
} Event_SLL;


/* resetEventSLL
 *
 */
bool eventSLL_reset(Event_SLL* const sll);

/* eventSLL_insert
 *
 *
 *
 * Note:  it is recommended to run eventSLL_updateNow() after inserting events.
 *  this can be done after a bulk of insert operations.
 */
bool eventSLL_insert(Event_SLL* const sll, const struct CalendarEvent event);

/* eventSLL_remove
 *
 * Note:  it is recommended to run eventSLL_updateNow() after removing events.
 *  this can be done after a bulk of remove operations.
 */
bool eventSLL_remove(Event_SLL* const sll, const int id);

/* eventSLL_peekIdx
 *
 */
bool eventSLL_peekIdx(Event_SLL* const sll, const int id, struct CalendarEvent* const event);

/* eventSLL_updateNow
 *
 */
bool eventSLL_updateNow(Event_SLL* const sll, const DateTime now, DateTime* const alarm);


#endif /* CALENDAR_INC_EVENT_SLL_H_ */
