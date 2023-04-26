/*******************************************************************************
 * @file crypt_base64.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 23 Mar 2023
 * @brief Implementation of the Base64 encoding algorithm.
 * @ingroup crypt_base64
 ******************************************************************************/

/**
 * @defgroup crypt_base64 crypt_base64
 * @brief Implementation of the Base64 encoding algorithm.
 * @{
 */

#ifndef CRYPT_BASE64_H
#define CRYPT_BASE64_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Encode into a base64 string a binary data buffer and return the
 * encoded data in the same buffer.
 *
 * @param[in, out] buffer Binary data as input. Base64 string as output.
 * @param[in] length Binary data length.
 *
 * @return Base64 string length.
 */
size_t crypt_base64_encode(uint8_t * buffer, size_t length);

/**
 * @brief Decode a base64 string and return the decoded data in the same buffer.
 *
 * @param[in, out] buffer Base64 string as input. Binary data as output.
 * @param[in] length Base64 string length.
 *
 * @return Binary data length.
 */
size_t crypt_base64_decode(uint8_t * buffer, size_t length);

#endif // CRYPT_BASE64_H

/** @} */

/******************************** End of file *********************************/
