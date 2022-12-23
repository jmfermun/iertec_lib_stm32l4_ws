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
#include "itf_io.h"
#include "itf_uart.h"
#include "main.h"
#include "usart.h"

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
};

/****************************************************************************//*
 * itf_uart board configuration
 ******************************************************************************/

/** Hardware configuration of the available UART interfaces. */
const itf_uart_config_t itf_uart_config[H_ITF_UART_COUNT] =
{
    {
        .handle       = &huart2,
        .timeout_msec = 5000,
    },
};

/****************************************************************************//*
 * Public functions
 ******************************************************************************/

bool
itf_bsp_init (void)
{
#ifndef TEST
    bool ret = true;

    ret = itf_io_init() && ret;
    ret = itf_uart_init(H_ITF_UART_DEBUG) && ret;

    return ret;
#else // !TEST
    return false;
#endif // TEST
}

/** @} */

/******************************** End of file *********************************/
