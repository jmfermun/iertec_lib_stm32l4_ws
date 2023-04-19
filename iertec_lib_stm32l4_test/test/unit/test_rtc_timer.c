/*******************************************************************************
 * @file test_rtc_timer.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 19 Apr 2023
 * @brief Test for the Generic timers that can use any tick interrupt source.
 ******************************************************************************/

#include "rtc_timer.h"

#include "unity.h"
#include "assert_test_helper.h"

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

#include "mock_sys_util.h"
#include "mock_portmacro.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

#define TIMER_COUNT      (5)
#define EXP_TIMER_ID_MAX (10)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

rtc_timer_t timer[TIMER_COUNT];
static uint32_t exp_timer_id[EXP_TIMER_ID_MAX];
static size_t exp_timer_idx = 0;
static size_t exp_timer_count = 0;

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

static void timer_cb(rtc_timer_t * timer);

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_rtc_timer_init(void)
{
    rtc_timer_init();
}

void test_rtc_timer_assert(void)
{
    TEST_ASSERT_FAIL_ASSERT(rtc_timer_config(NULL, 0, 0));
    TEST_ASSERT_FAIL_ASSERT(rtc_timer_start(NULL, 0));
    TEST_ASSERT_FAIL_ASSERT(rtc_timer_stop(NULL));
}

void test_rtc_timer_config(void)
{
    for (size_t i = 0; i < TIMER_COUNT; i++)
    {
        vPortEnterCritical_Expect();
        vPortExitCritical_Expect();
        rtc_timer_config(&timer[i], i, timer_cb);
    }

    for (size_t i = 0; i < TIMER_COUNT; i++)
    {
        TEST_ASSERT_EQUAL(i, timer[i].id);
        TEST_ASSERT_EQUAL(timer_cb, timer[i].fn);
    }
}

void test_rtc_timer_start_and_expire_single(void)
{
    uint32_t t_ticks = 10;
    uint32_t t_id  = 2;

    // Start the timer
    vPortEnterCritical_Expect();
    vPortExitCritical_Expect();
    rtc_timer_start(&timer[t_id], t_ticks);

    // Call the tick function to consume the needed time
    exp_timer_count = 0;
    exp_timer_idx = 0;

    TEST_PRINTF("Wait %u ticks", t_ticks);

    while (--t_ticks > 0)
    {
        rtc_timer_tick();
    }

    // Call one more time the tick function and check that the timer has expired
    exp_timer_id[exp_timer_count++] = t_id;

    rtc_timer_tick();

    TEST_ASSERT_EQUAL(exp_timer_count, exp_timer_idx);
}

void test_rtc_timer_start_and_expire_multi(void)
{
    uint32_t t_id[TIMER_COUNT] =
    {
        4,
        1,
        0,
        2,
        3
    };
    uint32_t t_ticks[TIMER_COUNT] =
    {
        15,
        0,
        10,
        15,
        20,
    };

    // Start the timers
    for (size_t i = 0; i < TIMER_COUNT; i++)
    {
        vPortEnterCritical_Expect();
        vPortExitCritical_Expect();
        rtc_timer_start(&timer[t_id[i]], t_ticks[i]);
    }

    // Reorder timers with their execution order
    t_id[0] = 1;
    t_id[1] = 0;
    t_id[2] = 4;

    t_ticks[0] = 1;
    t_ticks[1] = 10;
    t_ticks[2] = 15;

    for (size_t i = 0; i < TIMER_COUNT; i++)
    {
        uint32_t ticks;

        if (i == 0)
        {
            ticks = t_ticks[i];
        }
        else
        {
            ticks = t_ticks[i] - t_ticks[i - 1];
        }

        // Call the tick function to consume the needed time
        exp_timer_count = 0;
        exp_timer_idx = 0;

        TEST_PRINTF("Wait %u ticks", ticks);

        while (--ticks > 0)
        {
            rtc_timer_tick();
        }

        // Call one more time the tick function and check that the timer has expired
        exp_timer_id[exp_timer_count++] = t_id[i];

        if (i == 2)
        {
            // Special case with 2 consecutive timers with the same ticks
            i++;
            exp_timer_id[exp_timer_count++] = t_id[i];
        }

        rtc_timer_tick();

        TEST_ASSERT_EQUAL(exp_timer_count, exp_timer_idx);
    }
}

void test_rtc_timer_start_and_stop_single(void)
{
    uint32_t t_ticks = 10;
    uint32_t t_id  = 2;

    // Start the timer
    vPortEnterCritical_Expect();
    vPortExitCritical_Expect();
    rtc_timer_start(&timer[t_id], t_ticks);

    // Call the tick function to consume the needed time
    exp_timer_count = 0;
    exp_timer_idx = 0;

    TEST_PRINTF("Wait %u ticks", t_ticks - 5);

    while (--t_ticks > 5)
    {
        rtc_timer_tick();
    }

    // Stop the timer
    vPortEnterCritical_Expect();
    vPortExitCritical_Expect();
    rtc_timer_stop(&timer[t_id]);

    TEST_PRINTF("Wait %u ticks", t_ticks);

    while (--t_ticks > 0)
    {
        rtc_timer_tick();
    }

    // Call one more time the tick function and check that the timer has not
    // expired
    rtc_timer_tick();

    TEST_ASSERT_EQUAL(exp_timer_count, exp_timer_idx);
}

