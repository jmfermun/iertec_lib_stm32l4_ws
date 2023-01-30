/*******************************************************************************
 * @file task_wait.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Jan 2023
 * @brief Utility to lock a task until other task unlocks it.
 * @ingroup task_wait
 ******************************************************************************/

/**
 * @addtogroup task_wait
 * @{
 */

#include "task_wait.h"
#include "sys_util.h"
#include "debug_util.h"

#include "FreeRTOS.h"
#include "semphr.h"

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** Semaphore to wait for tasks locked. */
static SemaphoreHandle_t h_task_wait_sem_lock;

/** Semaphore to wait for unlocker task. */
static SemaphoreHandle_t h_task_wait_sem_unlock;

/** Identifier of the current task fired. */
static task_id_t task_wait_id_fired;

/****************************************************************************//*
 * Public code
 ******************************************************************************/

void
task_wait_init (void)
{
    task_wait_id_fired     = TASK_ID_NONE;
    h_task_wait_sem_lock   = xSemaphoreCreateBinary();
    h_task_wait_sem_unlock = xSemaphoreCreateBinary();

    DEBUG_ASSERT(NULL != h_task_wait_sem_lock);
    DEBUG_ASSERT(NULL != h_task_wait_sem_unlock);
}

void
task_wait_lock (task_id_t task_id)
{
    DEBUG_ASSERT(task_id < TASK_ID_COUNT);
    DEBUG_ASSERT(task_id > TASK_ID_NONE);

    debug_printf("Task locked: %u\r\n", task_id);

    bool loop_continue = true;

    while (loop_continue)
    {
        if (xSemaphoreTake(h_task_wait_sem_lock, portMAX_DELAY))
        {
            if (task_id == task_wait_id_fired)
            {
                loop_continue = false;

                (void)xSemaphoreGive(h_task_wait_sem_unlock);
            }

            (void)xSemaphoreGive(h_task_wait_sem_lock);

            if (loop_continue)
            {
                // Sleep the task some time to allow lower priority tasks to be
                // unlocked
                sys_sleep_msec(1);
            }
        }
    }

    debug_printf("Task unlocked: %u\r\n", task_id);
}

void
task_wait_unlock (task_id_t task_id)
{
    DEBUG_ASSERT(task_id < TASK_ID_COUNT);
    DEBUG_ASSERT(task_id > TASK_ID_NONE);

    task_wait_id_fired = task_id;

    // Start the locked tasks check
    (void)xSemaphoreGive(h_task_wait_sem_lock);

    // When the locked task is unlocked, it will notify it to the unlocker task
    (void)xSemaphoreTake(h_task_wait_sem_unlock, portMAX_DELAY);

    // Stop the locked tasks check
    (void)xSemaphoreTake(h_task_wait_sem_lock, portMAX_DELAY);

    task_wait_id_fired = TASK_ID_NONE;
}

/** @} */

/******************************** End of file *********************************/
