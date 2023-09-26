/*
 * Author:  Kevin Imlay
 * Date:  September, 2023
 *
 * Purpose:
 *		T
 */

#ifndef MODE_TIMER_INC_MODE_TIMER_H_
#define MODE_TIMER_INC_MODE_TIMER_H_


#include <stdbool.h>
#include <stdint.h>
#include <stm32wlxx_hal.h>

/*
 * Module status codes.
 */
typedef enum {
	MODE_TIMER_OKAY = 0,
	MODE_TIMER_NOT_INIT,
	MODE_TIMER_ERROR
} MODE_TIMER_STATUS;

/* modeTimer_init
 *
 * Function:
 * 	Initialize the mode timer module.
 *
 * Parameters:
 * 	hlptim - pointer to LPTIM_HandleTypeDef (HAL handle) for a low-powered timer.
 *
 * Return:
 * 	bool - false if the pointer passed was NULL, true otherwise.
 */
bool modeTimer_init(LPTIM_HandleTypeDef* const hlptim);

/* modeTimer_configure
 *
 * Function:
 * 	Configure the PRI settings (period and pulse width) and set callback functions
 * 	to fun at the start and end of each pulse.
 *
 * Parameters:
 * 	period - 16-bit auto-reload value (period of the PRI)
 * 	pulse - 16 bit compare-match value (pulse width of the PRI)
 * 	startCallback - pointer to function to execute at start of pulse
 * 	endCallback - pointer to function to execute at end of pulse
 *
 * Return:
 * 	MODE_TIMER_STATUS
 * 		MODE_TIMER_NOT_INIT - if the module has not been initialized
 * 		MODE_TIMER_OKAY - successful
 */
MODE_TIMER_STATUS modeTimer_configure(const uint16_t period, const uint16_t pulse, void (*startCallback)(), void (*endCallback)());

/* modeTimer_start
 *
 * Function:
 * 	Start the mode timer.
 *
 * Return:
 * 	MODE_TIMER_STATUS
 * 		MODE_TIMER_NOT_INIT - if the module has not been initialized
 * 		MODE_TIMER_OKAY - successful
 *
 * Note:
 * 	Configure by calling modeTimer_configure() before starting the mode timer.
 */
MODE_TIMER_STATUS modeTimer_start(void);

/* modeTimer_stop
 *
 * Function:
 * 	Stops the mode timer.
 *
 * Return:
 * 	MODE_TIMER_STATUS
 * 		MODE_TIMER_NOT_INIT - if the module has not been initialized
 * 		MODE_TIMER_OKAY - successful
 */
MODE_TIMER_STATUS modeTimer_stop(void);

/* modeTimer_CompareMatch_IRQ
 *
 * Function:
 * 	ISR for mode timer.
 *
 * Note:
 * 	Call within HAL_LPTIM_CompareMatchCallback() surrounded by
 * 	if (hlptim->Instance == LPTIMx), where LPTIMx is LPTIM1, LPTIM2,
 * 	or LPTIM3 whichever you use in modeTimer_init().
 */
void modeTimer_CompareMatch_IRQ(void);

/* modeTimer_AutoReload_IRQ
 *
 * Function:
 * 	ISR for mode timer.
 *
 * Note:
 * 	Call within HAL_LPTIM_AutoReloadMatchCallback() surrounded by
 * 	if (hlptim->Instance == LPTIMx), where LPTIMx is LPTIM1, LPTIM2,
 * 	or LPTIM3 whichever you use in modeTimer_init().
 */
void modeTimer_AutoReload_IRQ(void);


#endif /* MODE_TIMER_INC_MODE_TIMER_H_ */
