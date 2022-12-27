/*******************************************************************************
 * @file itf_uart.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 22 Dec 2022
 * @brief UART interface driver.
 * @ingroup itf_uart
 ******************************************************************************/

/**
 * @defgroup itf_uart itf_uart
 * @brief UART interface driver.
 * @{
 */

#ifndef ITF_UART_H
#define ITF_UART_H

#include "itf_bsp.h"
#include "stm32l4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/** @brief UART interface hardware configuration type. */
typedef struct
{
    UART_HandleTypeDef * handle;
    uint32_t             timeout_msec;
    itf_bsp_init_ll_t    init_ll;
} itf_uart_config_t;

/**
 * @brief Initialize an UART interface.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 *
 * @return true If the interface is initialized correctly.
 * @return false If an error occurs.
 */
bool itf_uart_init(h_itf_uart_t h_itf_uart);

/**
 * @brief Deinitialize an UART interface.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 */
void itf_uart_deinit(h_itf_uart_t h_itf_uart);

/**
 * @brief Send a NULL terminated string through the UART.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 * @param data Data formed by a NULL terminated string.
 *
 * @retval true String sent correctly.
 * @retval false An error occurred sending the string.
 */
bool itf_uart_write(h_itf_uart_t h_itf_uart, const char * data);

/**
 * @brief Send binary data through the UART.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 * @param data Data in binary format.
 * @param len Number of bytes to send.
 *
 * @retval true Data sent correctly.
 * @retval false An error occurred sending the data.
 */
bool itf_uart_write_bin(h_itf_uart_t h_itf_uart, const char * data, size_t len);

/**
 * @brief Enable the UART reception.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 */
void itf_uart_read_enable(h_itf_uart_t h_itf_uart);

/**
 * @brief Disable the UART reception.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 */
void itf_uart_read_disable(h_itf_uart_t h_itf_uart);

/**
 * @brief Read a null terminated string from the UART.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 * @param data String formed by a NULL terminated string.
 * @param max_len Size of data buffer.
 *
 * @return The number of bytes read, including the null terminator.
 * @retval 0 If an error occurs.
 */
size_t itf_uart_read(h_itf_uart_t h_itf_uart, char * data, size_t max_len);

/**
 * @brief Read binary data from the UART. It reads the number of bytes indicated
 * by max_rsp_len. If a time greater than @ref ITF_UART_READ_BIN_TIMEOUT occurs,
 * the read is aborted.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 * @param data Data in binary format.
 * @param len Maximum number of bytes to read.
 *
 * @return The number of bytes read.
 */
size_t itf_uart_read_bin(h_itf_uart_t h_itf_uart, char * data, size_t len);

/**
 * @brief Get the number of bytes available in the read buffer.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 *
 * @return Number of bytes available.
 */
size_t itf_uart_read_count(h_itf_uart_t h_itf_uart);

/**
 * @brief UART interrupt service routine handler for transmission and reception.
 *
 * @param h_itf_uart Handler of the UART interface to use.
 */
void itf_uart_isr(h_itf_uart_t h_itf_uart);

#endif // ITF_UART_H

/** @} */

/******************************** End of file *********************************/
