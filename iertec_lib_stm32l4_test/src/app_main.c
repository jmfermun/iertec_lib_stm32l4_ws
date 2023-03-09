/*******************************************************************************
 * @file app_main.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Dec 2022
 * @brief Main program entry.
 ******************************************************************************/

#include "itf_bsp.h"
#include "debug_util.h"

#include "FreeRTOS.h"
#include "task.h"

extern void MX_FREERTOS_Init(void);

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

    // Call init function for freertos objects (in freertos.c)
    MX_FREERTOS_Init();

    // Start scheduler
    vTaskStartScheduler();

    return 0;
}

/******************************** End of file *********************************/
