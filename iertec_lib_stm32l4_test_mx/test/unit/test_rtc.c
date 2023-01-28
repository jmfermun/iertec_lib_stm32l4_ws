/*******************************************************************************
 * @file test_rtc.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 24 Feb 2021
 * @brief Test for RTC module.
 ******************************************************************************/

#include "rtc.h"

#include "unity.h"

#include <string.h>

/****************************************************************************//*
 * Dependencies
 ******************************************************************************/

const char test_file_name[] = __FILE_NAME__;

// System dependencies
TEST_FILE("system_stm32l4xx.c")
TEST_FILE("stm32l4xx_it.c")
TEST_FILE("sysmem.c")

// HAL dependencies
TEST_FILE("stm32l4xx_hal.c")
TEST_FILE("stm32l4xx_hal_cortex.c")
TEST_FILE("stm32l4xx_hal_pwr_ex.c")
TEST_FILE("stm32l4xx_hal_pwr.c")
TEST_FILE("stm32l4xx_hal_rcc_ex.c")
TEST_FILE("stm32l4xx_hal_rcc.c")
TEST_FILE("stm32l4xx_hal_tim_ex.c")
TEST_FILE("stm32l4xx_hal_tim.c")
TEST_FILE("stm32l4xx_hal_timebase_tim.c")
TEST_FILE("main.c")

// Test support dependencies
TEST_FILE("test_main.c")

// Test dependencies
#include "mock_itf_rtc.h"

/*******************************************************************************
 * Private data
 ******************************************************************************/

static uint32_t stub_seconds;
static uint8_t stub_cseconds;

/*******************************************************************************
 * Private code
 ******************************************************************************/

static void stub_itf_rtc_set_time(uint32_t seconds, uint8_t cseconds)
{
    stub_seconds = seconds;
    stub_cseconds = cseconds;
}

static void stub_itf_rtc_get_time(uint32_t *seconds, uint8_t *cseconds)
{
    *seconds = stub_seconds;
    *cseconds = stub_cseconds;
}

static void util_stub_seconds_increment(uint32_t seconds)
{
    stub_seconds += seconds;
}

static void util_set_inc_get_time(datetime_t *time_set,
                                  const datetime_t *time_exp,
                                  uint32_t time_inc)
{
    datetime_t time_get;

    itf_rtc_set_time_Stub(stub_itf_rtc_set_time);
    itf_rtc_get_time_Stub(stub_itf_rtc_get_time);

    rtc_set_datetime(time_set, true);
    util_stub_seconds_increment(time_inc);
    rtc_get_datetime(&time_get);

    TEST_ASSERT_EQUAL_UINT8(time_exp->year, time_get.year);
    TEST_ASSERT_EQUAL_UINT8(time_exp->month, time_get.month);
    TEST_ASSERT_EQUAL_UINT8(time_exp->day, time_get.day);
    TEST_ASSERT_EQUAL_UINT8(time_exp->hour, time_get.hour);
    TEST_ASSERT_EQUAL_UINT8(time_exp->minutes, time_get.minutes);
    TEST_ASSERT_EQUAL_UINT8(time_exp->seconds, time_get.seconds);
    TEST_ASSERT_EQUAL_INT8(time_exp->delta_hour, time_get.delta_hour);
}

/*******************************************************************************
 * Tests
 ******************************************************************************/

void setUp(void)
{
    stub_seconds = 0;
    stub_cseconds = 0;
}

void test_rtc_set_and_get_timestamp(void)
{
    uint32_t timestamp_set = 1000;
    uint32_t timestamp_get = 0;
    uint8_t centiseconds_get = 255;
    uint32_t timestamp_exp = 1000;

    itf_rtc_set_time_Stub(stub_itf_rtc_set_time);
    itf_rtc_get_time_Stub(stub_itf_rtc_get_time);

    rtc_set_timestamp(timestamp_set);
    rtc_get_timestamp(&timestamp_get, &centiseconds_get);

    TEST_ASSERT_EQUAL_UINT32(timestamp_exp, timestamp_get);
    TEST_ASSERT_EQUAL_UINT8(0, centiseconds_get);
}

