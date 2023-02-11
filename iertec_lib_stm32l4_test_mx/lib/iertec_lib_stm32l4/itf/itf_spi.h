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

/** @brief SPI data frame formats. */
typedef enum
{
    ITF_SPI_MODE_POL0_PHA0 = 0,
    ITF_SPI_MODE_POL0_PHA1,
    ITF_SPI_MODE_POL1_PHA0,
    ITF_SPI_MODE_POL1_PHA1,
} itf_spi_mode_t;

/** @brief SPI interface hardware configuration type. */
typedef struct
{
    SPI_HandleTypeDef * handle;
    itf_bsp_init_ll_t   init_ll;
} itf_spi_config_t;

/** @brief SPI chip interface hardware configuration type. */
typedef struct
{
    h_itf_spi_t    h_itf_spi;
    h_itf_io_t     pin_cs;
    itf_spi_mode_t mode;
} itf_spi_chip_config_t;

/**
 * @brief Initialization of the SPI interface.
 *
 * @param[in] h_itf_spi Handler of the SPI interface to use.
 *
 * @retval true If the interface is initialized correctly.
 * @retval false If an error occurs.
 */
bool itf_spi_init(h_itf_spi_t h_itf_spi);

/**
 * @brief Close the SPI interface and release its resources.
 *
 * @param[in] h_itf_spi Handler of the SPI interface to use.
 *
 * @retval true If the interface is deinitialized correctly.
 * @retval false If an error occurs.
 */
bool itf_spi_deinit(h_itf_spi_t h_itf_spi);

/**
 * @brief Do a write/read transaction with the SPI interface.
 *
 * @param[in] h_itf_spi Handler of the SPI interface to use.
 * @param[in] tx_data Data to write.
 * @param[out] rx_data Where the read data will be stored.
 * @param[in] count Number of bytes to write/read.
 *
 * @retval true If transaction is done correctly.
 * @retval false If an error occurs.
 */
bool itf_spi_transaction(h_itf_spi_t h_itf_spi, const uint8_t * tx_data,
                         uint8_t * rx_data, size_t count);

/**
 * @brief Clear data from SPI interface.
 *
 * @param[in] h_itf_spi Handler of the SPI interface to use.
 */
void itf_spi_flush(h_itf_spi_t h_itf_spi);

/**
 * @brief Activate the chip select signal and lock the SPI interface associated
 * with it to be used only by the current task.
 *
 * @param[in] h_itf_spi_chip Handler of the SPI chip interface to use.
 */
void itf_spi_select(h_itf_spi_chip_t h_itf_spi_chip);

/**
 * @brief Deactivate the chip select signal and unlock the SPI interface
 * associated with it to be used by any task.
 *
 * @param[in] h_itf_spi_chip Handler of the SPI chip interface to use.
 */
void itf_spi_deselect(h_itf_spi_chip_t h_itf_spi_chip);

#endif // ITF_SPI_H

/** @} */

/******************************** End of file *********************************/
