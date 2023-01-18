/*******************************************************************************
 * @file itf_i2c.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 17 Jan 2023
 * @brief I2C interface driver.
 * @ingroup itf_i2c
 ******************************************************************************/

/**
 * @defgroup itf_i2c itf_i2c
 * @brief I2C interface driver.
 * @{
 */

#ifndef ITF_I2C_H
#define ITF_I2C_H

#include "itf_bsp.h"
#include "stm32l4xx_hal.h"

/** @brief I2C interface hardware configuration type. */
typedef struct
{
    I2C_HandleTypeDef * handle;
    itf_bsp_init_ll_t   init_ll;
} itf_i2c_config_t;

/**
 * @brief Initialization of the I2C interface.
 *
 * @param[in] h_itf_i2c Handler of the I2C interface to use.
 *
 * @retval true If the interface is initialized correctly.
 * @retval false If an error occurs.
 */
bool itf_i2c_init(h_itf_i2c_t h_itf_i2c);

/**
 * @brief Close the I2C interface and release its resources.
 *
 * @param[in] h_itf_i2c Handler of the I2C interface to use.
 *
 * @retval true If the interface is deinitialized correctly.
 * @retval false If an error occurs.
 */
bool itf_i2c_deinit(h_itf_i2c_t h_itf_i2c);

/**
 * @brief Do a write/read transaction with the I2C interface.
 *
 * @param[in] h_itf_i2c Handler of the I2C interface to use.
 * @param[in] tx_data Data to write.
 * @param[in] tx_count Number of bytes to write.
 * @param[out] rx_data Where the read data will be stored.
 * @param[in] rx_count Number of bytes to read.
 *
 * @retval true If transaction is done correctly.
 * @retval false If an error occurs.
 */
bool itf_i2c_transaction(h_itf_i2c_t h_itf_i2c, uint8_t slave_address,
                         const uint8_t * tx_data, size_t tx_count,
                         uint8_t * rx_data, size_t rx_count);

#endif // ITF_I2C_H

/** @} */

/******************************** End of file *********************************/
