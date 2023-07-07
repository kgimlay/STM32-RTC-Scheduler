/*
 *
 */

#include <rtc_alarm_control.h>
#include <stdbool.h>


#define RTC_FORMAT RTC_FORMAT_BCD


RTC_HandleTypeDef* _rtc_handle;


/*
 *
 */
void rtcAlarmControl_init(RTC_HandleTypeDef* hrtc) {
	_rtc_handle = hrtc;
}


/* Set the date and time in the RTC.
 *
 */
void setDateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
  RTC_DateTypeDef date;
  RTC_TimeTypeDef time;

  date.Year = year;
  date.Month = month;
  date.Date = day;
  date.WeekDay = 0;
  time.Hours = hour;
  time.Minutes = minute;
  time.Seconds = second;

  // Set the date.
  if (HAL_RTC_SetDate(_rtc_handle, &date, RTC_FORMAT) != HAL_OK) {
    __disable_irq();
    while (1)
    	(void)0;
  }

  // Set the time.
  if (HAL_RTC_SetTime(_rtc_handle, &time, RTC_FORMAT) != HAL_OK) {
    __disable_irq();
    while (1)
    	(void)0;
  }
}


/* Get the time and date in the RTC.
 *
 */
void getDateTime(uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second) {
  RTC_TimeTypeDef time = {0};
  RTC_DateTypeDef date = {0};

  // Get the time and the date.
  if(HAL_RTC_GetTime(_rtc_handle, &time, RTC_FORMAT) != HAL_OK) {
    __disable_irq();
    while (1)
    	(void)0;
  }
  if(HAL_RTC_GetDate(_rtc_handle, &date, RTC_FORMAT) != HAL_OK) {
    __disable_irq();
    while (1)
    	(void)0;
  }

  // Return through parameters
  *year = date.Year;
  *month = date.Month;
  *day = date.Date;
  *hour = time.Hours;
  *minute = time.Minutes;
  *second = time.Seconds;
}


/* Set the time and date for alarm A.
 *
 */
void setAlarm_A(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
  RTC_AlarmTypeDef alarm = {0};
  alarm.AlarmDateWeekDay = day;
  alarm.AlarmTime.Hours = hour;
  alarm.AlarmTime.Minutes = minute;
  alarm.AlarmTime.Seconds = second;
  alarm.AlarmTime.SubSeconds = 0;
  alarm.AlarmMask = RTC_ALARMMASK_NONE;
  alarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  alarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  alarm.Alarm = RTC_ALARM_A;

  if (HAL_RTC_SetAlarm_IT(_rtc_handle, &alarm, RTC_FORMAT) != HAL_OK)
  {
    __disable_irq();
    while (1)
    	(void)0;
  }
}


/* Get the time and date of alarm A.
 *
 */
void getAlarm_A(uint8_t* year, uint8_t* month, uint8_t* day, uint8_t* hour, uint8_t* minute, uint8_t* second) {
  RTC_AlarmTypeDef alarm = {0};

  // Get the alarm information.
  if(HAL_RTC_GetAlarm(_rtc_handle, &alarm, RTC_ALARM_A, RTC_FORMAT) != HAL_OK) {
    __disable_irq();
    while (1)
    	(void)0;
  }

  // Return through parameters
  *year = 0;
  *month = 0;
  *day = alarm.AlarmDateWeekDay;
  *hour = alarm.AlarmTime.Hours;
  *minute = alarm.AlarmTime.Minutes;
  *second = alarm.AlarmTime.Seconds;
}


/* Find the time difference in seconds of dateTime1 and dateTime2.
 *
 * Note: Does not account for leap years.
 */
int compareDateTime(uint8_t year_1, uint8_t month_1, uint8_t day_1, uint8_t hour_1, uint8_t minute_1, uint8_t second_1,
		uint8_t year_2, uint8_t month_2, uint8_t day_2, uint8_t hour_2, uint8_t minute_2, uint8_t second_2) {
  uint32_t dateTime1_seconds, dateTime2_seconds;

  // Convert to seconds. Note: assumes 30 days in a month and
  // no leap years, it is not needed for the calculation because
  // they are used for relative comparisons, not absolute values.
  dateTime1_seconds = second_1
        + (minute_1 * 60)
        + (hour_1 * 3600)
        + ((day_1 - 1) * 86400)
        + ((month_1 - 1) * 2592000)
        + (year_1 * 31104000);
  dateTime2_seconds = second_2
        + (minute_2 * 60)
        + (hour_2 * 3600)
        + ((day_2 - 1) * 86400)
        + ((month_2 - 1) * 2592000)
        + (year_2 * 31104000);

  // return net comparison (date time 1 - date time 2)
  return dateTime1_seconds - dateTime2_seconds;
}
