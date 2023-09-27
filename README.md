# STM32WL5x Calendar

The STM32WL5x Calendar software module adds event scheduling to the STM32WL5x MCU.  It is designed for use within an STM32CubeMX project for easy compatibility and use across projects.  The STM32WL5x Calendar provides a simple means to schedule events that could, for example, tell the MCU when to perform a task and how long to perform it for.

## Conceptual Design

The scheduler consists of two conceptual parts:  the RTC hardware peripheral used for time and date keeping and the software calendar of events scheduled.

The Calendar module consists of three conceptual parts: the RTC hardware peripheral used for keeping the time and date, the software calendar for storing a collection of events to execute, and the scheduler which ties the RTC and the software calendar together for execution of events.

At the heart of the module is the RTC hardware peripheral, which keeps the current time and date independent of the MCU's operations or power mode.  Along with keeping the current time and date, the RTC is capable of setting an alarm to fire once a specified time and day of the month has been reached.  The RTC is not capable of setting an alarm with a month or year, so this is handled by the scheduler.

Storage of events is taken care of by the software calendar.  Each event has a start time and date and an end time and date, and no more than one event can be in execution at once.  An event on its own is not very interesting unless it is paired with some other action outside of the module.  For this, each event provides the programmer with the option to register callback functions to execute upon the start and end of an event.

Finally, the scheduler brings the RTC and the software calendar together.  The scheduler manages event starting and ending by setting alarms in the RTC, executing event start and end callback functions, and supplements the RTC's lack of ability to set the month and year in it's alarms.

## API

1. *CalendarStatus* - Status code returns from API function calls to signal if the call was successful or why the call was unsuccessful:
   - *CALENDAR_OKAY*  - API function call was successfully completed.
	- *CALENDAR_PARAMETER_ERROR* - API function call was given an invalid parameter.
	- *CALENDAR_NOT_INIT* - The Calendar module has not been initialized before API call.
	- *CALENDAR_FULL* - The calendar is full and cannot accept any more events.
	- *CALENDAR_PAUSED* - The calendar is paused.
	- *CALENDAR_RUNNING* - The calendar is running.

2. *DateTime* - Structure to hold the date and time:
   - *year* - two digit 21st century year (0 - 99)
   - *month* - two digit month (1 - 12)
   - *day* - two digit day of month (1 - 28/29/30/31)
   - *hour* - two digit hour in 24 hour format (0 - 23)
   - *minute* - two digit minute (0 - 59)
   - *second* - two digit second (0 - 59)

## Installation/Usage



## STM32CubeMX Configuration



## Design Choices and Limitations

