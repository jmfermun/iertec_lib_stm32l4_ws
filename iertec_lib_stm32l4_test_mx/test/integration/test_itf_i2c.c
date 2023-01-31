/*******************************************************************************
 * @file test_itf_i2c.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 18 Jan 2023
 * @brief Test for the I2C interface driver.
 *
 * The test connects the I2C interface to a PCF85063A device and interchanges
 * information with it.
 *
 * Needed connections:
 * - PB8 ->  SCL
 * - PB9 <-> SDA
 ******************************************************************************/

#include "itf_i2c.h"

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

#define SLAVE_ADDRESS (0x51)
#define DATA_BYTES    30, 30, 12, 15, 6, 25
#define DATA_SIZE     (6)

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_itf_i2c_init(void)
{
    TEST_ASSERT_FALSE(itf_i2c_init(H_ITF_I2C_COUNT));
    TEST_ASSERT_TRUE(itf_i2c_init(H_ITF_I2C_0));
}

void test_itf_i2c_write(void)
{
    uint8_t tx_data[] = {0x04, DATA_BYTES};

    TEST_ASSERT_TRUE(itf_i2c_transaction(H_ITF_I2C_0, SLAVE_ADDRESS, tx_data,
                                         sizeof(tx_data), NULL, 0));
}

void test_itf_i2c_error(void)
{
    uint8_t tx_data[] = {0x04, DATA_BYTES};
    uint8_t rx_data[DATA_SIZE] = {0};

    // Use an incorrect slave address
    TEST_ASSERT_FALSE(itf_i2c_transaction(H_ITF_I2C_0, 0, tx_data,
                                          sizeof(tx_data), NULL, 0));
    TEST_ASSERT_FALSE(itf_i2c_transaction(H_ITF_I2C_0, 0, NULL, 0, rx_data,
                                          sizeof(rx_data)));
}

void test_itf_i2c_read(void)
{
    uint8_t tx_data[] = {0x04};
    uint8_t rx_data[DATA_SIZE] = {0};
    uint8_t exp_data[DATA_SIZE] = {DATA_BYTES};

    TEST_ASSERT_TRUE(itf_i2c_transaction(H_ITF_I2C_0, SLAVE_ADDRESS, tx_data,
                                         sizeof(tx_data), rx_data,
                                         sizeof(rx_data)));

    // Seconds could have incremented by one
    if (exp_data[0] + 1 == rx_data[0])
    {
        exp_data[0] += 1;
    }

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_data, rx_data, DATA_SIZE);
}

void test_itf_i2c_deinit(void)
{
    TEST_ASSERT_FALSE(itf_i2c_deinit(H_ITF_I2C_COUNT));
    TEST_ASSERT_TRUE(itf_i2c_deinit(H_ITF_I2C_0));
    TEST_ASSERT_FALSE(itf_i2c_deinit(H_ITF_I2C_0));
}

/******************************** End of file *********************************/
