/*******************************************************************************
 * @file wdgt.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 17 Jan 2023
 * @brief Watchdog interface driver.
 * @ingroup itf_wdgt
 ******************************************************************************/

/**
 * @defgroup itf_wdgt itf_wdgt
 * @brief Watchdog interface driver.
 * @{
 */

#ifndef ITF_WDGT_H
#define ITF_WDGT_H

#include "itf_bsp.h"
#include "stm32l4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

/** @brief Watchdog interface hardware configuration type. */
typedef struct
{
    IWDG_HandleTypeDef * handle;
    uint32_t             timeout_msec;
} itf_wdgt_config_t;

/**
 * @brief Initialize the watchdog module and start its operation.
 *
 * @retval true If the configuration is correct.
 * @retval false If an error occurred.
 */
bool itf_wdgt_init(void);

/**
 * @brief Change the watchdog timeout. If timeout_msec is set to 0, the default
 * timeout will be used.
 *
 * @param[in] timeout_msec Watchdog timer timeout in milliseconds.
 *
 * @retval true If the configuration is correct.
 * @retval false If an error occurred.
 */
bool itf_wdgt_set_timeout(uint32_t timeout_msec);

/**
 * @brief Reset the watchdog counter.
 */
void itf_wdgt_feed(void);

#endif // ITF_WDGT_H

/** @} */

/******************************** End of file *********************************/
