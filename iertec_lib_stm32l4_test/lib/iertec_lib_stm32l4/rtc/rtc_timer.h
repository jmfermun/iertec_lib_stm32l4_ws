/*******************************************************************************
 * @file rtc_timer.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 19 Apr 2023
 * @brief Generic timers that can use any tick interrupt source.
 * @ingroup rtc_timer
 ******************************************************************************/

/**
 * @defgroup rtc_timer rtc_timer
 * @brief Generic timers that can use any tick interrupt source.
 * @{
 */

#ifndef RTC_TIMER_H
#define RTC_TIMER_H

#include <stdbool.h>
#include <stdint.h>

// Forward declaration needed in the structure definition
typedef struct rtc_timer_st rtc_timer_t;

/**
 * @brief Timer timeout handler function prototype.
 *
 * @param[in] Timer that has expired.
 */
typedef void (* rtc_timer_fn)(rtc_timer_t * timer);

/** @brief Timer instance structure. */
typedef struct rtc_timer_st
{
    /** Timer state. */
    bool active;

    /** Timer ticks relative to its preceding timer. */
    uint32_t ticks;

    /** Timer that will expire next to this one. */
    rtc_timer_t * next;

    /** Auxiliary variable used by the tick processing function. */
    rtc_timer_t * next_exp;

    /** Timer identifier. It can be used in the timeout function. */
    uint32_t id;

    /** The function that will be called when the timer expires. */
    rtc_timer_fn fn;
} rtc_timer_t;

/**
 * @brief Initialize the timer resources.
 *
 * @return true on success, false if an error occurs.
 */
void rtc_timer_init(void);

/**
 * @brief Initialize a timer instance.
 *
 * @param[in] timer Timer instance.
 * @param[in] id Timer identifier.
 * @param[in] fn Timer timeout handler.
 */
void rtc_timer_config(rtc_timer_t * timer, uint32_t id, rtc_timer_fn fn);

/**
 * @brief Start a timer. It will expire when the indicated ticks have been
 * elapsed.
 *
 * @param[in] timer Timer instance.
 * @param[in] ticks Number of ticks until the timer expiration.
 */
void rtc_timer_start(rtc_timer_t * timer, uint32_t ticks);

/**
 * @brief Stop a timer if it was in active mode.
 *
 * @param[in] timer Timer instance.
 */
void rtc_timer_stop(rtc_timer_t * timer);

/**
 * @brief Update the active timers with a new tick and call to the timeout
 * handlers of the expired ones.
 *
 * @note This function is called from a interrupt, so the timeout handlers will
 * be called from an interrupt too.
 */
void rtc_timer_tick(void);

#endif // RTC_TIMER_H

/** @} */

/******************************** End of file *********************************/
