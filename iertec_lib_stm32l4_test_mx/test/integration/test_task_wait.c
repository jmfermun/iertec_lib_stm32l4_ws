/*******************************************************************************
 * @file test_task_wait.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Jan 2023
 * @brief Test for the task locking mechanism.
 ******************************************************************************/

#include "task_wait.h"

#include <FreeRTOS.h>
#include <task.h>

#include "unity.h"

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

// Test dependencies
#include "mock_itf_uart.h"

TEST_FILE("itf_rtc.c")
TEST_FILE("sys_util.c")

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

#define TASK_STATE_UNKNOWN  (0)
#define TASK_STATE_ACTIVE   (1)
#define TASK_STATE_INACTIVE (2)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

const char test_file_name[] = __FILE_NAME__;

static uint8_t task_active[TASK_ID_COUNT];
static uint8_t exp_task_active[TASK_ID_COUNT];

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void test_task_fn(void *parameters)
{
    task_id_t my_task_id = (task_id_t)parameters;

    task_active[my_task_id] = TASK_STATE_INACTIVE;

    task_wait_lock(my_task_id);

    task_active[my_task_id] = TASK_STATE_ACTIVE;

    // Lock the task permanently for the rest of the test
    for (;;)
    {
        sys_sleep_msec(60000);
    }
}

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_task_wait_init(void)
{
    for (size_t i = 0; i < TASK_ID_COUNT; i++)
    {
        task_active[i] = TASK_STATE_UNKNOWN;
    }

    task_wait_init();

    TEST_ASSERT_TRUE(xTaskCreate(test_task_fn, "T0", 128, TASK_ID_0, 2, NULL) == pdPASS);
    TEST_ASSERT_TRUE(xTaskCreate(test_task_fn, "T1", 128, TASK_ID_1, 2, NULL) == pdPASS);
    TEST_ASSERT_TRUE(xTaskCreate(test_task_fn, "T2", 128, TASK_ID_2, 3, NULL) == pdPASS);
    TEST_ASSERT_TRUE(xTaskCreate(test_task_fn, "T3", 128, TASK_ID_3, 4, NULL) == pdPASS);
}

void test_task_wait_inactive(void)
{
    for (size_t i = 0; i < TASK_ID_COUNT; i++)
    {
        exp_task_active[i] = TASK_STATE_INACTIVE;
    }

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_task_active, task_active, TASK_ID_COUNT);
}


void test_task_wait_active(void)
{
    task_wait_unlock(TASK_ID_1);
    exp_task_active[TASK_ID_1] = TASK_STATE_ACTIVE;
    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_task_active, task_active, TASK_ID_COUNT);

    task_wait_unlock(TASK_ID_0);
    exp_task_active[TASK_ID_0] = TASK_STATE_ACTIVE;
    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_task_active, task_active, TASK_ID_COUNT);

    task_wait_unlock(TASK_ID_2);
    exp_task_active[TASK_ID_2] = TASK_STATE_ACTIVE;
    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_task_active, task_active, TASK_ID_COUNT);

    task_wait_unlock(TASK_ID_3);
    exp_task_active[TASK_ID_3] = TASK_STATE_ACTIVE;
    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_task_active, task_active, TASK_ID_COUNT);
}

/******************************** End of file *********************************/
