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

/*
 *
 */
#define MAX_NUM_EVENTS 256

/*
 *
 */
typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} struct_DateTime;

/*
 *
 */
typedef struct {
  struct_DateTime start;
  void (*start_callback)(void);
  struct_DateTime end;
  void (*end_callback)(void);
} CalendarEvent;

/*
 *
 */
typedef enum {
	CALENDAR_OKAY = 0,
	CALENDER_NOT_INIT,
	CALENDAR_FULL,
	CALENDAR_EMPTY
} CalendarStatus;

/*
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
void calendar_init(RTC_HandleTypeDef* hrtc);

/*
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
void calendar_start(void);

/*
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
void calendar_pause(void);

/*
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
void calendar_setDateTime(struct_DateTime dateTime);

/*
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
void calendar_getDateTime(struct_DateTime* dateTime);

/*
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

/*
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

/*
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

/*
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

/*
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
