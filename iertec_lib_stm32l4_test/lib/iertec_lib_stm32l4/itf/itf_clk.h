/*******************************************************************************
 * @file itf_clk.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Dec 2022
 * @brief System clocks initialization.
 * @ingroup itf_clk
 ******************************************************************************/

/**
 * @defgroup itf_clk itf_clk
 * @brief System clocks initialization.
 * @{
 */

#ifndef ITF_CLK_H
#define ITF_CLK_H

#include "itf_bsp.h"
#include "stm32l4xx_hal.h"

#include <stdbool.h>

/**
 * @brief System clocks initialization.
 *
 * @return true If the clocks are initialized correctly.
 * @return false If an error occurs.
 */
bool itf_clk_init(void);

/**
 * @brief System clocks deinitialization.
 *
 * @return true If the clocks are deinitialized correctly.
 * @return false If an error occurs.
 */
bool itf_clk_deinit(void);

#endif // ITF_CLK_H

/** @} */

/******************************** End of file *********************************/
