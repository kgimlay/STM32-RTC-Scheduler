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
//#include <stm32wlxx_hal_lptim.h>

/*
 *
 */
typedef enum {
	MODE_TIMER_OKAY = 0,
	MODE_TIMER_NOT_INIT
} MODE_TIMER_STATUS;

/*
 *
 */
//bool modeTimer_init(LPTIM_HandleTypeDef* const lptimh);


#endif /* MODE_TIMER_INC_MODE_TIMER_H_ */
