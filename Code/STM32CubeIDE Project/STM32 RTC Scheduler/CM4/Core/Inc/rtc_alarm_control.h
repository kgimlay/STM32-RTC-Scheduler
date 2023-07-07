/*
 *
 */


#ifndef RTC_UTILS_H
#define RTC_UTILS_H


#include "stm32wlxx_hal.h"


/* ----- Typedefs ----- */


/* Status returns for RTC.
 *
 */
typedef enum {
  RTC_OK = 0
} RTC_UTILS_STATUS;


/* ----- Public Function Prototypes ----- */


/*
 *
 */
void rtcAlarmControl_init(RTC_HandleTypeDef* hrtc);


/* Set Date Time
 *
 * Sets the date and time of the RTC.
 */
void setDateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);


/* Get Date Time
 *
 * Returns the current date and time of the RTC.
 */
void getDateTime(uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second);


/* Set Alarm
 *
 * Sets the date and time in the RTC alarm specified.
 *
 * Note: RTC alarms do not support the month and year! to use this
 *  additional software support is needed (provided in calendar.c).
 */
void setAlarm_A(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);


/* Get Alarm
 *
 * Returns the date and time stored in the specified alarm.
 *
 * Note: RTC alarms do not support the month and year! Zero will be
 *  filled in for the month and year.
 */
void getAlarm_A(uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second);


/* Compare Date Time
 *
 * Compares dateTime1 to dateTime2 and returns the difference in seconds.
 *
 * Note: Does not account for leap years, so the returned value can only be used
 *  as a relative measure and does not accurately calculate the time difference
 *  in seconds when leap years are involved!
 */
int compareDateTime(uint8_t year_1, uint8_t month_1, uint8_t day_1, uint8_t hour_1, uint8_t minute_1, uint8_t second_1,
		uint8_t year_2, uint8_t month_2, uint8_t day_2, uint8_t hour_2, uint8_t minute_2, uint8_t second_2);

#endif
