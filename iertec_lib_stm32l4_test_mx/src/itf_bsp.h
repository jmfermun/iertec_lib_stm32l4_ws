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

/** @brief Function prototype for the low level initializers used by the
 * different interfaces. */
typedef void (* itf_bsp_init_ll_t)(void);

/** @brief I/O interface handlers. */
typedef enum
{
    H_ITF_IO_BUTTON_1 = 0,
    H_ITF_IO_IN_1,
    H_ITF_IO_LED_GREEN,
    H_ITF_IO_OUT_1,
    H_ITF_IO_TDI,
    H_ITF_IO_UART_0_RTS,
    H_ITF_IO_COUNT,
    H_ITF_IO_INT_COUNT = 2,
    H_ITF_IO_NONE = 0xFF,
} h_itf_io_t;

/** @brief SPI interface handlers. */
typedef enum
{
    H_ITF_SPI_0 = 0,
    H_ITF_SPI_COUNT,
} h_itf_spi_t;

/** @brief I2C interface handlers. */
typedef enum
{
    H_ITF_I2C_0 = 0,
    H_ITF_I2C_COUNT,
} h_itf_i2c_t;

/** @brief UART interface handlers. */
typedef enum
{
    H_ITF_UART_0 = 0,
    H_ITF_UART_DEBUG,
    H_ITF_UART_COUNT,
} h_itf_uart_t;

/**
 * @brief Initialize the low level specific board interfaces.
 *
 * @return true If the interfaces are initialized correctly.
 * @return false If an error occurs.
 */
bool itf_bsp_ll_init(void);

/**
 * @brief Initialize the rest of specific board interfaces.
 *
 * @return true If the interfaces are initialized correctly.
 * @return false If an error occurs.
 */
bool itf_bsp_init(void);

/**
 * @brief Utility function to be used by the each peripheral initialization
 * function to check if the low level initialization has executed correctly.
 *
 * @retval true If the execution was correct.
 * @retval false If an error occurs.
 */
bool itf_bsp_get_error(void);

#endif // ITF_BSP_H

/** @} */

/******************************** End of file *********************************/
