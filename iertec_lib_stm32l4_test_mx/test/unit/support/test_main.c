/*******************************************************************************
 * @file test_main.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 12 Jan 2023
 * @brief Test Main program entry.
 ******************************************************************************/

#ifdef TEST_TARGET

#include "stm32l4xx_hal.h"
#include "main.h"

#include <stdlib.h>

extern void initialise_monitor_handles(void);
extern int unity_main(void);
extern void __gcov_dump(void);
extern const char test_file_name[];

/*******************************************************************************
 * Private code
 ******************************************************************************/

static void dump_coverage_data(void)
{
    __gcov_dump();
}

/*******************************************************************************
 * Public code
 ******************************************************************************/

int main(void)
{
    int ret;

    // Semihosting initialization
    initialise_monitor_handles();

    // Call driver init functions
    HAL_Init();
    SystemClock_Config();

    // Run tests
    ret = unity_main();

    dump_coverage_data();

    exit(ret);

    return ret;
}

#endif // TEST_TARGET

/******************************** End of file *********************************/
