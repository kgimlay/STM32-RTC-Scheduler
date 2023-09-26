/*
 * mode_timer.c
 *
 *  Created on: Jul 7, 2023
 *      Author: kevinimlay
 */


#include <mode_timer.h>


/*
 * Static operational variables for operation across function calls.
 */
static LPTIM_HandleTypeDef* _lptimer;
static uint32_t _ar_value;
static uint32_t _cm_value;
static void (*_startCallback)(void);
static void (*_endCallback)(void);


/* modeTimer_init
 *
 */
bool modeTimer_init(LPTIM_HandleTypeDef* const hlptim)
{
	if (hlptim != NULL)
	{
		_lptimer = hlptim;

		return true;
	}

	else
	{
		return false;
	}
}


/* modeTimer_configure
 *
 */
MODE_TIMER_STATUS modeTimer_configure(const uint16_t period, const uint16_t pulse, void (*startCallback)(), void (*endCallback)())
{
	if (_lptimer != NULL)
	{
		_ar_value = period;
		_cm_value = pulse;
		_startCallback = startCallback;
		_endCallback = endCallback;

		return MODE_TIMER_OKAY;
	}

	else
	{
		return MODE_TIMER_NOT_INIT;
	}
}


/* modeTimer_start
 *
 */
MODE_TIMER_STATUS modeTimer_start(void)
{
	if (_lptimer != NULL)
	{
		// start timer in PWM mode (compare match and auto reload callbacks used)
		HAL_LPTIM_PWM_Start_IT(_lptimer, _ar_value, _cm_value);

		// call start callback
		if (_startCallback != NULL)
			_startCallback();

		return MODE_TIMER_OKAY;
	}

	else
	{
		return MODE_TIMER_NOT_INIT;
	}
}


/* modeTimer_stop
 *
 */
MODE_TIMER_STATUS modeTimer_stop(void)
{
	if (_lptimer != NULL)
	{
		// stop the timer
		HAL_LPTIM_PWM_Stop_IT(_lptimer);

		// call end callback
		if (_endCallback != NULL)
			_endCallback();

		return MODE_TIMER_OKAY;
	}

	else
	{
		return MODE_TIMER_NOT_INIT;
	}
}


/* modeTimer_CompareMatch_IRQ
 *
 */
void modeTimer_CompareMatch_IRQ(void)
{
	// call end callback
	if (_endCallback != NULL)
		_endCallback();
}


/* modeTimer_AutoReload_IRQ
 *
 */
void modeTimer_AutoReload_IRQ(void)
{
	// call start callback
	if (_startCallback != NULL)
		_startCallback();
}
