/*******************************************************************************
 * @file itf_debug.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 23 Dec 2022
 * @brief Debug interface driver.
 * @ingroup itf_debug
 ******************************************************************************/

/**
 * @addtogroup itf_debug
 * @{
 */

#include "itf_debug.h"

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const itf_debug_config_t itf_debug_config;

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_debug_is_connected (void)
{
    const size_t NUM_READS = 10;
    bool         ret       = true;

    for (size_t i = 0; i < NUM_READS; i++)
    {
        if (itf_io_get_value(itf_debug_config.pin_tdi) == ITF_IO_LOW)
        {
            ret = false;
            break;
        }
    }

    return ret;
}

bool
itf_debug_init (void)
{
    bool ret;

    ret = itf_uart_init(itf_debug_config.handle);
    itf_uart_read_enable(itf_debug_config.handle);

    return ret;
}

bool
itf_debug_deinit (void)
{
    return itf_uart_deinit(itf_debug_config.handle);
}

void
itf_debug_put_char (char ch)
{
    (void)itf_uart_write_bin(itf_debug_config.handle, &ch, 1);
}

int
itf_debug_get_char (void)
{
    char   ch;
    // Return -2 if no character is available
    if (itf_uart_read_count(itf_debug_config.handle) == 0)
    {
        return -2;
    }

    if (itf_uart_read_bin(itf_debug_config.handle, &ch, 1) == 1)
    {
        return ch;
    }

    return -1;
}

void
itf_debug_write (const char * data, size_t len)
{
    (void)itf_uart_write_bin(itf_debug_config.handle, data, len);
}

/** @} */

/******************************** End of file *********************************/
