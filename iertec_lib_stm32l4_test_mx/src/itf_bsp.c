/*******************************************************************************
 * @file itf_bsp.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 22 Dec 2022
 * @brief Interfaces board support package.
 * @ingroup itf_bsp
 ******************************************************************************/

/**
 * @addtogroup itf_bsp
 * @{
 */

#include "itf_bsp.h"
#include "itf_clk.h"
#include "itf_wdgt.h"
#include "itf_io.h"
#include "itf_pwr.h"
#include "itf_debug.h"
#include "itf_rtc.h"
#include "itf_spi.h"
#include "itf_i2c.h"
#include "itf_uart.h"
#include "main.h"
#include "iwdg.h"
#include "gpio.h"
#include "lptim.h"
#include "dma.h"
#include "spi.h"
#include "i2c.h"
#include "usart.h"

/****************************************************************************//*
 * itf_clk board configuration
 ******************************************************************************/

/** CLK low level initializer function.*/
const itf_bsp_init_ll_t itf_clk_init_ll = SystemClock_Config;

/****************************************************************************//*
 * itf_wdgt board configuration
 ******************************************************************************/

/** Hardware configuration of the available watchdog interface. */
const itf_wdgt_config_t itf_wdgt_config =
{
    .handle       = &hiwdg,
    .timeout_msec = 5000,
};

/****************************************************************************//*
 * itf_io board configuration
 ******************************************************************************/

/** Board configuration of the available I/O lines. */
const itf_io_config_t itf_io_config[H_ITF_IO_COUNT] =
{
    {   // H_ITF_IO_BUTTON_1
        .port = B1_GPIO_Port,
        .pin  = B1_Pin,
    },
    {   // H_ITF_IO_IN_1
        .port = IO_IN_1_GPIO_Port,
        .pin  = IO_IN_1_Pin,
    },
    {   // H_ITF_IO_LED_GREEN
        .port = LD4_GPIO_Port,
        .pin  = LD4_Pin,
    },
    {   // H_ITF_IO_OUT_1
        .port = IO_OUT_1_GPIO_Port,
        .pin  = IO_OUT_1_Pin,
    },
    {   // H_ITF_IO_TDI
        .port = TDI_GPIO_Port,
        .pin  = TDI_Pin,
    },
    {   // H_ITF_IO_UART_0_RTS
        .port = USART1_RTS_GPIO_Port,
        .pin  = USART1_RTS_Pin,
    },
};

/** I/O low level initializer function.*/
const itf_bsp_init_ll_t itf_io_init_ll = MX_GPIO_Init;

/****************************************************************************//*
 * itf_debug board configuration
 ******************************************************************************/

/** Hardware configuration of the available UART interfaces. */
const itf_debug_config_t itf_debug_config =
{
    .handle  = H_ITF_UART_DEBUG,
    .pin_tdi = H_ITF_IO_TDI,
};

/****************************************************************************//*
 * itf_rtc board configuration
 ******************************************************************************/

/** Hardware configuration of the available RTC interface. */
const itf_rtc_config_t itf_rtc_config =
{
    .handle  = &hlptim2,
    .init_ll = MX_LPTIM2_Init,
};

/****************************************************************************//*
 * itf_spi board configuration
 ******************************************************************************/

/** Board configuration of the available SPI interfaces. */
const itf_spi_config_t itf_spi_config[H_ITF_SPI_COUNT] =
{
    {   // H_ITF_SPI_0
        .handle  = &hspi1,
        .init_ll = MX_SPI1_Init,
    },
};

/** Board configuration of the available SPI chip interfaces. */
const itf_spi_chip_config_t itf_spi_chip_config[H_ITF_SPI_CHIP_COUNT] =
{
    {   // H_ITF_SPI_CHIP_MODE_0
        .h_itf_spi = H_ITF_SPI_0,
        .pin_cs    = H_ITF_IO_OUT_1,
        .mode      = ITF_SPI_MODE_POL0_PHA0,
    },
    {   // H_ITF_SPI_CHIP_MODE_1
        .h_itf_spi = H_ITF_SPI_0,
        .pin_cs    = H_ITF_IO_OUT_1,
        .mode      = ITF_SPI_MODE_POL0_PHA1,
    },
    {   // H_ITF_SPI_CHIP_MODE_2
        .h_itf_spi = H_ITF_SPI_0,
        .pin_cs    = H_ITF_IO_OUT_1,
        .mode      = ITF_SPI_MODE_POL1_PHA0,
    },
    {   // H_ITF_SPI_CHIP_MODE_3
        .h_itf_spi = H_ITF_SPI_0,
        .pin_cs    = H_ITF_IO_OUT_1,
        .mode      = ITF_SPI_MODE_POL1_PHA1,
    },
};

/****************************************************************************//*
 * itf_i2c board configuration
 ******************************************************************************/

/** Board configuration of the available I2C interfaces. */
const itf_i2c_config_t itf_i2c_config[H_ITF_I2C_COUNT] =
{
    {   // H_ITF_I2C_0
        .handle  = &hi2c1,
        .init_ll = MX_I2C1_Init,
    },
};

/****************************************************************************//*
 * itf_uart board configuration
 ******************************************************************************/

/** Empty configuration for special line cases. */
static const itf_uart_line_no_crlf_t line_no_crlf_none[] =
{
    {
        .line = NULL,
        .len  = 0,
    },
};

/** Hardware configuration of the available UART interfaces. */
const itf_uart_config_t itf_uart_config[H_ITF_UART_COUNT] =
{
    {   // H_ITF_UART_DEBUG
        .handle       = &huart2,
        .pin_rts      = H_ITF_IO_NONE,
        .timeout_msec = 2000,
        .init_ll      = MX_USART2_UART_Init,
        .line_no_crlf = line_no_crlf_none,
    },
    {   // H_ITF_UART_0
        .handle       = &huart1,
        .pin_rts      = H_ITF_IO_UART_0_RTS,
        .timeout_msec = 1000,
        .init_ll      = MX_USART1_UART_Init,
        .line_no_crlf = line_no_crlf_none,
    },
};

/****************************************************************************//*
 * Public functions
 ******************************************************************************/

bool
itf_bsp_ll_init (void)
{
    bool ret = true;

    // Reset of all peripherals, Initializes the Flash interface and the Systick
    HAL_Init();

    // Initialize the low level interfaces
    ret = itf_clk_init() && ret;
#ifndef TEST
    ret = itf_wdgt_init() && ret;
#endif // TEST
    ret = itf_io_init() && ret;
    ret = itf_pwr_init() && ret;

    // Initialize here the DMA because it is a shared resource
    MX_DMA_Init();

    return ret;
}

bool
itf_bsp_init (void)
{
#ifndef TEST
    bool ret = true;

    ret = itf_rtc_init() && ret;
    ret = itf_spi_init(H_ITF_SPI_0) && ret;
    ret = itf_i2c_init(H_ITF_I2C_0) && ret;
    ret = itf_uart_init(H_ITF_UART_0) && ret;

    return ret;
#else // !TEST
    return false;
#endif // TEST
}

bool
itf_bsp_get_error (void)
{
    return Error_Get();
}

/** @} */

/******************************** End of file *********************************/
