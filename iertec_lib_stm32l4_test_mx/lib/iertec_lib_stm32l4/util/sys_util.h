/*******************************************************************************
 * @file sys_util.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 16 Jan 2023
 * @brief System utilities.
 * @ingroup sys_util
 ******************************************************************************/

/**
 * @defgroup sys_util sys_util
 * @brief System utilities.
 * @{
 */

#ifndef SYS_UTIL_H
#define SYS_UTIL_H

#include "FreeRTOS.h"

#include <stdint.h>

/** Convert time from milliseconds to system ticks. */
#define SYS_MSEC_TO_TICKS(X) (pdMS_TO_TICKS(X))

/** Convert time from system ticks to milliseconds. */
#define SYS_TICKS_TO_MSEC(X) \
    ((TickType_t)(X) * ((TickType_t)1000u / (TickType_t)configTICK_RATE_HZ))

/** Enter into a critical section. */
#define SYS_ENTER_CRITICAL   taskENTER_CRITICAL

/** Exit from a citical section. */
#define SYS_EXIT_CRITICAL    taskEXIT_CRITICAL

/**
 * @brief Get the reset cause.
 *
 * @param[out] code Code number identifying the reset cause.
 * @param[out] str Pointer to a constant string describing the reset cause.
 */
void sys_get_reset_source(uint32_t * code, const char ** str);

/**
 * @brief Get the current timestamp since uC start up.
 *
 * @return Timestamp in us.
 */
uint32_t sys_get_timestamp(void);

/**
 * @brief Compute the time difference between calls done to this function.
 *
 * @param[in,out] ticks Variable used to store the clock tick mark of the previous call.
 * @param[in,out] max_diff Maximum difference in us of the calls done.
 *
 * @return Time difference in us.
 */
uint32_t sys_time_diff(uint32_t * ticks, uint32_t * max_diff);

/**
 * @brief Do a delay in microseconds blocking the CPU.
 *
 * @param[in] usec Duration of the delay in microseconds.
 */
void sys_delay_usec(uint32_t usec);

/**
 * @brief Do a sleep in milliseconds blocking the current task.
 *
 * @param[in] msec Duration of the sleep in milliseconds.
 */
void sys_sleep_msec(uint32_t msec);

/**
 * @brief Delay a task until a specified time. This function can be used by
 * periodic tasks to ensure a constant execution frequency.
 *
 * @param[in,out] prev_ticks Pointer to a variable that holds the time at which the task
 * was last unblocked. The variable must be initialized with the current tick
 * prior to its first use. TO do it, call this function with inc_msec equl to 0.
 * @param[in] inc_msec The cycle time period in milliseconds.
 *
 * @note Function based on FreeRTOS function vTaskDelayUntil.
 */
void sys_sleep_until_msec(uint32_t * prev_ticks, uint32_t inc_msec);

/**
 * @brief Reset the uC by software.
 */
void sys_reset(void);

#endif // SYS_UTIL_H

/** @} */

/******************************** End of file *********************************/
