/*******************************************************************************
 * @file itf_flash.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 03 May 2023
 * @brief Microcontroller internal flash driver.
 * @ingroup itf_flash
 ******************************************************************************/

/**
 * @defgroup itf_flash itf_flash
 * @brief Microcontroller internal flash driver.
 * @{
 */

#ifndef ITF_FLASH_H
#define ITF_FLASH_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Erase the memory region starting at the indicated address and with the
 * indicated length.
 *
 * @param address The region starting address.
 * @param length Number of bytes to erase.
 *
 * @retval true Operation executed correctly.
 * @retval false An error occurred.
 */
bool itf_flash_erase(uint32_t address, size_t length);

/**
 * @brief Write a byte array starting at a given address of memory of the
 * device.
 *
 * @param address Starting address to be written.
 * @param data Data to write.
 * @param length Number of bytes to write.
 *
 * @retval true Operation executed correctly.
 * @retval false An error occurred.
 */
bool itf_flash_write(uint32_t address, const uint8_t * data, size_t length);

/**
 * @brief Read a byte array starting from a given address of memory of the
 * device.
 *
 * @param address Starting address to be written.
 * @param data Data read.
 * @param length Number of bytes to read.
 *
 * @retval true Operation executed correctly.
 * @retval false An error occurred.
 */
bool itf_flash_read(uint32_t address, uint8_t * data, size_t length);

#endif // ITF_FLASH_H

/** @} */

/******************************** End of file *********************************/