void test_rtc_set_and_get_time(void)
{
    datetime_t time_set = {.year = 20, .month = 12, .day =  31,
                           .hour = 22, .minutes = 59, .seconds = 10,
                           .delta_hour = 1};
    datetime_t time_exp = {.year = 20, .month = 12, .day =  31,
                           .hour = 22, .minutes = 59, .seconds = 10,
                           .delta_hour = 1};
    uint32_t seconds_inc = 0;

    util_set_inc_get_time(&time_set, &time_exp, seconds_inc);
}

void test_rtc_set_inc_get_time(void)
{
    datetime_t time_set = {.year = 20, .month = 12, .day =  31,
                           .hour = 22, .minutes = 59, .seconds = 50,
                           .delta_hour = 1};
    datetime_t time_exp = {.year = 21, .month = 1, .day =  1,
                           .hour = 0, .minutes = 0, .seconds = 05,
                           .delta_hour = 1};
    uint32_t seconds_inc = 3615;

    util_set_inc_get_time(&time_set, &time_exp, seconds_inc);
}

void test_rtc_epoch_time(void)
{
    datetime_t time_set = {.year = 21, .month = 9, .day =  12,
                           .hour = 11, .minutes = 37, .seconds = 39,
                           .delta_hour = 2};
    datetime_t time_exp = {.year = 21, .month = 9, .day =  12,
                           .hour = 11, .minutes = 37, .seconds = 39,
                           .delta_hour = 2};
    uint32_t seconds_inc = 0;
    uint32_t epoch_get = 0;
    uint32_t epoch_exp = 1631439459;    // https://www.epochconverter.com/
    uint8_t centiseconds_get = 255;

    util_set_inc_get_time(&time_set, &time_exp, seconds_inc);

    rtc_get_epoch_timestamp(&epoch_get, &centiseconds_get);

    TEST_ASSERT_EQUAL_UINT32(epoch_exp, epoch_get);
    TEST_ASSERT_EQUAL_UINT8(0, centiseconds_get);
}

void test_rtc_compare_datetime(void)
{
    int ret;

    // dt 1 == dt 2
    {
        datetime_t dt_1 = {.year = 20, .month = 12, .day =  31,
                           .hour = 23, .minutes = 59, .seconds = 10,
                           .cseconds = 0, .delta_hour = 0};
        datetime_t dt_2 = {.year = 21, .month = 1, .day =  1,
                           .hour = 0, .minutes = 59, .seconds = 10,
                           .cseconds = 0, .delta_hour = 1};

        ret = rtc_compare_datetime(&dt_1, &dt_2);

        TEST_ASSERT_EQUAL_INT(0, ret);
    }

    // dt 1 < dt 2
    {
        datetime_t dt_1 = {.year = 20, .month = 12, .day =  31,
                           .hour = 23, .minutes = 59, .seconds = 10,
                           .cseconds = 0, .delta_hour = 0};
        datetime_t dt_2 = {.year = 21, .month = 1, .day =  1,
                           .hour = 0, .minutes = 59, .seconds = 11,
                           .cseconds = 0, .delta_hour = 1};

        ret = rtc_compare_datetime(&dt_1, &dt_2);

        TEST_ASSERT_EQUAL_INT(-1, ret);
    }

    // dt 1 > dt 2
    {
        datetime_t dt_1 = {.year = 20, .month = 12, .day =  31,
                           .hour = 23, .minutes = 59, .seconds = 11,
                           .cseconds = 0, .delta_hour = 0};
        datetime_t dt_2 = {.year = 21, .month = 1, .day =  1,
                           .hour = 0, .minutes = 59, .seconds = 10,
                           .cseconds = 0, .delta_hour = 1};

        ret = rtc_compare_datetime(&dt_1, &dt_2);

        TEST_ASSERT_EQUAL_INT(1, ret);
    }
}

/******************************** End of file *********************************/
