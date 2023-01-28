/*******************************************************************************
 * @file sys_util.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 16 Jan 2023
 * @brief System utilities.
 * @ingroup sys_util
 ******************************************************************************/

/**
 * @addtogroup sys_util
 * @{
 */

#include "sys_util.h"
#include "itf_rtc.h"

#include "stm32l4xx_hal.h"
#include "task.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Convert time from system ticks to microseconds. */
#define SYS_TICKS_TO_USEC(X) ((uint64_t)(X) * 1000000u / ITF_RTC_CLK_FREQ)

/** Convert time from microseconds to system ticks. */
#define SYS_USEC_TO_TICKS(X) ((uint64_t)(X) * ITF_RTC_CLK_FREQ / 1000000u)

/****************************************************************************//*
 * Public code
 ******************************************************************************/

void
sys_get_reset_source (uint32_t * code, const char ** str)
{
    uint32_t reset_source = HAL_RCC_GetResetSource();

    switch (reset_source)
    {
        case RCC_CSR_FWRSTF:
            *str = "Firewall reset";
        break;

        case RCC_RESET_FLAG_OBL:
            *str = "Option byte loader reset";
        break;

        case RCC_RESET_FLAG_PIN:
            *str = "PIN reset";
        break;

        case RCC_RESET_FLAG_PWR:
            *str = "BOR or POR/PDR reset";
        break;

        case RCC_RESET_FLAG_SW | RCC_RESET_FLAG_PIN:
            *str = "Software reset";
        break;

        case RCC_RESET_FLAG_IWDG | RCC_RESET_FLAG_PIN:
            *str = "Independent watchdog reset";
        break;

        case RCC_RESET_FLAG_WWDG:
            *str = "Window watchdog reset";
        break;

        case RCC_RESET_FLAG_LPWR:
            *str = "Low power reset";
        break;

        default:
            *str = "Combined reset cause";
        break;
    }

    *code = reset_source >> 24;
}

uint32_t
sys_get_timestamp (void)
{
    uint32_t usec = SYS_TICKS_TO_USEC(itf_rtc_get_ticks());

    return usec;
}

uint32_t
sys_time_diff (uint32_t * ticks, uint32_t * max_diff)
{
    uint32_t diff;

    diff     = (*ticks);
    (*ticks) = itf_rtc_get_ticks();

    if (diff != 0)
    {
        diff = SYS_TICKS_TO_USEC((*ticks) - diff);

        if ((max_diff != NULL) && (diff > (*max_diff)))
        {
            (*max_diff) = diff;
        }
    }

    return diff;
}

void
sys_delay_usec (uint32_t usec)
{
    uint32_t start = itf_rtc_get_ticks();
    uint32_t wait  = SYS_USEC_TO_TICKS(usec);

    // Wait one additional tick to ensure at least the indicated time
    wait += 1;

    while ((itf_rtc_get_ticks() - start) < wait)
    {
        // Lock until the complete delay has elapsed
    }
}

void
sys_sleep_msec (uint32_t msec)
{
    vTaskDelay(SYS_MSEC_TO_TICKS(msec));
}

void
sys_sleep_until_msec (uint32_t * prev_ticks, uint32_t inc_msec)
{
    if (inc_msec == 0)
    {
        *prev_ticks = xTaskGetTickCount();
    }
    else
    {
        vTaskDelayUntil(prev_ticks, inc_msec);
    }
}

void
sys_reset (void)
{
    HAL_NVIC_SystemReset();
}

/** @} */

/******************************** End of file *********************************/
