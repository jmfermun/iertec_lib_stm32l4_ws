/*******************************************************************************
 * @file crypt_checksum.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Apr 2023
 * @brief Implementation of the XOR checksum algorithm.
 * @ingroup crypt_checksum
 ******************************************************************************/

/**
 * @defgroup crypt_checksum crypt_checksum
 * @brief Implementation of the XOR checksum algorithm.
 * @{
 */

#ifndef CRYPT_CHECKSUM_H
#define CRYPT_CHECKSUM_H

#include <stdint.h>
#include <stddef.h>

/** Checksum initial value. */
#define CRYPT_CHECKSUM_INIT_VAL (0ul)

/**
 * @brief Calculate the checksum of the packet (XOR on all bytes).
 *
 * @param packet Pointer to the packet.
 * @param len Length of the packet.
 * @param crc Initial checksum value. It allows to compute the checksum in
 * chunks. To calculate it from the start, set this argument to @ref
 * CRYPT_CHECKSUM_INIT_VAL.
 *
 * @return Checksum of the packet.
 */
uint8_t crypt_checksum(const uint8_t * packet, size_t len, uint8_t checksum);

#endif // CRYPT_CHECKSUM_H

/** @} */

/******************************** End of file *********************************/
