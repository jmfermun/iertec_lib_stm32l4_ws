/*******************************************************************************
 * @file test_itf_spi.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Dec 2022
 * @brief Test for the SPI interface driver.
 *
 * The test connects a SPI interface in a loop-back configuration, so the data
 * sent by the SPI interface is received by the same interface.
 *
 * Needed connections:
 * - PA7 -> PA6
 * - PB1 --> PA8
 ******************************************************************************/

#include "itf_spi.h"
#include "itf_io.h"

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
TEST_FILE("itf_debug_none.c")
TEST_FILE("debug_util.c")

// Test dependencies

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

#define DATA_SIZE   (256)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

static uint8_t tx_data[DATA_SIZE];
static uint8_t rx_data[DATA_SIZE];

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void setUp(void)
{
    memset(rx_data, 0, DATA_SIZE);
}

void test_itf_spi_init(void)
{
    TEST_ASSERT_FALSE(itf_spi_init(H_ITF_SPI_COUNT));
    TEST_ASSERT_TRUE(itf_spi_init(H_ITF_SPI_0));
    itf_spi_flush(H_ITF_SPI_0);
    itf_io_set_value(H_ITF_IO_OUT_1, ITF_IO_HIGH);
}

void test_itf_spi_write(void)
{
    for (int i = 0; i < DATA_SIZE; i++)
    {
        tx_data[i] = i;
    }

    // Chip select
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);
    itf_spi_select(H_ITF_SPI_CHIP_MODE_0);
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);

    // Transaction
    TEST_ASSERT_TRUE(itf_spi_transaction(H_ITF_SPI_0, tx_data, NULL,
                     DATA_SIZE));

    // Chip deselect
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);
    itf_spi_deselect(H_ITF_SPI_CHIP_MODE_0);
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);
}

void test_itf_spi_read(void)
{
    for (int i = 0; i < DATA_SIZE; i++)
    {
        tx_data[i] = 0;
        rx_data[i] = i;
    }

    // Chip select
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);
    itf_spi_select(H_ITF_SPI_CHIP_MODE_0);
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);

    // Transaction
    TEST_ASSERT_TRUE(itf_spi_transaction(H_ITF_SPI_0, NULL, rx_data,
                                         DATA_SIZE));

    // Chip deselect
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);
    itf_spi_deselect(H_ITF_SPI_CHIP_MODE_0);
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(tx_data, rx_data, DATA_SIZE);
}

void test_itf_spi_error(void)
{
    // Chip select
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);
    itf_spi_select(H_ITF_SPI_CHIP_MODE_0);
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);

    // Transaction
    TEST_ASSERT_FALSE(itf_spi_transaction(H_ITF_SPI_0, NULL, NULL, DATA_SIZE));
    TEST_ASSERT_FALSE(itf_spi_transaction(H_ITF_SPI_0, tx_data, rx_data, 0));

    // Chip deselect
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);
    itf_spi_deselect(H_ITF_SPI_CHIP_MODE_0);
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);
}

void test_itf_spi_write_and_read(void)
{
    for (int i = 0; i < DATA_SIZE; i++)
    {
        tx_data[i] = DATA_SIZE - 1 - i;
    }

    // Chip select
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);
    itf_spi_select(H_ITF_SPI_CHIP_MODE_0);
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);

    // Transaction
    TEST_ASSERT_TRUE(itf_spi_transaction(H_ITF_SPI_0, tx_data, rx_data,
                                         DATA_SIZE));

    // Chip deselect
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);
    itf_spi_deselect(H_ITF_SPI_CHIP_MODE_0);
    TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(tx_data, rx_data, DATA_SIZE);
}

void test_itf_spi_mode_change(void)
{
    for (h_itf_spi_chip_t chip = H_ITF_SPI_CHIP_MODE_0;
         chip < H_ITF_SPI_CHIP_COUNT; chip++)
    {
        for (int i = 0; i < DATA_SIZE; i++)
        {
            tx_data[i] = DATA_SIZE - 1 - i;
        }

        // Chip select
        TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);
        itf_spi_select(chip);
        TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);

        // Transaction
        TEST_ASSERT_TRUE(itf_spi_transaction(H_ITF_SPI_0, tx_data, rx_data,
                                             DATA_SIZE));

        // Chip deselect
        TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_LOW);
        itf_spi_deselect(chip);
        TEST_ASSERT_TRUE(itf_io_get_value(H_ITF_IO_IN_1) == ITF_IO_HIGH);

        TEST_ASSERT_EQUAL_UINT8_ARRAY(tx_data, rx_data, DATA_SIZE);
    }
}

void test_itf_spi_deinit(void)
{
    TEST_ASSERT_FALSE(itf_spi_deinit(H_ITF_SPI_COUNT));
    TEST_ASSERT_TRUE(itf_spi_deinit(H_ITF_SPI_0));
    TEST_ASSERT_FALSE(itf_spi_deinit(H_ITF_SPI_0));
}

/******************************** End of file *********************************/
