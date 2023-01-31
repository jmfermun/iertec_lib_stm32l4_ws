/*******************************************************************************
 * @file test_itf_uart.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 16 Jan 2022
 * @brief Test for the UART interface driver.
 *
 * The test connects the UART in a loop-back configuration, so the data sent by
 * the UART interface is received by the same interface.
 *
 * Two tasks are used, one for transmission of data and another for reception of
 * data. The reception task is slower than the transmission task to test the
 * hardware flow control functionality.
 *
 * Needed connections:
 * - PA9 --> PA10
 * - PA12 --> PA11
 ******************************************************************************/

#include "itf_uart.h"
#include "itf_rtc.h"
#include "sys_util.h"

#include <FreeRTOS.h>
#include <task.h>

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
TEST_FILE("itf_rtc.c")
TEST_FILE("sys_util.c")

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

#define TASK_SENDER_PRIORITY   (1)
#define TASK_SENDER_STACK_SIZE (256)

#define TX_DATA_VALUE          "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\r\n"
#define DATA_SIZE              (256)

#define PERIOD_STOP_MS         (0)
#define PERIOD_FAST_MS         (25)
#define PERIOD_SLOW_MS         (250)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

static uint8_t tx_data[DATA_SIZE];
static uint8_t rx_data[DATA_SIZE];

static uint32_t period_sender = PERIOD_STOP_MS;
static uint32_t period_receiver = PERIOD_STOP_MS;
static uint32_t len_sender = 0;
static uint32_t len_receiver = 0;

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void task_sender_fn(void *parameters)
{
    uint32_t last_wake_time;
    bool write_ret = true;

    // Get the current tick count
    sys_sleep_until_msec(&last_wake_time, 0);

    for (;;)
    {
        if (period_sender != PERIOD_STOP_MS)
        {
            if (write_ret)
            {
                write_ret = itf_uart_write(H_ITF_UART_0, (char*)tx_data);
                len_sender += strlen(TX_DATA_VALUE);
            }

            // Sleep until the next period
            sys_sleep_until_msec(&last_wake_time, period_sender);
        }
        else
        {
            sys_sleep_msec(10);
            sys_sleep_until_msec(&last_wake_time, 0);
        }
    }
}

static void task_receiver_fn(void)
{
    uint32_t last_wake_time;
    size_t read_count;

    sys_sleep_msec(period_receiver);

    // Get the current tick count
    sys_sleep_until_msec(&last_wake_time, 0);

    for (unsigned int i = 0; i < 10; i++)
    {
        memset(rx_data, 0, DATA_SIZE);

        read_count = itf_uart_read(H_ITF_UART_0, (char*)rx_data, DATA_SIZE);
        len_receiver += read_count - 1;

        TEST_ASSERT_EQUAL_size_t(strlen(TX_DATA_VALUE) + 1, read_count);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(TX_DATA_VALUE, rx_data, strlen(TX_DATA_VALUE));

        // Sleep until the next period
        sys_sleep_until_msec(&last_wake_time, period_receiver);
    }
}

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_itf_uart_init(void)
{
    strcpy((char*)tx_data, TX_DATA_VALUE);

    TEST_ASSERT_TRUE(itf_rtc_init());
    TEST_ASSERT_FALSE(itf_uart_init(H_ITF_UART_COUNT));
    TEST_ASSERT_TRUE(itf_uart_init(H_ITF_UART_0));
    itf_uart_read_enable(H_ITF_UART_0);

    // Initialize the sender task
    BaseType_t h_task = xTaskCreate(task_sender_fn, "Sender",
                                    TASK_SENDER_STACK_SIZE, NULL,
                                    TASK_SENDER_PRIORITY, NULL);
    TEST_ASSERT_TRUE(pdPASS == h_task);
}

void test_itf_uart_flow_control_none(void)
{
    period_sender = PERIOD_SLOW_MS;
    period_receiver = PERIOD_FAST_MS;

    task_receiver_fn();

    period_sender = PERIOD_STOP_MS;
}

