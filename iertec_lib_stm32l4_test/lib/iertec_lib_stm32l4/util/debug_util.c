/*******************************************************************************
 * @file debug_util.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 24 Jan 2023
 * @brief Debug utilities.
 * @ingroup debug_util
 ******************************************************************************/

/**
 * @addtogroup debug_util
 * @{
 */

#include "debug_util.h"
#include "itf_debug.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include <stdarg.h>
#include <stdio.h>

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Debug buffer size. */
#define DEBUG_BUFFER_SIZE (1024)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** Mutex to access to the debug interface. */
static SemaphoreHandle_t h_debug_mutex;

/** Debug buffer. */
static char debug_buffer[DEBUG_BUFFER_SIZE];

/** Flag that indicates if the interface is ready for printf. */
static bool b_debug_is_ready = false;

/** Flag that indicates if the interface is attached to an external device. */
static bool b_debug_is_attached = false;

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
debug_init (void)
{
#ifndef NDEBUG
    b_debug_is_attached = true;
#else
    b_debug_is_attached = itf_debug_is_connected();
#endif

    if (b_debug_is_attached)
    {
        // Construct debug mutex
        h_debug_mutex = xSemaphoreCreateMutex();

        if (NULL == h_debug_mutex)
        {
            return false;
        }

        if (!itf_debug_init())
        {
            return false;
        }

        b_debug_is_ready = true;
    }

    return true;
}

bool
debug_deinit (void)
{
#ifdef NDEBUG
    if (b_debug_is_ready)
    {
        b_debug_is_ready = false;

        if (itf_debug_deinit())
        {
            return true;
        }
    }
#endif // NDEBUG

    return false;
}

bool
debug_is_attached (void)
{
    return b_debug_is_attached;
}

void
debug_printf_impl (const char * format, ...)
{
    if (b_debug_is_ready)
    {
        if (xSemaphoreTake(h_debug_mutex, portMAX_DELAY) == pdPASS)
        {
            va_list args;
            int     len;

            va_start(args, format);
            len = vsnprintf(debug_buffer, DEBUG_BUFFER_SIZE, format, args);
            va_end(args);

            if (len > 0)
            {
                itf_debug_write(debug_buffer, (size_t)len);
            }

            (void)xSemaphoreGive(h_debug_mutex);
        }
    }
}

void
debug_write (const char * data, size_t len)
{
    if (b_debug_is_ready)
    {
        if (xSemaphoreTake(h_debug_mutex, portMAX_DELAY) == pdPASS)
        {
            itf_debug_write(data, len);

            (void)xSemaphoreGive(h_debug_mutex);
        }
    }
}

void
debug_put_char (char ch)
{
    if (b_debug_is_ready)
    {
        itf_debug_put_char(ch);
    }
}

int
debug_get_char (void)
{
    if (b_debug_is_ready)
    {
        return itf_debug_get_char();
    }

    return -1;
}

bool
debug_is_ready (void)
{
    return b_debug_is_ready;
}

/** @} */

/******************************** End of file *********************************/
