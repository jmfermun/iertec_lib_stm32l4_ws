/*******************************************************************************
 * @file test_main.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Dec 2022
 * @brief Test Main program entry.
 ******************************************************************************/

#include "itf_bsp.h"

#include "cmsis_os.h"

#include <stdlib.h>

extern void initialise_monitor_handles(void);
extern int unity_main(void);
extern void __gcov_dump(void);
extern const char test_file_name[];

/*******************************************************************************
 * Private data
 ******************************************************************************/

static osThreadId_t h_test_main_task;

static const osThreadAttr_t test_main_task_attributes =
{
    .name = "Test main",
    .stack_size = 1024,
    .priority = (osPriority_t) osPriorityNormal,
};

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

    osKernelInitialize();

    // Initialize the test main task
    h_test_main_task = osThreadNew(test_main_task_fn, NULL,
                                   &test_main_task_attributes);

    // Start the RTOS scheduler
    osKernelStart();

    return 0;
}

/******************************** End of file *********************************/
