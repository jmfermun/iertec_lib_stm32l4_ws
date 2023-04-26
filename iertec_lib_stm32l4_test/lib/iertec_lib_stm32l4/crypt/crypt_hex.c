/*******************************************************************************
 * @file crypt_hex.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 26 Apr 2023
 * @brief Implementation of the hexadecimal encoding algorithm.
 * @ingroup crypt_hex
 ******************************************************************************/

/**
 * @addtogroup crypt_hex
 * @{
 */

#include "crypt_hex.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Convert the low nibble of a byte into an ASCII character. */
#define NIBBLE_LOW_TO_HEX_ASCII(x)  (crypt_hex_nibble_to_ascii[((x) & 0x0F)])

/** Convert the high nibble of a byte into an ASCII character. */
#define NIBBLE_HIGH_TO_HEX_ASCII(x) (crypt_hex_nibble_to_ascii[(((x) >> 4) & 0x0F)])

/** Convert an ASCII character into a byte low nibble. */
#define HEX_ASCII_TO_NIBBLE_LOW(x)  (crypt_hex_ascii_to_nibble(x))

/** Convert an ASCII character into a byte high nibble. */
#define HEX_ASCII_TO_NIBBLE_HIGH(x) (crypt_hex_ascii_to_nibble(x) << 4)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** Lookup table to convert a binary nibble into an ASCII character. */
static const uint8_t crypt_hex_nibble_to_ascii[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
};

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Convert a single ASCII character into its corresponding binary nibble.
 *
 * @param data ASCII character to convert.
 *
 * @return Binary nibble.
 */
static uint8_t crypt_hex_ascii_to_nibble(uint8_t data);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

size_t
crypt_hex_encode (uint8_t * buffer, size_t length)
{
    for (int i = length - 1; i > -1; i--)
    {
        buffer[i * 2 + 1] = NIBBLE_LOW_TO_HEX_ASCII(buffer[i]);
        buffer[i * 2]     = NIBBLE_HIGH_TO_HEX_ASCII(buffer[i]);
    }

    // The length of the resulting hexadecimal ASCII string is the double of the
    // original data length
    length *= 2;

    // Null terminate the encoded data
    buffer[length] = '\0';

    return length;
}

size_t
crypt_hex_decode (uint8_t * buffer, size_t length)
{
    length /= 2;

    for (size_t i = 0; i < length; i++)
    {
        buffer[i] = HEX_ASCII_TO_NIBBLE_HIGH(buffer[i * 2])
                    | HEX_ASCII_TO_NIBBLE_LOW(buffer[i * 2 + 1]);
    }

    // Null terminate the decoded data
    buffer[length] = '\0';

    return length;
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static uint8_t
crypt_hex_ascii_to_nibble (uint8_t data)
{
    if (data >= '0' && data <= '9')
    {
        return data - '0';
    }
    else if (data >= 'a' && data <= 'f')
    {
        return data - 'a' + 10;
    }
    else if (data >= 'A' && data <= 'F')
    {
        return data - 'A' + 10;
    }
    else
    {
        return 0;
    }
}

/** @} */

/******************************** End of file *********************************/
