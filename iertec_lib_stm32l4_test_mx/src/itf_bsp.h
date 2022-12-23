/*******************************************************************************
 * @file itf_bsp.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 22 Dec 2022
 * @brief Interfaces board support package.
 * @ingroup itf_bsp
 ******************************************************************************/

/**
 * @defgroup itf_bsp itf_bsp
 * @brief Interfaces board support package.
 * @{
 */

#ifndef ITF_BSP_H
#define ITF_BSP_H

#include <stdbool.h>

/** @brief I/O interface handlers. */
typedef enum
{
    H_ITF_IO_BUTTON_1 = 0,
    H_ITF_IO_LED_GREEN,
    H_ITF_IO_COUNT,
    H_ITF_IO_INT_COUNT = 1,
} h_itf_io_t;

/** @brief UART interface handlers. */
typedef enum
{
    H_ITF_UART_DEBUG = 0,
    H_ITF_UART_COUNT,
} h_itf_uart_t;

/**
 * @brief Initialize the specific board interfaces.
 *
 * @return true If the interfaces are initialized correctly.
 * @return false If an error occurs.
 */
bool itf_bsp_init(void);

#endif // ITF_BSP_H

/** @} */

/******************************** End of file *********************************/
