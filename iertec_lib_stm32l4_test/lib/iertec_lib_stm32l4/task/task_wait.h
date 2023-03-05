/*******************************************************************************
 * @file task_wait.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Jan 2023
 * @brief Utility to lock a task until other task unlocks it.
 * @ingroup task_wait
 ******************************************************************************/

/**
 * @defgroup task_wait task_wait
 * @brief Utility to lock a task until other task unlocks it.
 * @{
 */

#ifndef TASK_WAIT_H
#define TASK_WAIT_H

#include "task_def.h"

#include <stdint.h>

/**
 * @brief Initialize the wait task mechanism.
 */
void task_wait_init(void);

/**
 * @brief Lock the calling task wiating for the specified task ID to be
 * unlocked.
 *
 * @note This function can be used by several task at the same time.
 *
 * @param[in] task_id Identifier of the task to be locked.
 */
void task_wait_lock(task_id_t task_id);

/**
 * @brief Lock the calling task waiting for the specified task ID to be
 * unlocked.
 *
 * @note This function can be only used by one task at the sae time.
 *
 * @param[in] task_id Identifier of the task to be unlocked.
 */
void task_wait_unlock(task_id_t task_id);

#endif // TASK_WAIT_H

/** @} */

/******************************** End of file *********************************/
