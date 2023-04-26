/*******************************************************************************
 * @file test_z_dummy.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 12 Jan 2022
 * @brief Unit test for the module z_dummy.
 ******************************************************************************/

#include "unity.h"

/****************************************************************************//*
 * Dependencies
 ******************************************************************************/

const char test_file_name[] = __FILE_NAME__;

//// System dependencies
//TEST_FILE("system_stm32l4xx.c")
//TEST_FILE("stm32l4xx_it.c")
//TEST_FILE("sysmem.c")
//
//// HAL dependencies
//TEST_FILE("stm32l4xx_hal.c")
//TEST_FILE("stm32l4xx_hal_cortex.c")
//TEST_FILE("stm32l4xx_hal_pwr_ex.c")
//TEST_FILE("stm32l4xx_hal_pwr.c")
//TEST_FILE("stm32l4xx_hal_rcc_ex.c")
//TEST_FILE("stm32l4xx_hal_rcc.c")
//TEST_FILE("stm32l4xx_hal_tim_ex.c")
//TEST_FILE("stm32l4xx_hal_tim.c")
//TEST_FILE("stm32l4xx_hal_timebase_tim.c")
//TEST_FILE("main.c")
//
//// Test support dependencies
//TEST_FILE("test_main.c")

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_z_dummy(void)
{
    int z = 0;

    TEST_ASSERT_EQUAL_INT(0, z);

    TEST_PRINTF("Hello %s!", "world");
}

/******************************** End of file *********************************/
