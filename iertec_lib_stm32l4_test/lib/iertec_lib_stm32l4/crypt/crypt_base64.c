/*******************************************************************************
 * @file crypt_base64.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 23 Mar 2023
 * @brief Implementation of the Base64 encoding algorithm.
 * @ingroup crypt_base64
 ******************************************************************************/

/**
 * @addtogroup crypt_base64
 * @{
 */

#include "crypt_base64.h"

#include <string.h>
#include <ctype.h>

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** Base64 encoding map. */
static const uint8_t crypt_base64_encode_map[] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Decode 4 base64 characters to 3 binary bytes.
 *
 * @param[out] a_4 4 base64 characters.
 * @param[in] a_3 3 binary bytes.
 */
static inline void crypt_base64_a_3_to_a_4(uint8_t * a_4, const uint8_t * a_3);

/**
 * @brief Encode 3 binary bytes to 4 base64 characters.
 *
 * @param[out] a_3 3 binary bytes.
 * @param[in] a_4 4 base64 characters.
 */
static inline void crypt_base64_a_4_to_a_3(uint8_t * a_3, const uint8_t * a_4);

/**
 * @brief Get the expected length of the encoded output.
 *
 * @param[in] length Length of the binary bytes.
 */
static size_t crypt_base64_encoded_length(size_t length);

/**
 * @brief Get the expected length of the decoded output.
 *
 * @param[in] buffer Buffer with the base64 characters.
 * @param[in] length Length of the base64 characters.
 */
static size_t crypt_base64_decoded_length(const uint8_t * buffer,
                                          size_t length);

/**
 * @brief Base64 decoding map.
 *
 * @param[in] c Base64 character.
 *
 * @return Decoded binary representation.
 */
static inline uint8_t crypt_base64_decode_map(uint8_t c);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

size_t
crypt_base64_encode (uint8_t * buffer, size_t length)
{
    uint8_t   a_3[3];
    uint8_t   a_4[4];
    int       i              = length % 3 - 1;
    int       j;
    size_t    encoded_length = crypt_base64_encoded_length(length);
    uint8_t * buffer_in      = buffer + length;
    uint8_t * buffer_out     = buffer + encoded_length;

    // Null terminate the encoded data
    *buffer_out = '\0';

    // If the buffer length is not a multiple of 3, fill the output with the
    // needed padding characters
    if (i < 0)
    {
        i = 2;
        j = 3;
    }
    else
    {
        for (j = 2; j > i; j--)
        {
            a_3[j] = '\0';
        }

        for (j = 3; j > i + 1; j--)
        {
            *--buffer_out = '=';
        }
    }

    while (length--)
    {
        a_3[i--] = *--buffer_in;

        if (i < 0)
        {
            crypt_base64_a_3_to_a_4(a_4, a_3);

            for (i = j; i >= 0; i--)
            {
                *--buffer_out = crypt_base64_encode_map[a_4[i]];
            }

            i = 2;
            j = 3;
        }
    }

    // Check for errors
    if ((buffer_out != buffer) || (buffer_in != buffer))
    {
        encoded_length = 0;
    }

    return encoded_length;
}

size_t
crypt_base64_decode (uint8_t * buffer, size_t length)
{
    int       i          = 0;
    uint8_t * buffer_in  = buffer;
    uint8_t * buffer_out = buffer;
    uint8_t   a_3[3];
    uint8_t   a_4[4];
    size_t    decoded_length = crypt_base64_decoded_length(buffer, length);

    while (length--)
    {
        if (!(isalnum(*buffer_in) || ('+' == *buffer_in) || ('/' == *buffer_in))
            || ('=' == *buffer_in))
        {
            break;
        }

        a_4[i++] = *(buffer_in++);

        if (i == 4)
        {
            for (i = 0; i < 4; i++)
            {
                a_4[i] = crypt_base64_decode_map(a_4[i]);
            }

            crypt_base64_a_4_to_a_3(a_3, a_4);

            for (i = 0; i < 3; i++)
            {
                *buffer_out++ = a_3[i];
            }

            i = 0;
        }
    }

    if (i > 0)
    {
        int j;

        for (j = i; j < 4; j++)
        {
            a_4[j] = '\0';
        }

        for (j = 0; j < 4; j++)
        {
            a_4[j] = crypt_base64_decode_map(a_4[j]);
        }

        crypt_base64_a_4_to_a_3(a_3, a_4);

        for (j = 0; j < i - 1; j++)
        {
            *buffer_out++ = a_3[j];
        }
    }

    // Check for errors
    if (buffer_out != (buffer + decoded_length))
    {
        buffer_out     = buffer;
        decoded_length = 0;
    }

    // Null terminate the decoded data
    *buffer_out++ = '\0';

    return decoded_length;
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static inline void
crypt_base64_a_3_to_a_4 (uint8_t * a_4, const uint8_t * a_3)
{
    a_4[0] = (a_3[0] & 0xfc) >> 2;
    a_4[1] = ((a_3[0] & 0x03) << 4) + ((a_3[1] & 0xf0) >> 4);
    a_4[2] = ((a_3[1] & 0x0f) << 2) + ((a_3[2] & 0xc0) >> 6);
    a_4[3] = (a_3[2] & 0x3f);
}

static inline void
crypt_base64_a_4_to_a_3 (uint8_t * a_3, const uint8_t * a_4)
{
    a_3[0] = (a_4[0] << 2) + ((a_4[1] & 0x30) >> 4);
    a_3[1] = ((a_4[1] & 0xf) << 4) + ((a_4[2] & 0x3c) >> 2);
    a_3[2] = ((a_4[2] & 0x3) << 6) + a_4[3];
}

static size_t
crypt_base64_encoded_length (size_t length)
{
    return (length + 2 - ((length + 2) % 3)) / 3 * 4;
}

static size_t
crypt_base64_decoded_length (const uint8_t * buffer, size_t length)
{
    int             num_eq     = 0;
    const uint8_t * buffer_end = buffer + length;

    while (*--buffer_end == '=')
    {
        ++num_eq;
    }

    return ((6 * length) / 8) - num_eq;
}

static inline uint8_t
crypt_base64_decode_map (uint8_t c)
{
    if ((c >= 'A') && (c <= 'Z'))
    {
        return c - 'A';
    }

    if ((c >= 'a') && (c <= 'z'))
    {
        return c - 71;
    }

    if ((c >= '0') && (c <= '9'))
    {
        return c + 4;
    }

    if (c == '+')
    {
        return 62;
    }

    if (c == '/')
    {
        return 63;
    }

    return 255;
}

/** @} */

/******************************** End of file *********************************/
