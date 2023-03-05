/*******************************************************************************
 * @file task_wdgt.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Jan 2023
 * @brief Watchdog that checks the activity of different tasks.
 * @ingroup task_wdgt
 ******************************************************************************/

/**
 * @defgroup task_wdgt task_wdgt
 * @brief Watchdog that checks the activity of different tasks.
 * @{
 */

#ifndef TASK_WDGT_H
#define TASK_WDGT_H

#include "task_def.h"

#include <stdint.h>

/**
 * @brief Initialize the task watchdog resources.
 */
void task_wdgt_init(void);

/**
 * @brief Register a task in the watchdog monitoring.
 *
 * @param[in] task_id Task identifier.
 */
void task_wdgt_register(task_id_t task_id);

/**
 * @brief Unregister a task from the watchdog monitoring.
 *
 * @param[in] task_id Task identifier.
 */
void task_wdgt_unregister(task_id_t task_id);

/**
 * @brief Allow a task to feed the watchdog. All register tasks must feed
 * periodically the watchdog before the timeout expires.
 *
 * @param[in] task_id Task identifier.
 */
void task_wdgt_feed(task_id_t task_id);

#endif // TASK_WDGT_H

/** @} */

/******************************** End of file *********************************/
