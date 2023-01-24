/*******************************************************************************
 * @file test_itf_io.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 17 Jan 2023
 * @brief Test for the I/O interface driver.
 *
 * Needed connections:
 * - PB1 --> PA8
 ******************************************************************************/

#include "itf_io.h"

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

/****************************************************************************//*
 * Private data
 ******************************************************************************/

const char test_file_name[] = __FILE_NAME__;

static uint8_t interrupt_val;

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void interrupt_isr(void)
{
    interrupt_val = itf_io_get_value(H_ITF_IO_IN_1);
}

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_itf_io_init(void)
{
    TEST_ASSERT_TRUE(itf_io_init());
}

void test_itf_io_set_get(void)
{
    for (int i = 0; i < 10; i++)
    {
        uint8_t val;

        itf_io_set_value(H_ITF_IO_OUT_1, ITF_IO_HIGH);
        val = itf_io_get_value(H_ITF_IO_IN_1);
        TEST_ASSERT_EQUAL(ITF_IO_HIGH, val);

        itf_io_set_value(H_ITF_IO_OUT_1, ITF_IO_LOW);
        val = itf_io_get_value(H_ITF_IO_IN_1);
        TEST_ASSERT_EQUAL(ITF_IO_LOW, val);
    }
}

void test_itf_io_toogle(void)
{
    // Clear state
    itf_io_set_value(H_ITF_IO_OUT_1, ITF_IO_LOW);

    for (int i = 0; i < 10; i++)
    {
        uint8_t val;

        itf_io_toggle_value(H_ITF_IO_OUT_1);
        val = itf_io_get_value(H_ITF_IO_IN_1);
        TEST_ASSERT_EQUAL(ITF_IO_HIGH, val);

        itf_io_toggle_value(H_ITF_IO_OUT_1);
        val = itf_io_get_value(H_ITF_IO_IN_1);
        TEST_ASSERT_EQUAL(ITF_IO_LOW, val);
    }
}

void test_itf_io_interrupt_set(void)
{
    // Clear state
    interrupt_val = 0xFF;
    itf_io_set_value(H_ITF_IO_OUT_1, ITF_IO_LOW);
    itf_io_set_int_cb(H_ITF_IO_IN_1, interrupt_isr);

    for (int i = 0; i < 10; i++)
    {
        itf_io_toggle_value(H_ITF_IO_OUT_1);
        TEST_ASSERT_EQUAL(ITF_IO_HIGH, interrupt_val);

        itf_io_toggle_value(H_ITF_IO_OUT_1);
        TEST_ASSERT_EQUAL(ITF_IO_LOW, interrupt_val);
    }
}

void test_itf_io_interrupt_unset(void)
{
    // Clear state
    itf_io_set_int_cb(H_ITF_IO_IN_1, NULL);
    interrupt_val = 0xFF;
    itf_io_set_value(H_ITF_IO_OUT_1, ITF_IO_LOW);

    for (int i = 0; i < 10; i++)
    {
        uint8_t val;

        itf_io_toggle_value(H_ITF_IO_OUT_1);
        val = itf_io_get_value(H_ITF_IO_IN_1);
        TEST_ASSERT_EQUAL(ITF_IO_HIGH, val);
        TEST_ASSERT_EQUAL(0xFF, interrupt_val);

        itf_io_toggle_value(H_ITF_IO_OUT_1);
        val = itf_io_get_value(H_ITF_IO_IN_1);
        TEST_ASSERT_EQUAL(ITF_IO_LOW, val);
        TEST_ASSERT_EQUAL(0xFF, interrupt_val);
    }
}

/******************************** End of file *********************************/
