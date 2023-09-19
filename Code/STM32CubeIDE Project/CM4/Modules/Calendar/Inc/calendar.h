/*
 * Author:  Kevin Imlay
 * Date:  September, 2023
 *
 * Purpose:
 *		A
 */

#ifndef INC_CALENDAR_H_
#define INC_CALENDAR_H_


#include <stdint.h>
#include <uart_packet_helpers.h>
#include "stm32wlxx_hal.h"
#include <stdbool.h>

/*
 * Size of the CalendarEvent queue.
 */
#define MAX_NUM_EVENTS 256

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
typedef struct {
  DateTime start;
  void (*start_callback)(void);
  DateTime end;
  void (*end_callback)(void);
} CalendarEvent;

/*
 * Return status codes for the calendar module.
 */
typedef enum {
	CALENDAR_OKAY = 0,
	CALENDER_NOT_INIT,
	CALENDAR_FULL,
	CALENDAR_EMPTY
} CalendarStatus;

/* calendar_init
 *
 * Function:
 *	Initializes the calendar module.  Must be called before the module
 *	can operate.
 *
 * Parameters:
 *	hrtc - pointer to RTC_HandleTypeDef (HAL) handle of the RTC peripheral
 *			to be used.
 *
 * Return:
 *	bool - true if hrtc is not NULL and the rtc peripheral has been initialized
 *			by the HAL, false otherwise.
 *
 * Note:
 * 	Will not reinitialize if the module is already initialized.
 */
bool calendar_init(RTC_HandleTypeDef* hrtc);

/* calendar_start
 *
 * Function:
 *	Starts execution of calendar events.  Will not start if the module has not
 *	been initialized.
 *
 * Return:
 *	bool - true if the module has been initialized, false if not.
 *
 * Note:
 * 	Starting the calendar is still successful if there are no events in the queue
 * 	or if all events ended prior to the current RTC date and time.
 */
bool calendar_start(void);

/* calendar_pause
 *
 * Function:
 *	Pauses execution of calendar events.  Will not start if the module has not
 *	been initialized.
 *
 * Return:
 *	bool - true if the module has been initialized, false if not.
 *
 * Note:
 * 	Pausing the calendar is still successful if there are no events in the queue
 * 	or if the calendar is not within any events.  Pausing within an event will
 * 	delay the end event callback function execution until the calendar is unpaused
 * 	with calendar_start().
 */
bool calendar_pause(void);

/* calendar_setDateTime
 *
 * Function:
 *	Set the date and time of the RTC.
 *
 * Parameters:
 *	dateTime - the time and date to set the RTC to.
 *
 * Note:
 * 	Only sets time and date if the module has been initialized.  Can set the time
 * 	and date regardless of if the calendar is running or paused.
 */
void calendar_setDateTime(DateTime dateTime);

/* calendar_getDateTime
 *
 * Function:
 *	Get the date and time of the RTC.
 *
 * Parameters:
 *	dateTime - pointer to a DateTime as a destination.
 *
 * Note:
 * 	Only gets time and date if the module has been initialized.  Can get the time
 * 	and date regardless of if the calendar is running or paused.
 */
void calendar_getDateTime(DateTime* dateTime);

/* calendar_addEvent
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
CalendarStatus calendar_addEvent(CalendarEvent* event);

/* calendar_peekEvent
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
void calendar_peekEvent(unsigned int index);

/* calendar_removeEvent
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
void calendar_removeEvent(unsigned int index);

/* calendar_update
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
void calendar_update(void);

/* calendar_AlarmA_ISR
 *
 * Function:
 *
 *
 * Parameters:
 *
 *
 * Return:
 *
 *
 * Note:
 */
void calendar_AlarmA_ISR(void);


#endif /* INC_CALENDAR_H_ */
