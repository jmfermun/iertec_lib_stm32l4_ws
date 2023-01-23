/*******************************************************************************
 * @file itf_spi.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 23 Dec 2022
 * @brief SPI interface driver.
 * @ingroup itf_spi
 ******************************************************************************/

/**
 * @defgroup itf_spi itf_spi
 * @brief SPI interface driver.
 * @{
 */

#ifndef ITF_SPI_H
#define ITF_SPI_H

#include "itf_bsp.h"
#include "stm32l4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/** @brief SPI interface hardware configuration type. */
typedef struct
{
    SPI_HandleTypeDef * handle;
    itf_bsp_init_ll_t   init_ll;
} itf_spi_config_t;

/**
 * @brief Initialization of the SPI interface.
 *
 * @param h_itf_spi Handler of the SPI interface to use.
 *
 * @retval true If the interface is initialized correctly.
 * @retval false If an error occurs.
 */
bool itf_spi_init(h_itf_spi_t h_itf_spi);

/**
 * @brief Close the SPI interface and release its resources.
 *
 * @param h_itf_spi Handler of the SPI interface to use.
 *
 * @retval true If the interface is deinitialized correctly.
 * @retval false If an error occurs.
 */
bool itf_spi_deinit(h_itf_spi_t h_itf_spi);

/**
 * @brief Do a write/read transaction with the SPI interface.
 *
 * @param h_itf_spi Handler of the SPI interface to use.
 * @param tx_data Data to write.
 * @param rx_data Where the read data will be stored.
 * @param count Number of bytes to write/read.
 *
 * @retval true If transaction is done correctly.
 * @retval false If an error occurs.
 */
bool itf_spi_transaction(h_itf_spi_t h_itf_spi, const uint8_t * tx_data,
                         uint8_t * rx_data, size_t count);

/**
 * @brief Clear data from SPI interface.
 *
 * @param h_itf_spi Handler of the SPI interface to use.
 */
void itf_spi_flush(h_itf_spi_t h_itf_spi);

/**
 * @brief Lock the SPI interface to be used only by the current task.
 *
 * @param h_itf_spi Handler of the SPI interface to use.
 */
void itf_spi_lock(h_itf_spi_t h_itf_spi);

/**
 * @brief Unlock the SPI interface to be used by any task.
 *
 * @param h_itf_spi Handler of the SPI interface to use.
 */
void itf_spi_unlock(h_itf_spi_t h_itf_spi);

#endif // ITF_SPI_H

/** @} */

/******************************** End of file *********************************/
