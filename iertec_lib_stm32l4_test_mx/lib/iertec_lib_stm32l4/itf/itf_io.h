/*******************************************************************************
 * @file itf_io.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 22 Dec 2022
 * @brief Input/Output digital signals interface.
 * @ingroup itf_io
 ******************************************************************************/

/**
 * @defgroup itf_io itf_io
 * @brief Input/Output digital signals interface.
 * @{
 */

#ifndef ITF_IO_H
#define ITF_IO_H

#include "itf_bsp.h"
#include "stm32l4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

/** Low logic value. */
#define ITF_IO_LOW    (0)

/** High logic value. */
#define ITF_IO_HIGH   (1)

/** Chip Select on logic value. */
#define ITF_IO_CS_ON  (ITF_IO_LOW)

/** Chip Select off logic value. */
#define ITF_IO_CS_OFF (ITF_IO_HIGH)

/** @brief Type of the callbacks used as ISR. */
typedef void (* itf_io_int_cb_t)(void);

/** @brief I/O interface hardware configuration type. */
typedef struct
{
    GPIO_TypeDef * port;
    uint16_t       pin;
} itf_io_config_t;

/**
 * @brief I/O interface initialization.
 *
 * @retval true If the interface is initialized correctly.
 * @retval false If an error occurs.
 */
bool itf_io_init(void);

/**
 * @brief Set an interrupt callback.
 *
 * @param[in] h_itf_io Interrupt pin to configure.
 * @param[in] cb Callback that will handle the pin interrupts.
 */
void itf_io_set_int_cb(h_itf_io_t h_itf_io, itf_io_int_cb_t cb);

/**
 * @brief Set the output value of the indicated pin.
 *
 * @param[in] h_itf_io Pin to set.
 * @param[in] value Can be ITF_IO_LOW or ITF_IO_HIGH.
 */
void itf_io_set_value(h_itf_io_t h_itf_io, uint8_t value);

/**
 * @brief Get the input value of the indicated pin.
 *
 * @param[in] h_itf_io Pin to get.
 *
 * @return Value can be ITF_IO_LOW or ITF_IO_HIGH.
 */
uint8_t itf_io_get_value(h_itf_io_t h_itf_io);

/**
 * @brief Toggle the value of the indicated pin.
 *
 * @param[in] h_itf_io Pin to toggle.
 */
void itf_io_toggle_value(h_itf_io_t h_itf_io);

#endif // ITF_IO_H

/** @} */

/******************************** End of file *********************************/
