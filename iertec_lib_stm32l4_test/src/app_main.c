/*******************************************************************************
 * @file app_main.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Dec 2022
 * @brief Main program entry.
 ******************************************************************************/

#include "itf_bsp.h"
#include "debug_util.h"
#include "test.h"

#include "FreeRTOS.h"
#include "task.h"

/****************************************************************************//*
 * Public code
 ******************************************************************************/

/**
 * @brief Main program entry.
 * @return Always 0 (non-reachable).
 */
int
main (void)
{
    bool ret;

    ret = itf_bsp_ll_init();
    DEBUG_ASSERT(ret);
    (void)ret;

    ret = debug_init();
    DEBUG_ASSERT(ret);
    (void)ret;

    ret = itf_bsp_init();
    DEBUG_ASSERT(ret);
    (void)ret;

    // Initialize the test task
    ret = test_init();
    DEBUG_ASSERT(ret);
    (void)ret;

    // Start scheduler
    vTaskStartScheduler();

    return 0;
}

/******************************** End of file *********************************/
