/*******************************************************************************
 * @file test_z_dummy.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Dec 2022
 * @brief Dummy test to be executed the last one in the test regression.
 ******************************************************************************/

#include "unity.h"

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
TEST_FILE("itf_uart.c")
TEST_FILE("itf_debug.c")
TEST_FILE("debug_util.c")

// Test dependencies

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_z_dummy(void)
{
    int z = 0;

    TEST_ASSERT_EQUAL_INT(0, z);
}

/******************************** End of file *********************************/
