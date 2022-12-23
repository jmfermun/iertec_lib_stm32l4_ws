/*******************************************************************************
 * @file itf_debug.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 23 Dec 2022
 * @brief Debug UART interface driver.
 * @ingroup itf_debug
 ******************************************************************************/

/**
 * @defgroup itf_debug itf_debug
 * @brief Debug UART interface driver.
 * @{
 */

#ifndef ITF_DEBUG_H
#define ITF_DEBUG_H

#include "itf_uart.h"
#include "itf_io.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/** @brief Debug interface hardware configuration type. */
typedef struct
{
    h_itf_uart_t handle;
    h_itf_io_t   pin_tdi;
} itf_debug_config_t;

/**
 * @brief Check if the UART Rx pin is 1 (UART connected).
 *
 * @return true if UART is connected, false otherwise.
 */
bool itf_debug_uart_check(void);

/**
 * @brief Initialize the debug UART.
 */
void itf_debug_init(void);

/**
 * @brief Deinitialize the debug UART.
 */
void itf_debug_deinit(void);

/**
 * @brief Send a char through the debug UART.
 *
 * @param ch Character to be sent.
 */
void itf_debug_put_char(char ch);

/**
 * @brief Read a character from the debug UART.
 *
 * @return Character read.
 * @retval -1 An error occurs.
 * @retval -2 No character available.
 */
int itf_debug_get_char(void);

/**
 * @brief Send binary data through the debug UART.
 *
 * @param data Binary data to be sent.
 * @param len Number of bytes to send.
 */
void itf_debug_write(const char * data, size_t len);

#endif // ITF_DEBUG_H

/** @} */

/******************************** End of file *********************************/