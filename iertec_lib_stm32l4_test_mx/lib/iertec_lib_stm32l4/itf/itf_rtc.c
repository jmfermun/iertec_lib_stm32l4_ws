/*******************************************************************************
 * @file itf_rtc.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 18 Jan 2023
 * @brief RTC interface driver.
 * @ingroup itf_rtc
 ******************************************************************************/

/**
 * @addtogroup itf_rtc
 * @{
 */

#include "itf_rtc.h"
#include "itf_pwr.h"

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const itf_rtc_config_t itf_rtc_config;

/** Callback function to be called each second. */
static itf_rtc_cb_t itf_rtc_cb = NULL;

/** RTC seconds count. */
static volatile uint32_t itf_rtc_seconds = 0;

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_rtc_init (void)
{
    // Low level initialization
    if (NULL != itf_rtc_config.init_ll)
    {
        itf_rtc_config.init_ll();
    }

    if (itf_bsp_get_error())
    {
        return false;
    }

    // LPTIM2 instance does not support Stop 2 mode
    uint8_t h_itf_pwr = itf_pwr_register(ITF_PWR_LEVEL_1);

    if (H_ITF_PWR_NONE == h_itf_pwr)
    {
        return false;
    }

    itf_pwr_set_active(h_itf_pwr);

    if (HAL_LPTIM_TimeOut_Start_IT(itf_rtc_config.handle, ITF_RTC_CLK_FREQ,
                                   ITF_RTC_CLK_FREQ - 1) != HAL_OK)
    {
        return false;
    }

    return true;
}

void
itf_rtc_set_time (uint32_t seconds, uint8_t cseconds)
{
    // Modification of centiseconds is not supported
    (void)cseconds;

    // Unsigned integer read is atomic, so no critical section is needed
    itf_rtc_seconds = seconds;
}

void
itf_rtc_get_time (uint32_t * seconds, uint8_t * cseconds)
{
    uint32_t sec;
    uint32_t counter;

    // To read reliably the content of the LPTIM_CNT register, two successive
    // read accesses must be performed and compared
    do
    {
        counter = itf_rtc_config.handle->Instance->CNT;

        // Performing the seconds read between the counter reads we can ensure
        // that the seconds value is reliable and no critical section is needed
        sec = itf_rtc_seconds;
    } while (counter != itf_rtc_config.handle->Instance->CNT);

    *seconds = sec;

    if (counter >= ITF_RTC_CLK_FREQ)
    {
        *cseconds = 0;
    }
    else
    {
        *cseconds = counter * 100u / ITF_RTC_CLK_FREQ;
    }
}

uint32_t
itf_rtc_get_ticks (void)
{
    uint32_t sec;
    uint32_t counter;

    // To read reliably the content of the LPTIM_CNT register, two successive
    // read accesses must be performed and compared
    do
    {
        counter = itf_rtc_config.handle->Instance->CNT;

        // Performing the seconds read between the counter reads we can ensure
        // that the seconds value is reliable and no critical section is needed
        sec = itf_rtc_seconds;
    } while (counter != itf_rtc_config.handle->Instance->CNT);

    if (counter >= ITF_RTC_CLK_FREQ)
    {
        counter = 0;
    }

    return sec * ITF_RTC_CLK_FREQ + counter;
}

void
itf_rtc_set_callback (itf_rtc_cb_t cb)
{
    itf_rtc_cb = cb;
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

void
HAL_LPTIM_CompareMatchCallback (LPTIM_HandleTypeDef * h_lptim)
{
    if (h_lptim == itf_rtc_config.handle)
    {
        itf_rtc_seconds++;

        if (itf_rtc_cb != NULL)
        {
            itf_rtc_cb();
        }
    }
}

/** @} */

/******************************** End of file *********************************/
