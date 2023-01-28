/*******************************************************************************
 * @file itf_debug_none.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 19 Oct 2022
 * @brief Dummy debug interface driver.
 * @ingroup itf_debug
 ******************************************************************************/

/**
 * @addtogroup itf_debug
 * @{
 */

#include "itf_debug.h"

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_debug_is_connected (void)
{
    return false;
}

bool
itf_debug_init (void)
{
    // Do none
    return true;
}

bool
itf_debug_deinit (void)
{
    // Do none
    return true;
}

void
itf_debug_put_char (char ch)
{
    (void)ch;

    // Do none
}

int
itf_debug_get_char (void)
{
    return -1;
}

void
itf_debug_write (const char * data, size_t len)
{
    (void)data;
    (void)len;

    // Do none
}

/** @} */

/******************************** End of file *********************************/
