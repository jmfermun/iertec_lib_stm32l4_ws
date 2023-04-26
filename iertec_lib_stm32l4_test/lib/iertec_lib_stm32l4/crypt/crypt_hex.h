/*******************************************************************************
 * @file crypt_hex.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 26 Apr 2023
 * @brief Implementation of the hexadecimal encoding algorithm.
 * @ingroup crypt_hex
 ******************************************************************************/

/**
 * @defgroup crypt_hex crypt_hex
 * @brief Implementation of the hexadecimal encoding algorithm.
 * @{
 */

#ifndef CRYPT_HEX_H
#define CRYPT_HEX_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Encode into an hexadecimal string a binary data buffer and return the
 * encoded data in the same buffer.
 *
 * @param[in, out] buffer Binary data as input. Base64 string as output.
 * @param[in] length Binary data length.
 *
 * @return Base64 string length.
 */
size_t crypt_hex_encode(uint8_t * buffer, size_t length);

/**
 * @brief Decode an hexadecimal string and return the decoded data in the same
 * buffer.
 *
 * @param[in, out] buffer Base64 string as input. Binary data as output.
 * @param[in] length Base64 string length.
 *
 * @return Binary data length.
 */
size_t crypt_hex_decode(uint8_t * buffer, size_t length);

#endif // CRYPT_HEX_H

/** @} */

/******************************** End of file *********************************/
