/*******************************************************************************
 * @file itf_clk.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Dec 2022
 * @brief System clocks initialization.
 * @ingroup itf_clk
 ******************************************************************************/

/**
 * @addtogroup itf_clk
 * @{
 */

#include "itf_clk.h"

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const itf_bsp_init_ll_t itf_clk_init_ll;

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_clk_init (void)
{
    // Low level initialization
    if (NULL != itf_clk_init_ll)
    {
        itf_clk_init_ll();
    }

    if (itf_bsp_get_error())
    {
        return false;
    }

    return true;
}

/** @} */

/******************************** End of file *********************************/
