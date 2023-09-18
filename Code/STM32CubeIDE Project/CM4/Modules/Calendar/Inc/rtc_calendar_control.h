/*
 * Author:  Kevin Imlay
 * Date:  September, 2023
 *
 * Purpose:
 *		A
 */


#ifndef RTC_UTILS_H
#define RTC_UTILS_H


#include "stm32wlxx_hal.h"


/* Status returns for RTC.
 *
 */
typedef enum {
  RTC_OKAY = 0,
  RTC_TIMEOUT
} RtcUtilsStatus;


/*
 *
 */
void rtcCalendarControl_init(RTC_HandleTypeDef* hrtc);


/* Set Date Time
 *
 * Sets the date and time of the RTC.
 */
void rtcCalendarControl_setDateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);


/* Get Date Time
 *
 * Returns the current date and time of the RTC.
 */
void rtcCalendarControl_getDateTime(uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second);


/* Set Alarm
 *
 * Sets the date and time in the RTC alarm specified.
 *
 * Note: RTC alarms do not support the month and year! to use this
 *  additional software support is needed (provided in calendar.c).
 */
void rtcCalendarControl_setAlarm_A(uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);


/* Get Alarm
 *
 * Returns the date and time stored in the specified alarm.
 *
 * Note: RTC alarms do not support the month and year! Zero will be
 *  filled in for the month and year.
 */
void rtcCalendarControl_getAlarm_A(uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second);


/* Disable Alarm
 *
 *
 */
void rtcCalendarControl_diableAlarm_A(void);


#endif
