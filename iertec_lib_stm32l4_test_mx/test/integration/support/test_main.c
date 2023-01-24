/*******************************************************************************
 * @file test_main.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Dec 2022
 * @brief Test Main program entry.
 ******************************************************************************/

#include "itf_bsp.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdlib.h>

extern void initialise_monitor_handles(void);
extern int unity_main(void);
extern void __gcov_dump(void);
extern const char test_file_name[];

/*******************************************************************************
 * Constants and macros
 ******************************************************************************/

/** Test task priority. */
#define TEST_MAIN_TASK_PRIORITY         (1)

/** Test task stack size in words (4 bytes). */
#define TEST_MAIN_TASK_STACK_SIZE       (256)

/*******************************************************************************
 * Private code
 ******************************************************************************/

static void dump_coverage_data(void)
{
    __gcov_dump();
}

static void test_main_task_fn(void *parameters)
{
    int ret;

    ret = unity_main();

    dump_coverage_data();
	
    exit(ret);

    // Infinite end loop
    while (1);
}

/*******************************************************************************
 * Public code
 ******************************************************************************/

int main(void)
{
    // Semihosting initialization
    initialise_monitor_handles();

    // Call driver init functions
    itf_bsp_ll_init();

    // Initialize the test main task
    xTaskCreate(test_main_task_fn, "Test main", TEST_MAIN_TASK_STACK_SIZE, NULL,
                TEST_MAIN_TASK_PRIORITY, NULL);

    // Start the RTOS scheduler
    vTaskStartScheduler();

    return 0;
}

/******************************** End of file *********************************/
