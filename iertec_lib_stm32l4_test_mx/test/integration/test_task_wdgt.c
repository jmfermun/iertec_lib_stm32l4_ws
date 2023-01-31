/*******************************************************************************
 * @file test_task_wdgt.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 08 Dec 2022
 * @brief Test for the task watchdog mechanism.
 ******************************************************************************/

#include "task_wdgt.h"
#include "task_wait.h"
#include "sys_util.h"

#include <FreeRTOS.h>
#include <task.h>

#include "unity.h"
#include "assert_test_helper.h"

/****************************************************************************//*
 * Dependencies
 ******************************************************************************/

// System dependencies
TEST_FILE("system_stm32l4xx.c")
TEST_FILE("stm32l4xx_it.c")
TEST_FILE("sysmem.c")

// FreeRTOS dependencies
TEST_FILE("croutine.c")
TEST_FILE("event_groups.c")
TEST_FILE("list.c")
TEST_FILE("queue.c")
TEST_FILE("stream_buffer.c")
TEST_FILE("tasks.c")
TEST_FILE("timers.c")
TEST_FILE("port.c")
TEST_FILE("heap_4.c")
TEST_FILE("cmsis_os.c")
TEST_FILE("lptimTick.c")
TEST_FILE("rtos_util.c")

// HAL dependencies
TEST_FILE("stm32l4xx_hal.c")
TEST_FILE("stm32l4xx_hal_msp.c")
TEST_FILE("stm32l4xx_hal_cortex.c")
TEST_FILE("stm32l4xx_hal_pwr_ex.c")
TEST_FILE("stm32l4xx_hal_pwr.c")
TEST_FILE("stm32l4xx_hal_rcc_ex.c")
TEST_FILE("stm32l4xx_hal_rcc.c")
TEST_FILE("stm32l4xx_hal_tim_ex.c")
TEST_FILE("stm32l4xx_hal_tim.c")
TEST_FILE("stm32l4xx_hal_timebase_tim.c")
TEST_FILE("stm32l4xx_hal_dma_ex.c")
TEST_FILE("stm32l4xx_hal_dma.c")
TEST_FILE("stm32l4xx_hal_exti.c")
TEST_FILE("stm32l4xx_hal_flash_ex.c")
TEST_FILE("stm32l4xx_hal_flash_ramfunc.c")
TEST_FILE("stm32l4xx_hal_flash.c")
TEST_FILE("stm32l4xx_hal_lptim.c")
TEST_FILE("stm32l4xx_hal_gpio.c")
TEST_FILE("stm32l4xx_hal_i2c_ex.c")
TEST_FILE("stm32l4xx_hal_i2c.c")
TEST_FILE("stm32l4xx_hal_spi_ex.c")
TEST_FILE("stm32l4xx_hal_spi.c")
TEST_FILE("stm32l4xx_hal_uart_ex.c")
TEST_FILE("stm32l4xx_hal_uart.c")
TEST_FILE("dma.c")
TEST_FILE("lptim.c")
TEST_FILE("gpio.c")
TEST_FILE("main.c")
TEST_FILE("spi.c")
TEST_FILE("i2c.c")
TEST_FILE("usart.c")

// Test support dependencies
TEST_FILE("test_main.c")
TEST_FILE("itf_clk.c")
TEST_FILE("itf_io.c")
TEST_FILE("itf_pwr.c")
TEST_FILE("itf_bsp.c")
TEST_FILE("itf_debug.c")
TEST_FILE("debug_util.c")

// Test dependencies
TEST_FILE("itf_uart.c")
TEST_FILE("itf_rtc.c")
TEST_FILE("sys_util.c")
TEST_FILE("task_wait.c")

#include "mock_itf_wdgt.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

#define TASK_TEST_STACK_SIZE  (256)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

const char test_file_name[] = __FILE_NAME__;

static int task_test_priority[TASK_ID_COUNT] = {2, 2, 3, 4};

static uint32_t task_test_count[TASK_ID_COUNT] = {0};
static uint32_t task_test_count_exp[TASK_ID_COUNT] = {0};

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void task_test_thread(void * parameters)
{
    task_id_t task_id = (task_id_t)parameters;

    debug_printf("Task init: %u\r\n", task_id);

    for (;;)
    {
        task_wait_lock(task_id);
        task_test_count[task_id]++;
        task_wdgt_feed(task_id);
    }
}

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void setUp(void)
{
    itf_wdgt_feed_StopIgnore();
}

void test_task_wdgt_init(void)
{
    task_wdgt_init();
    task_wait_init();

    // Construct the test tasks
    for (size_t i = 0; i < TASK_ID_COUNT; i++)
    {
        BaseType_t ret = xTaskCreate(task_test_thread, NULL,
                                     TASK_TEST_STACK_SIZE, (void *)i,
                                     task_test_priority[i], NULL);
        TEST_ASSERT_TRUE(pdPASS == ret);
    }

    sys_sleep_msec(1000);

    TEST_ASSERT_EQUAL_UINT32_ARRAY(task_test_count_exp, task_test_count,
                                   TASK_ID_COUNT);
}

void test_task_wdgt_register(void)
{
    for (size_t i = 0; i < TASK_ID_COUNT; i++)
    {
        task_wdgt_register(i);

        for (size_t j = 0; j < TASK_ID_COUNT; j++)
        {
            if (j == i)
            {
                itf_wdgt_feed_Expect();
            }

            task_wait_unlock(j);
            task_test_count_exp[j]++;

            sys_sleep_msec(10);

            TEST_ASSERT_EQUAL_UINT32_ARRAY(task_test_count_exp, task_test_count,
                                           TASK_ID_COUNT);
        }
    }
}

void test_task_wdgt_assert(void)
{
    TEST_ASSERT_FAIL_ASSERT(task_wdgt_register(TASK_ID_NONE));
    TEST_ASSERT_FAIL_ASSERT(task_wdgt_register(TASK_ID_COUNT));
    TEST_ASSERT_FAIL_ASSERT(task_wdgt_unregister(TASK_ID_NONE));
    TEST_ASSERT_FAIL_ASSERT(task_wdgt_unregister(TASK_ID_COUNT));
    TEST_ASSERT_FAIL_ASSERT(task_wdgt_feed(TASK_ID_NONE));
    TEST_ASSERT_FAIL_ASSERT(task_wdgt_feed(TASK_ID_COUNT));
}

void test_task_wdgt_unregister(void)
{
    for (int i = TASK_ID_COUNT - 1; i >= 0; i--)
    {
        for (size_t j = 0; j < TASK_ID_COUNT; j++)
        {
            if (j == i)
            {
                itf_wdgt_feed_Expect();
                task_wdgt_unregister(i);
            }

            task_wait_unlock(j);
            task_test_count_exp[j]++;

            sys_sleep_msec(10);

            TEST_ASSERT_EQUAL_UINT32_ARRAY(task_test_count_exp, task_test_count,
                                           TASK_ID_COUNT);
        }
    }
}

/******************************** End of file *********************************/
