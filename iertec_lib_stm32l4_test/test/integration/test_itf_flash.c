/*******************************************************************************
 * @file test_itf_flash.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 03 May 2023
 * @brief Integration test for the module itf_flash.
 ******************************************************************************/

#include "itf_flash.h"

#include "stm32l4xx_hal.h"

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
TEST_FILE("itf_debug_none.c")
TEST_FILE("debug_util.c")

// Test dependencies

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

#define MEMORY_OFFSET    (FLASH_BANK_SIZE - (FLASH_PAGE_SIZE * 10))
#define MEMORY_ADDRESS   (FLASH_BASE + MEMORY_OFFSET)
#define MEMORY_SIZE      (FLASH_PAGE_SIZE * 4)
#define MEMORY_ADDRESS_2 (MEMORY_ADDRESS + FLASH_PAGE_SIZE)
#define MEMORY_SIZE_2    (FLASH_PAGE_SIZE * 2)

#define DATA_SIZE (256)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

const char test_file_name[] = __FILE_NAME__;

static const char data_val[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const size_t data_val_len = sizeof(data_val) - 1;
static uint8_t wr_data[DATA_SIZE];
static uint8_t rd_data[DATA_SIZE];

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_itf_flash_erase(void)
{
    TEST_PRINTF("Address: %x, Size: %x", MEMORY_ADDRESS, MEMORY_SIZE);
    TEST_ASSERT_TRUE(itf_flash_erase(MEMORY_ADDRESS, MEMORY_SIZE));
}

void test_itf_flash_write(void)
{
    size_t count = 0;

    for (size_t len = 0; len < MEMORY_SIZE; len += DATA_SIZE)
    {
        for (size_t i = 0; i < DATA_SIZE; i++)
        {
            wr_data[i] = data_val[count++ % data_val_len];
        }

        TEST_ASSERT_TRUE(itf_flash_write(MEMORY_ADDRESS + len, wr_data, DATA_SIZE));
    }
}

void test_itf_flash_read(void)
{
    size_t count = 0;

    for (size_t len = 0; len < MEMORY_SIZE; len += DATA_SIZE)
    {
        for (size_t i = 0; i < DATA_SIZE; i++)
        {
            wr_data[i] = data_val[count++ % data_val_len];
            rd_data[i] = 0;
        }

        TEST_ASSERT_TRUE(itf_flash_read(MEMORY_ADDRESS + len, rd_data, DATA_SIZE));
        TEST_ASSERT_EQUAL_MEMORY(wr_data, rd_data, DATA_SIZE);
    }
}

void test_itf_flash_erase_partly(void)
{
    size_t count = 0;

    TEST_ASSERT_TRUE(itf_flash_erase(MEMORY_ADDRESS_2, MEMORY_SIZE_2));

    for (size_t len = 0; len < MEMORY_SIZE; len += DATA_SIZE)
    {
        for (size_t i = 0; i < DATA_SIZE; i++)
        {
            if ((MEMORY_ADDRESS + len < MEMORY_ADDRESS_2)
                || (MEMORY_ADDRESS + len >= MEMORY_ADDRESS_2 + MEMORY_SIZE_2))
            {
                wr_data[i] = data_val[count % data_val_len];
            }
            else
            {
                wr_data[i] = 0xFF;
            }

            rd_data[i] = 0;
            count++;
        }

        TEST_ASSERT_TRUE(itf_flash_read(MEMORY_ADDRESS + len, rd_data, DATA_SIZE));
        TEST_ASSERT_EQUAL_MEMORY(wr_data, rd_data, DATA_SIZE);
    }
}

/******************************** End of file *********************************/
