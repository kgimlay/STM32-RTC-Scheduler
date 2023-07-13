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
void setAlarm_A(uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);


/* Get Alarm
 *
 * Returns the date and time stored in the specified alarm.
 *
 * Note: RTC alarms do not support the month and year! Zero will be
 *  filled in for the month and year.
 */
void getAlarm_A(uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second);


#endif
