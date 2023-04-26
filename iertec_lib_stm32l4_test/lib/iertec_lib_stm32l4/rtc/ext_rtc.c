/*******************************************************************************
 * @file ext_rtc.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 18 Apr 2023
 * @brief External RTC driver.
 * @ingroup ext_rtc
 ******************************************************************************/

/**
 * @addtogroup ext_rtc
 * @{
 */

#include "ext_rtc.h"

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const ext_rtc_config_t ext_rtc_config;

/** Callback function to be called each second. */
static ext_rtc_cb_t ext_rtc_cb = NULL;

/** RTC seconds count. */
static volatile uint32_t ext_rtc_seconds = 0u;

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Tick interrupt service routine.
 */
static void ext_rtc_tick_isr(void);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool ext_rtc_init(void)
{
    if (ext_rtc_config.pin_tick < H_ITF_IO_INT_COUNT)
    {
        itf_io_set_int_cb(ext_rtc_config.pin_tick, ext_rtc_tick_isr);

        return true;
    }

    return false;
}

void
ext_rtc_set_time (uint32_t seconds, uint8_t cseconds)
{
    // Centiseconds not supported
    (void)cseconds;

    // Unsigned integer read is atomic, so no critical section is needed
    ext_rtc_seconds = seconds;
}

void
ext_rtc_get_time (uint32_t * seconds, uint8_t * cseconds)
{
    // Access to uint32_t is atomic
    *seconds = ext_rtc_seconds;

    // Centiseconds not supported
    *cseconds = 0;
}

uint32_t
ext_rtc_get_ticks (void)
{
    return ext_rtc_seconds;
}

void
ext_rtc_set_callback (ext_rtc_cb_t cb)
{
    ext_rtc_cb = cb;
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void
ext_rtc_tick_isr (void)
{
    ext_rtc_seconds++;

    if (ext_rtc_cb != NULL)
    {
        ext_rtc_cb();
    }
}

/** @} */

/******************************** End of file *********************************/
