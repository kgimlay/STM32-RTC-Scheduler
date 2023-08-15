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


/*
 *
 */
void setDateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
  RTC_DateTypeDef date;
  RTC_TimeTypeDef time;

  date.Year = ((year/10) << 4) | (year % 10);
  date.Month = ((month/10) << 4) | (month % 10);
  date.Date = ((day/10) << 4) | (day % 10);
  date.WeekDay = 0;
  time.Hours = ((hour/10) << 4) | (hour % 10);
  time.Minutes = ((minute/10) << 4) | (minute % 10);
  time.Seconds = ((second/10) << 4) | (second % 10);

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


/*
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
  *year = ((date.Year & 0xF0) >> 4) * 10 + (date.Year & 0x0F);
  *month = ((date.Month & 0xF0) >> 4) * 10 + (date.Month & 0x0F);
  *day = ((date.Date & 0xF0) >> 4) * 10 + (date.Date & 0x0F);
  *hour = ((time.Hours & 0xF0) >> 4) * 10 + (time.Hours & 0x0F);
  *minute = ((time.Minutes & 0xF0) >> 4) * 10 + (time.Minutes & 0x0F);
  *second = ((time.Seconds & 0xF0) >> 4) * 10 + (time.Seconds & 0x0F);
}


/*
 *
 */
void setAlarm_A(uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
  RTC_AlarmTypeDef alarm = {0};

  alarm.AlarmDateWeekDay = ((day/10) << 4) | (day % 10);
  alarm.AlarmTime.Hours = ((hour/10) << 4) | (hour % 10);
  alarm.AlarmTime.Minutes = ((minute/10) << 4) | (minute % 10);
  alarm.AlarmTime.Seconds = ((second/10) << 4) | (second % 10);
  alarm.AlarmTime.SubSeconds = 0x0;
  alarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  alarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
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


/*
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
