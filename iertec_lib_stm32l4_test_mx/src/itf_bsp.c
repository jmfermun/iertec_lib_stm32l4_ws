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
#include "itf_debug.h"
#include "itf_clk.h"
#include "itf_io.h"
#include "itf_spi.h"
#include "itf_uart.h"
#include "main.h"
#include "gpio.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"

/****************************************************************************//*
 * itf_clk board configuration
 ******************************************************************************/

/** CLK low level initializer function.*/
const itf_bsp_init_ll_t itf_clk_init_ll = SystemClock_Config;

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
 * itf_io board configuration
 ******************************************************************************/

/** Board configuration of the available I/O lines. */
const itf_io_config_t itf_io_config[H_ITF_IO_COUNT] =
{
    {
        .port = B1_GPIO_Port,
        .pin  = B1_Pin,
    },
    {
        .port = LD4_GPIO_Port,
        .pin  = LD4_Pin,
    },
    {
        .port = TDI_GPIO_Port,
        .pin  = TDI_Pin,
    },
    {
        .port = USART1_RTS_GPIO_Port,
        .pin  = USART1_RTS_Pin,
    },
};

/** I/O low level initializer function.*/
const itf_bsp_init_ll_t itf_io_init_ll = MX_GPIO_Init;

/****************************************************************************//*
 * itf_spi board configuration
 ******************************************************************************/

/** Board configuration of the available SPI interfaces. */
const itf_spi_config_t itf_spi_config[H_ITF_SPI_COUNT] =
{
    {
        .handle  = &hspi1,
        .init_ll = MX_SPI1_Init,
    },
};

/****************************************************************************//*
 * itf_uart board configuration
 ******************************************************************************/

/** Hardware configuration of the available UART interfaces. */
const itf_uart_config_t itf_uart_config[H_ITF_UART_COUNT] =
{
    {
        .handle       = &huart1,
        .pin_rts      = H_ITF_IO_UART_0_RTS,
        .timeout_msec = 1000,
        .init_ll      = MX_USART1_UART_Init,
    },
    {
        .handle       = &huart2,
        .pin_rts      = H_ITF_IO_NONE,
        .timeout_msec = 5000,
        .init_ll      = MX_USART2_UART_Init,
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
    ret = itf_io_init() && ret;

    // Initialize here the DMA because it is a shared resource
    MX_DMA_Init();

    return ret;
}

bool
itf_bsp_init (void)
{
#ifndef TEST
    bool ret = true;

    itf_debug_init();
    ret = itf_spi_init(H_ITF_SPI_0) && ret;
    ret = itf_uart_init(H_ITF_UART_0) && ret;

    return ret;
#else // !TEST
    return false;
#endif // TEST
}

bool itf_bsp_get_error(void)
{
    return Error_Get();
}

/** @} */

/******************************** End of file *********************************/
