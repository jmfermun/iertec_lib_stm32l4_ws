/*******************************************************************************
 * @file crypt_crc32.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 27 Apr 2023
 * @brief Implementation of the CRC32 algorithm according to IEEE standards.
 * Polynomials are represented in little endian form. Parameters:
 * - Width: 32 bit.
 * - Poly: 0xEDB88320.
 * - Output for "123456789": 0xCBF43926.
 * @ingroup crypt_crc32
 ******************************************************************************/

/**
 * @defgroup crypt_crc32 crypt_crc32
 * @brief Implementation of the CRC32 algorithm according to IEEE standards.
 * @{
 */

#ifndef CRYPT_CRC32_H
#define CRYPT_CRC32_H

#include <stdint.h>
#include <stddef.h>

/** CRC32 initial value. */
#define CRYPT_CRC32_INIT_VAL (0ul)

/**
 * @brief Calculate the CRC32 of the packet according to IEEE standards.
 * Algorithm obtained from https://github.com/LacobusVentura/MODBUS-CRC16.
 *
 * @param packet Pointer to the packet.
 * @param len Length of the packet.
 * @param crc Initial CRC value. It allows to compute the CRC in chunks. To
 * calculate it from the start, set this argument to @ref CRYPT_CRC32_INIT_VAL.
 *
 * @return CRC32 of the packet.
 */
uint32_t crypt_crc32(const uint8_t * packet, size_t len, uint32_t crc);

#endif // CRYPT_CRC32_H

/** @} */

/******************************** End of file *********************************/
