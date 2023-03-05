/*******************************************************************************
 * @file task_wdgt.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Jan 2023
 * @brief Watchdog that checks the activity of different tasks.
 * @ingroup task_wdgt
 ******************************************************************************/

/**
 * @addtogroup task_wdgt
 * @{
 */

#include "task_wdgt.h"
#include "itf_wdgt.h"
#include "sys_util.h"
#include "debug_util.h"

#include "FreeRTOS.h"
#include "semphr.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Maximum number of tasks that can be registered. */
#define TASK_WDGT_ID_MAX (32)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** Mutex to access to the data from different threads. */
static SemaphoreHandle_t h_task_wdgt_mutex;

/** Each individual bit represents a registered task. */
static uint32_t registered_tasks;

/** Each individual bit represents a task that feeds the watchdog. */
static uint32_t fed_tasks;

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Check that all registered tasks have fed the watchdog. If it is the
 * case, the watchdog counter is reset.
 */
static void task_wdgt_check(void);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

void
task_wdgt_init (void)
{
    DEBUG_ASSERT_STATIC(TASK_ID_COUNT < TASK_WDGT_ID_MAX);

    registered_tasks  = 0;
    fed_tasks         = 0;
    h_task_wdgt_mutex = xSemaphoreCreateMutex();

    DEBUG_ASSERT(NULL != h_task_wdgt_mutex);
}

void
task_wdgt_register (task_id_t task_id)
{
    DEBUG_ASSERT(task_id < TASK_ID_COUNT);
    DEBUG_ASSERT(task_id > TASK_ID_NONE);

    if (xSemaphoreTake(h_task_wdgt_mutex, portMAX_DELAY))
    {
        registered_tasks |= (1u << task_id);

        (void)xSemaphoreGive(h_task_wdgt_mutex);
    }
}

void
task_wdgt_unregister (task_id_t task_id)
{
    DEBUG_ASSERT(task_id < TASK_ID_COUNT);
    DEBUG_ASSERT(task_id > TASK_ID_NONE);

    if (xSemaphoreTake(h_task_wdgt_mutex, portMAX_DELAY))
    {
        registered_tasks &= ~(1u << task_id);
        fed_tasks        &= ~(1u << task_id);
        task_wdgt_check();

        (void)xSemaphoreGive(h_task_wdgt_mutex);
    }
}

void
task_wdgt_feed (task_id_t task_id)
{
    DEBUG_ASSERT(task_id < TASK_ID_COUNT);
    DEBUG_ASSERT(task_id > TASK_ID_NONE);

    uint32_t task_mask = 1u << task_id;

    if (xSemaphoreTake(h_task_wdgt_mutex, portMAX_DELAY))
    {
        if (registered_tasks & task_mask)
        {
            fed_tasks |= task_mask;
            task_wdgt_check();
        }

        (void)xSemaphoreGive(h_task_wdgt_mutex);
    }
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void
task_wdgt_check (void)
{
    if ((fed_tasks & registered_tasks) == registered_tasks)
    {
        // All the tasks have been fed!
        itf_wdgt_feed();
        fed_tasks = 0;
    }
}

/** @} */

/******************************** End of file *********************************/
