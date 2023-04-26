/*******************************************************************************
 * @file ext_rtc.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 18 Apr 2023
 * @brief External RTC driver.
 * @ingroup ext_rtc
 ******************************************************************************/

/**
 * @defgroup ext_rtc ext_rtc
 * @brief External RTC driver.
 * @{
 */

#ifndef EXT_RTC_H
#define EXT_RTC_H

#include "itf_io.h"

#include <stdbool.h>
#include <stdint.h>

/** Frequency of the clock used by the RTC (Hz). */
#define EXT_RTC_CLK_FREQ (1u)

/** @brief Function prototype for the callback called each second. */
typedef void (* ext_rtc_cb_t)(void);

/** @brief External RTC hardware configuration type. */
typedef struct
{
    h_itf_io_t pin_tick;
} ext_rtc_config_t;

/**
 * @brief Initialization of the external RTC.
 *
 * @return true If the RTC is initialized correctly.
 * @return false If an error occurs.
 */
bool ext_rtc_init(void);

/**
 * @brief Set the current time count.
 *
 * @param[in] seconds Seconds count.
 * @param[in] cseconds Centiseconds count.
 */
void ext_rtc_set_time(uint32_t seconds, uint8_t cseconds);

/**
 * @brief Get the current time count.
 *
 * @param[out] seconds Seconds count.
 * @param[out] cseconds Centiseconds count.
 */
void ext_rtc_get_time(uint32_t * seconds, uint8_t * cseconds);

/**
 * @brief Get the total number of ticks elapsed since the timer start.
 *
 * @return Number of ticks elapsed.
 */
uint32_t ext_rtc_get_ticks(void);

/**
 * @brief Set the callback to call each second.
 *
 * @param[in] cb Callback function to use.
 */
void ext_rtc_set_callback(ext_rtc_cb_t cb);

#endif // EXT_RTC_H

/** @} */

/******************************** End of file *********************************/
