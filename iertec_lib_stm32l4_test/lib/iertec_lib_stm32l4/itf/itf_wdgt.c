/*******************************************************************************
 * @file itf_wdgt.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 17 Jan 2023
 * @brief Watchdog interface driver.
 * @ingroup itf_wdgt
 ******************************************************************************/

/**
 * @addtogroup itf_wdgt
 * @{
 */

#include "itf_wdgt.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Watchdog clock frequency (Hz). */
#define ITF_WDGT_LSI_FREQ             (32000u)

/** Compute the maximum timeout that can be configured with the indicated
 * prescaler. */
#define ITF_WDGT_TIMEOUT_MAX(PR)      (((4095u * 1000u) * (PR)) \
                                       / ITF_WDGT_LSI_FREQ)

/** Compute reload value for the indicated prescaler and timeout (ms). */
#define ITF_WDGT_RELOAD_VAL(PR, TIME) ((((TIME) * ITF_WDGT_LSI_FREQ) / 1000u) \
                                       / (PR))

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const itf_wdgt_config_t itf_wdgt_config;

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Compute the prescaler and reload values for the indicated timeout.
 *
 * @param[out] prescaler Computed prescaler value.
 * @param[out] reload Computed reload value.
 * @param[in] timeout_msec Desired watchdog timeout in milliseconds.
 */
static void compute_load_value(uint32_t * prescaler, uint32_t * reload,
                               uint32_t timeout_msec);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_wdgt_init (void)
{
    return itf_wdgt_set_timeout(itf_wdgt_config.timeout_msec);
}

bool
itf_wdgt_set_timeout (uint32_t timeout_msec)
{
    uint32_t prescaler;
    uint32_t reload;

    if (0u == timeout_msec)
    {
        timeout_msec = itf_wdgt_config.timeout_msec;
    }

    compute_load_value(&prescaler, &reload, timeout_msec);

    if (0u == reload)
    {
        return false;
    }

    // Use our initialization function instead of the provided one by CubeMX
    itf_wdgt_config.handle->Instance       = IWDG;
    itf_wdgt_config.handle->Init.Prescaler = prescaler;
    itf_wdgt_config.handle->Init.Window    = IWDG_WINDOW_DISABLE;
    itf_wdgt_config.handle->Init.Reload    = reload;

    if (HAL_IWDG_Init(itf_wdgt_config.handle) != HAL_OK)
    {
        return false;
    }

    return true;
}

void
itf_wdgt_feed (void)
{
    (void)HAL_IWDG_Refresh(itf_wdgt_config.handle);
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void
compute_load_value (uint32_t * prescaler, uint32_t * reload,
                    uint32_t timeout_msec)
{
    if (timeout_msec < ITF_WDGT_TIMEOUT_MAX(4u))
    {
        *prescaler = IWDG_PRESCALER_4;
        *reload    = ITF_WDGT_RELOAD_VAL(4u, timeout_msec);
    }
    else if (timeout_msec < ITF_WDGT_TIMEOUT_MAX(8u))
    {
        *prescaler = IWDG_PRESCALER_8;
        *reload    = ITF_WDGT_RELOAD_VAL(8u, timeout_msec);
    }
    else if (timeout_msec < ITF_WDGT_TIMEOUT_MAX(16u))
    {
        *prescaler = IWDG_PRESCALER_16;
        *reload    = ITF_WDGT_RELOAD_VAL(16u, timeout_msec);
    }
    else if (timeout_msec < ITF_WDGT_TIMEOUT_MAX(32u))
    {
        *prescaler = IWDG_PRESCALER_32;
        *reload    = ITF_WDGT_RELOAD_VAL(32u, timeout_msec);
    }
    else if (timeout_msec < ITF_WDGT_TIMEOUT_MAX(64u))
    {
        *prescaler = IWDG_PRESCALER_64;
        *reload    = ITF_WDGT_RELOAD_VAL(64u, timeout_msec);
    }
    else if (timeout_msec < ITF_WDGT_TIMEOUT_MAX(128u))
    {
        *prescaler = IWDG_PRESCALER_128;
        *reload    = ITF_WDGT_RELOAD_VAL(128u, timeout_msec);
    }
    else if (timeout_msec < ITF_WDGT_TIMEOUT_MAX(256u))
    {
        *prescaler = IWDG_PRESCALER_256;
        *reload    = ITF_WDGT_RELOAD_VAL(256u, timeout_msec);
    }
    else
    {
        *prescaler = 0;
        *reload    = 0;
    }
}

/** @} */

/******************************** End of file *********************************/
