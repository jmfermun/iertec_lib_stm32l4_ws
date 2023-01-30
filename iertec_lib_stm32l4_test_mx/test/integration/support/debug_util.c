/*******************************************************************************
 * @file debug_util.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 30 Jan 2023
 * @brief Debug utilities.
 ******************************************************************************/

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
SemaphoreHandle_t h_debug_mutex;

/** Debug buffer. */
static char debug_buffer[DEBUG_BUFFER_SIZE];

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool debug_init(void)
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

    return true;
}

bool debug_deinit(void)
{
    return false;
}

bool debug_is_attached(void)
{
    return false;
}

void debug_printf(const char *format, ...)
{
    if (xSemaphoreTake(h_debug_mutex, portMAX_DELAY))
    {
        va_list args;
        size_t  len;

        va_start(args, format);
        len = vsnprintf(debug_buffer, DEBUG_BUFFER_SIZE, format, args);
        va_end(args);

        itf_debug_write(debug_buffer, len);

        xSemaphoreGive(h_debug_mutex);
    }
}

void debug_write(const char * data, size_t len)
{

}

void debug_put_char(char ch)
{

}

int debug_get_char(void)
{
    return -1;
}

bool debug_is_ready(Void)
{
    return false;
}

/******************************** End of file *********************************/
