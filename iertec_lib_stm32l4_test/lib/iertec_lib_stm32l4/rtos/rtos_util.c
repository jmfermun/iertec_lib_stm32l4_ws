/*******************************************************************************
 * @file rtos_util.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 25 Jan 2023
 * @brief RTOS utilities.
 * @ingroup rtos_util
 ******************************************************************************/

/**
 * @defgroup rtos_util rtos_util
 * @brief RTOS utilities.
 * @{
 */

#include "debug_util.h"

#include "FreeRTOS.h"
#include "task.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Maximum number of tasks that can be monitored. */
#define RTOS_UTIL_TASK_MAX (5)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** Total number of ticks lost during the execution of the application. */
static uint32_t rtos_util_ticks_dropped = 0;

#ifndef NDEBUG

/** Number of tasks created. */
static UBaseType_t rtos_util_task_count;

/** Status of all tasks. */
static TaskStatus_t rtos_util_task_status[RTOS_UTIL_TASK_MAX];

/** Total time running by all tasks. */
static uint32_t rtos_util_task_run_time;

/** Remaining heap size. */
static size_t rtos_util_free_heap_size;

#endif // NDEBUG

/****************************************************************************//*
 * Public code
 ******************************************************************************/

/**
 * @brief Run time stack overflow checking is performed if config
 * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.
 *
 * This hook function is called if a stack overflow is detected.
 *
 * @param[in] pxTask Handle of the overflowed task.
 * @param[in] pcTaskName Task name.
 */
void
vApplicationStackOverflowHook (TaskHandle_t pxTask, char * pcTaskName)
{
    (void)pxTask;
    (void)pcTaskName;

    DEBUG_ERROR_TRAP();
}

/**
 * @brief Called if a call to pvPortMalloc() fails because there is insufficient
 * free memory available in the FreeRTOS heap.
 *
 * pvPortMalloc() is called internally by FreeRTOS API functions that create
 * tasks, queues or semaphores.
 */
void
vApplicationMallocFailedHook (void)
{
    DEBUG_ERROR_TRAP();
}

/**
 * @brief Called if Tick interrupt is called with sufficient delay to lost some
 * ticks
 *
 * @param[in] ticks Number of ticks lost.
 */
void
vApplicationTraceTicksDropped (uint32_t ticks)
{
    // A minimum of one tick should be reported
    if (0 == ticks)
    {
        ticks = 1;
    }

    rtos_util_ticks_dropped += ticks;
}

/**
 * @brief Print information about the tasks and system state.
 */
void
vApplicationPrintRtosInfo (void)
{
#ifndef NDEBUG
    UBaseType_t ret_count;

    rtos_util_task_count = uxTaskGetNumberOfTasks();

    DEBUG_ASSERT(rtos_util_task_count <= RTOS_UTIL_TASK_MAX);

    ret_count = uxTaskGetSystemState(rtos_util_task_status,
                                     rtos_util_task_count,
                                     &rtos_util_task_run_time);

    DEBUG_ASSERT(ret_count == rtos_util_task_count);

    rtos_util_free_heap_size = xPortGetFreeHeapSize();

    debug_printf("Tasks:\r\n");

    for (size_t i = 0; i < rtos_util_task_count; i++)
    {
        TaskStatus_t * st = &rtos_util_task_status[i];

        debug_printf("\tNummber: %u, Name: %s, Stack: %u\r\n", st->xTaskNumber,
                     st->pcTaskName, st->usStackHighWaterMark);
    }

    debug_printf("System:\r\n\tHeap: %u\r\n\tTicks: %u\r\n",
                 rtos_util_free_heap_size, rtos_util_ticks_dropped);
#else
    // Avoid unused warning in Release build configuration
    (void)rtos_util_ticks_dropped;
#endif // NDEBUG
}

/** @} */

/******************************** End of file *********************************/
