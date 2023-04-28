/*******************************************************************************
 * @file crypt_checksum.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 28 Apr 2023
 * @brief Implementation of the XOR checksum algorithm.
 * @ingroup crypt_checksum
 ******************************************************************************/

/**
 * @addtogroup crypt_checksum
 * @{
 */

#include "crypt_checksum.h"

/****************************************************************************//*
 * Public code
 ******************************************************************************/

uint8_t
crypt_checksum (const uint8_t * packet, size_t len, uint8_t checksum)
{
    for (size_t i = 0; i < len; i++)
    {
        checksum ^= packet[i];
    }

    return checksum;
}

/** @} */

/******************************** End of file *********************************/
