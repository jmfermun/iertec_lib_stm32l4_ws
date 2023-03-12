/*******************************************************************************
 * @file itf_rtc.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 18 Jan 2023
 * @brief RTC interface driver.
 * @ingroup itf_rtc
 ******************************************************************************/

/**
 * @defgroup itf_rtc itf_rtc
 * @brief RTC interface driver.
 * @{
 */

#ifndef ITF_RTC_H
#define ITF_RTC_H

#include "itf_bsp.h"
#include "stm32l4xx_hal.h"

#include <stdbool.h>
#include <stdint.h>

/** Frequency of the clock used by the RTC (Hz). */
#define ITF_RTC_CLK_FREQ (32768u)

/** @brief I2C interface hardware configuration type. */
typedef struct
{
    LPTIM_HandleTypeDef * handle;
    itf_bsp_init_ll_t     init_ll;
} itf_rtc_config_t;

/** @brief Function prototype for the callback called each second. */
typedef void (* itf_rtc_cb_t)(void);

/**
 * @brief Initialization of the RTC interface.
 *
 * @return true If the interface is initialized correctly.
 * @return false If an error occurs.
 */
bool itf_rtc_init(void);

/**
 * @brief Set the current time count.
 *
 * @param[in] seconds Seconds count.
 * @param[in] cseconds Centiseconds count.
 */
void itf_rtc_set_time(uint32_t seconds, uint8_t cseconds);

/**
 * @brief Get the current time count.
 *
 * @param[out] seconds Seconds count.
 * @param[out] cseconds Centiseconds count.
 */
void itf_rtc_get_time(uint32_t * seconds, uint8_t * cseconds);

/**
 * @brief Get the total number of ticks elapsed since the timer start.
 *
 * @return Number of ticks elapsed.
 */
uint32_t itf_rtc_get_ticks(void);

/**
 * @brief Set the callback to call each second.
 *
 * @param[in] cb Callback function to use.
 */
void itf_rtc_set_callback(itf_rtc_cb_t cb);

#endif // ITF_RTC_H

/** @} */

/******************************** End of file *********************************/
