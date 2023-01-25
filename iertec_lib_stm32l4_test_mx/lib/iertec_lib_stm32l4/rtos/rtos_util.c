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
 * Public code
 ******************************************************************************/

/**
 * @brief Run time stack overflow checking is performed if config
 * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.
 *
 * This hook function is called if a stack overflow is detected.
 */
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
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
void vApplicationMallocFailedHook(void)
{
    DEBUG_ERROR_TRAP();
}

/** @} */

/******************************** End of file *********************************/
