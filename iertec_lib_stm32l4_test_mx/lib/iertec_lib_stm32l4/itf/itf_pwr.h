/*******************************************************************************
 * @file itf_pwr.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 23 Jan 2022
 * @brief Power control system interface.
 * @ingroup itf_pwr
 ******************************************************************************/

/**
 * @defgroup itf_pwr itf_pwr
 * @brief Power control system interface.
 * @{
 */

#ifndef ITF_PWR_H
#define ITF_PWR_H

#include "itf_bsp.h"
#include "stm32l4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

/** Invalid handler value. */
#define H_ITF_PWR_NONE (0xFF)

/** @brief Available power levels.
 * - Level 0: Sleep.
 * - Level 1: Stop 1.
 * - Level 2: Stop 2. */
typedef enum
{
    ITF_PWR_LEVEL_0,
    ITF_PWR_LEVEL_1,
    ITF_PWR_LEVEL_2,
    ITF_PWR_LEVEL_COUNT,
} itf_pwr_level_t;

/**
 * @brief Power control system initialization.
 *
 * @return true If the power control resources are initialized correctly.
 * @return false If an error occurs.
 */
bool itf_pwr_init(void);

/**
 * @brief Register a new peripheral in the power control system and return a
 * handler to be used in activate / deactivate operations.
 *
 * @param[in] level Lowest power level that can be used by the peripheral.
 *
 * @retval H_ITF_PWR_NONE If an error occurs.
 * @return A handle to be used by the peripheral when it needs to modify the
 * power control system state.
 */
uint8_t itf_pwr_register(itf_pwr_level_t level);

/**
 * @brief Mark as active the peripheral in the power control system.
 *
 * @param[in] h_itf_pwr Handler of the peripheral.
 */
void itf_pwr_set_active(uint8_t h_itf_pwr);

/**
 * @brief Mark as inactive the peripheral in the power control system.
 *
 * @param[in] h_itf_pwr Handler of the peripheral.
 */
void itf_pwr_set_inactive(uint8_t h_itf_pwr);

/**
 * @brief Function to be called before entering the active sleep mode.
 */
void itf_pwr_pre_sleep(void);

/**
 * @brief Function to be called after exiting the active sleep mode.
 */
void itf_pwr_post_sleep(void);

#endif // ITF_PWR_H

/** @} */

/******************************** End of file *********************************/
