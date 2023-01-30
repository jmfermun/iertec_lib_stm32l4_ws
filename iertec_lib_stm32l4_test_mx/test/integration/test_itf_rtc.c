/*******************************************************************************
 * @file test_itf_rtc.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 18 Jan 2023
 * @brief Test for the RTC interface driver.
 ******************************************************************************/

#include "itf_rtc.h"
#include "sys_util.h"

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
TEST_FILE("itf_uart.c")
TEST_FILE("itf_debug.c")
TEST_FILE("debug_util.c")

// Test dependencies
TEST_FILE("sys_util.c")

/****************************************************************************//*
 * Private data
 ******************************************************************************/

const char test_file_name[] = __FILE_NAME__;

static bool wait_flag;

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void rtc_cb(void)
{
    wait_flag = true;
}

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_itf_rtc_init(void)
{
    TEST_ASSERT_TRUE(itf_rtc_init());
}

void test_itf_rtc_set_get(void)
{
    const uint32_t EXP_SEC = 10000;
    uint32_t sec = 0;
    uint8_t csec;

    itf_rtc_set_time(EXP_SEC, 0);
    itf_rtc_get_time(&sec, &csec);

//    TEST_PRINTF("%u, %u", sec, csec);

    TEST_ASSERT_EQUAL(EXP_SEC, sec);
    TEST_ASSERT_TRUE(csec > 0 && csec < 100);
}

void test_itf_rtc_cb(void)
{
    const uint32_t EXP_SEC = 10001;
    const uint32_t EXP_CSEC = 0;
    uint32_t sec;
    uint8_t csec;

    itf_rtc_set_callback(rtc_cb);
    wait_flag = false;

    while (!wait_flag)
    {
        // Wait for the flag to be fired
    }

    itf_rtc_get_time(&sec, &csec);

//    TEST_PRINTF("%u, %u", sec, csec);

    TEST_ASSERT_EQUAL(EXP_SEC, sec);
    TEST_ASSERT_EQUAL(EXP_CSEC, csec);
}

void test_itf_rtc_tick_time(void)
{
    const uint32_t TIME_COUNT = 10;
    const uint32_t TIME_EXP_USEC = 1000000;
    const uint32_t TIME_DELTA_USEC = 5000;
    uint32_t time[TIME_COUNT];

    for (size_t i = 0; i < TIME_COUNT; i++)
    {
        wait_flag = false;

        while (!wait_flag)
        {
            // Wait for the flag to be fired
        }

        time[i] = sys_get_timestamp();
    }

    const uint32_t EXP_SEC = 10011;
    const uint32_t EXP_CSEC = 0;
    uint32_t sec;
    uint8_t csec;

    itf_rtc_get_time(&sec, &csec);

//    TEST_PRINTF("%u, %u", sec, csec);

    TEST_ASSERT_EQUAL(EXP_SEC, sec);
    TEST_ASSERT_EQUAL(EXP_CSEC, csec);

    for (size_t i = 1; i < TIME_COUNT; i++)
    {
//        TEST_PRINTF("%u", time[i] - time[i -1]);
        TEST_ASSERT_UINT32_WITHIN(TIME_DELTA_USEC, TIME_EXP_USEC,
                                  time[i] - time[i -1]);
    }
}

/******************************** End of file *********************************/