void test_itf_uart_flow_control_hardware(void)
{
    period_sender = PERIOD_SLOW_MS;
    period_receiver = PERIOD_FAST_MS;

    task_receiver_fn();

    period_sender = PERIOD_STOP_MS;
}

void test_itf_uart_read_bin_timeout(void)
{
    memset(rx_data, 0, DATA_SIZE);

    // Wait some time sending data
    period_sender = PERIOD_SLOW_MS;
    sys_sleep_msec(PERIOD_SLOW_MS);
    period_sender = PERIOD_STOP_MS;

    uint32_t time = sys_get_timestamp();

    size_t read_count = itf_uart_read_bin(H_ITF_UART_0, (char *)rx_data,
                                          DATA_SIZE);
    len_receiver += read_count;

    time = (sys_get_timestamp() - time) / 1000;

    TEST_ASSERT_UINT32_WITHIN(5, 1000, time);
    TEST_ASSERT_TRUE(read_count > 0 && read_count < DATA_SIZE);
    TEST_ASSERT_EQUAL(len_sender, len_receiver);
}

void test_itf_uart_read_timeout(void)
{
    char exp_data[] = "A\r\n";
    const size_t exp_len = sizeof(exp_data);
    size_t rx_len;

    for (size_t i_times = 0; i_times < 2; i_times++)
    {
        // Send the frame 5 times
        for (size_t i = 0; i < 5; i++)
        {
            TEST_ASSERT_TRUE(itf_uart_write(H_ITF_UART_0, exp_data));
        }

        // Read the frame 5 times
        for (size_t i = 0; i < 5; i++)
        {
            memset(rx_data, 0, exp_len);

            rx_len = itf_uart_read(H_ITF_UART_0, (char*)rx_data, DATA_SIZE);

            TEST_ASSERT_EQUAL(exp_len, rx_len);
            TEST_ASSERT_EQUAL_STRING(exp_data, rx_data);
        }

        // Read one more frame and expect a timeout
        TEST_ASSERT_TRUE(itf_uart_write(H_ITF_UART_0, "A"));

        uint32_t time = sys_get_timestamp();
        rx_len = itf_uart_read(H_ITF_UART_0, (char*)rx_data, DATA_SIZE);
        time = (sys_get_timestamp() - time) / 1000;

        TEST_ASSERT_UINT32_WITHIN(5, 1000, time);
        TEST_ASSERT_EQUAL(0, rx_len);
    }
}

void test_itf_uart_read_count(void)
{
    char exp_data[] = "0123456789\r\n";
    size_t exp_len = strlen(exp_data);

    TEST_ASSERT_EQUAL(0, itf_uart_read_count(H_ITF_UART_0));
    TEST_ASSERT_TRUE(itf_uart_write_bin(H_ITF_UART_0, exp_data, exp_len));
    TEST_ASSERT_EQUAL(exp_len, itf_uart_read_count(H_ITF_UART_0));
    TEST_ASSERT_TRUE(itf_uart_read_bin(H_ITF_UART_0, exp_data, exp_len / 2));
    TEST_ASSERT_EQUAL(exp_len - exp_len / 2, itf_uart_read_count(H_ITF_UART_0));
    TEST_ASSERT_TRUE(itf_uart_read_bin(H_ITF_UART_0, exp_data, exp_len - exp_len / 2));
    TEST_ASSERT_EQUAL(0, itf_uart_read_count(H_ITF_UART_0));
}

void test_itf_uart_deinit(void)
{
    TEST_ASSERT_FALSE(itf_uart_deinit(H_ITF_UART_COUNT));
    TEST_ASSERT_TRUE(itf_uart_deinit(H_ITF_UART_0));
    TEST_ASSERT_FALSE(itf_uart_deinit(H_ITF_UART_0));
}

/******************************** End of file *********************************/