void test_rtc_timer_start_and_stop_multi(void)
{
    uint32_t t_id[TIMER_COUNT] =
    {
        1,
        0,
        4,
        2,
        3
    };
    uint32_t t_ticks[TIMER_COUNT] =
    {
        5,
        10,
        15,
        15,
        20,
    };

    // Start the timers
    for (size_t i = 0; i < TIMER_COUNT; i++)
    {
        vPortEnterCritical_Expect();
        vPortExitCritical_Expect();
        rtc_timer_start(&timer[t_id[i]], t_ticks[i]);
    }

    for (size_t i = 0; i < TIMER_COUNT; i++)
    {
        uint32_t ticks;

        if (i == 0)
        {
            ticks = t_ticks[i];
        }
        else
        {
            ticks = t_ticks[i] - t_ticks[i - 1];
        }

        // Stop timers
        if (i == 0)
        {
            vPortEnterCritical_Expect();
            vPortExitCritical_Expect();
            rtc_timer_stop(&timer[t_id[2]]);
        }
        else if (i == 1)
        {
            vPortEnterCritical_Expect();
            vPortExitCritical_Expect();
            rtc_timer_stop(&timer[t_id[i]]);
        }

        // Call the tick function to consume the needed time
        exp_timer_count = 0;
        exp_timer_idx = 0;

        TEST_PRINTF("Wait %u ticks", ticks);

        while (--ticks > 0)
        {
            rtc_timer_tick();
        }

        if ((i != 1) && (i != 2))
        {
            // Call one more time the tick function and check that the timer has expired
            exp_timer_id[exp_timer_count++] = t_id[i];
        }

        if (i == 2)
        {
            // Special case with 2 consecutive timers with the same ticks
            i++;
            exp_timer_id[exp_timer_count++] = t_id[i];
        }

        rtc_timer_tick();

        TEST_ASSERT_EQUAL(exp_timer_count, exp_timer_idx);
    }
}

void test_rtc_timer_start_and_restart_single(void)
{
    uint32_t t_ticks = 10;
    uint32_t t_id  = 2;

    // Start the timer
    vPortEnterCritical_Expect();
    vPortExitCritical_Expect();
    rtc_timer_start(&timer[t_id], t_ticks);

    // Call the tick function to consume the needed time
    uint32_t ticks = t_ticks;

    exp_timer_count = 0;
    exp_timer_idx = 0;

    TEST_PRINTF("Wait %u ticks", ticks);

    while (--ticks > 0)
    {
        rtc_timer_tick();
    }

    // Restart the timer
    vPortEnterCritical_Expect();
    vPortEnterCritical_Expect();
    vPortExitCritical_Expect();
    vPortExitCritical_Expect();
    rtc_timer_start(&timer[t_id], t_ticks);

    // Call the tick function to consume the needed time
    ticks = t_ticks;

    TEST_PRINTF("Wait %u ticks", ticks);

    while (--ticks > 0)
    {
        rtc_timer_tick();
    }

    // Call one more time the tick function and check that the timer has expired
    exp_timer_id[exp_timer_count++] = t_id;

    rtc_timer_tick();

    TEST_ASSERT_EQUAL(exp_timer_count, exp_timer_idx);
}

void test_rtc_timer_start_and_restart_multi(void)
{
    uint32_t t_id[TIMER_COUNT] =
    {
        1,
        0,
        4,
        2,
        3
    };
    uint32_t t_ticks[TIMER_COUNT] =
    {
        5,
        10,
        15,
        15,
        20,
    };

    // Start the timers
    for (size_t i = 0; i < TIMER_COUNT; i++)
    {
        vPortEnterCritical_Expect();
        vPortExitCritical_Expect();
        rtc_timer_start(&timer[t_id[i]], t_ticks[i]);
    }

    for (size_t i = 0; i < TIMER_COUNT; i++)
    {
        uint32_t ticks;

        if (i == 0)
        {
            ticks = t_ticks[i];
        }
        else
        {
            ticks = t_ticks[i] - t_ticks[i - 1];
        }

        // Restart timers
        if (i == 2)
        {
            vPortEnterCritical_Expect();
            vPortEnterCritical_Expect();
            vPortExitCritical_Expect();
            vPortExitCritical_Expect();
            rtc_timer_start(&timer[t_id[i]], t_ticks[i]);
        }

        // Call the tick function to consume the needed time
        exp_timer_count = 0;
        exp_timer_idx = 0;

        TEST_PRINTF("Wait %u ticks", ticks);

        while (--ticks > 0)
        {
            rtc_timer_tick();
        }

        // Call one more time the tick function and check that the timer has expired
        if (i != 2)
        {
            exp_timer_id[exp_timer_count++] = t_id[i];
        }

        if (i == 2)
        {
            // Special case with 2 consecutive timers with the same ticks
            i++;
            exp_timer_id[exp_timer_count++] = t_id[i];
        }

        rtc_timer_tick();

        TEST_ASSERT_EQUAL(exp_timer_count, exp_timer_idx);
    }

    // Wait for the restarted timer to expire
    uint32_t ticks = 5;

    // Call the tick function to consume the needed time
    exp_timer_count = 0;
    exp_timer_idx = 0;

    TEST_PRINTF("Wait %u ticks", ticks);

    while (--ticks > 0)
    {
        rtc_timer_tick();
    }

    exp_timer_id[exp_timer_count++] = t_id[2];

    rtc_timer_tick();

    TEST_ASSERT_EQUAL(exp_timer_count, exp_timer_idx);
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void timer_cb(rtc_timer_t * timer)
{
    TEST_PRINTF("Timer %u expired", timer->id);

    TEST_ASSERT_TRUE(exp_timer_count > 0);
    TEST_ASSERT_EQUAL(exp_timer_id[exp_timer_idx], timer->id);

    exp_timer_idx++;
}

/******************************** End of file *********************************/
