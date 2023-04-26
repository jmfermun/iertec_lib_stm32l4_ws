/*******************************************************************************
 * @file test_crypt_base64.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 23 Mar 2023
 * @brief Unit test for the module crypt_base64.
 ******************************************************************************/

#include "crypt_base64.h"

#include <string.h>

#include "unity.h"

/****************************************************************************//*
 * Dependencies
 ******************************************************************************/

const char test_file_name[] = __FILE_NAME__;

//// System dependencies
//TEST_FILE("system_stm32l4xx.c")
//TEST_FILE("stm32l4xx_it.c")
//TEST_FILE("sysmem.c")
//
//// HAL dependencies
//TEST_FILE("stm32l4xx_hal.c")
//TEST_FILE("stm32l4xx_hal_cortex.c")
//TEST_FILE("stm32l4xx_hal_pwr_ex.c")
//TEST_FILE("stm32l4xx_hal_pwr.c")
//TEST_FILE("stm32l4xx_hal_rcc_ex.c")
//TEST_FILE("stm32l4xx_hal_rcc.c")
//TEST_FILE("stm32l4xx_hal_tim_ex.c")
//TEST_FILE("stm32l4xx_hal_tim.c")
//TEST_FILE("stm32l4xx_hal_timebase_tim.c")
//TEST_FILE("main.c")
//
//// Test support dependencies
//TEST_FILE("test_main.c")

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

#define BUFFER_SIZE (1024)
#define DATA_COUNT  (4)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

static const char data_decoded_0[] = "fo";
static const char data_decoded_1[] = "foobar";
static const char data_decoded_2[] = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
static const char data_decoded_3[] =
{
    0x24, 0x48, 0x6E, 0x56, 0x87, 0x62, 0x5A, 0xBD,
    0xBF, 0x17, 0xD9, 0xA2, 0xC4, 0x17, 0x1A, 0x01,
    0x94, 0xED, 0x8F, 0x1E, 0x11, 0xB3, 0xD7, 0x09,
    0x0C, 0xB6, 0xE9, 0x10, 0x6F, 0x22, 0xEE, 0x13,
    0xCA, 0xB3, 0x07, 0x05, 0x76, 0xC9, 0xFA, 0x31,
    0x6C, 0x08, 0x34, 0xFF, 0x8D, 0xC2, 0x6C, 0x38,
    0x00, 0x43, 0xE9, 0x54, 0x97, 0xAF, 0x50, 0x4B,
    0xD1, 0x41, 0xBA, 0x95, 0x31, 0x5A, 0x0B, 0x97
};

static const char data_encoded_0[] = "Zm8=";
static const char data_encoded_1[] = "Zm9vYmFy";
static const char data_encoded_2[] = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";
static const char data_encoded_3[] = "JEhuVodiWr2/F9mixBcaAZTtjx4Rs9cJDLbpEG8i7hPKswcFdsn6MWwINP+Nwmw4AEPpVJevUEvRQbqVMVoLlw==";

static const char * data_decoded[DATA_COUNT] =
{
    data_decoded_0,
    data_decoded_1,
    data_decoded_2,
    data_decoded_3,
};

static const size_t data_decoded_len[DATA_COUNT] =
{
    sizeof(data_decoded_0) - 1,
    sizeof(data_decoded_1) - 1,
    sizeof(data_decoded_2) - 1,
    sizeof(data_decoded_3),
};

static const char * data_encoded[DATA_COUNT] =
{
    data_encoded_0,
    data_encoded_1,
    data_encoded_2,
    data_encoded_3,
};

static const size_t data_encoded_len[DATA_COUNT] =
{
    sizeof(data_encoded_0) - 1,
    sizeof(data_encoded_1) - 1,
    sizeof(data_encoded_2) - 1,
    sizeof(data_encoded_3) - 1,
};

static uint8_t buffer[BUFFER_SIZE];

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_crypt_base64_encode(void)
{
    for (size_t i = 0; i < DATA_COUNT; i++)
    {
        memset(buffer, 0xA5, BUFFER_SIZE);
        memcpy(buffer, data_decoded[i], data_decoded_len[i]);

        size_t len = crypt_base64_encode(buffer, data_decoded_len[i]);

        TEST_PRINTF("ENC %u: %s", i, buffer);

        TEST_ASSERT_TRUE(len < BUFFER_SIZE);
        TEST_ASSERT_EQUAL(data_encoded_len[i], len);
        TEST_ASSERT_EQUAL_MEMORY(data_encoded[i], buffer, len);
        TEST_ASSERT_EQUAL('\0', buffer[len]);
    }
}

void test_crypt_base64_decode(void)
{
    for (size_t i = 0; i < DATA_COUNT; i++)
    {
        TEST_ASSERT_TRUE(data_encoded_len[i] < BUFFER_SIZE);

        memset(buffer, 0xA5, BUFFER_SIZE);
        memcpy(buffer, data_encoded[i], data_encoded_len[i]);

        size_t len = crypt_base64_decode(buffer, data_encoded_len[i]);

        TEST_PRINTF("DEC %u: %s", i, buffer);

        TEST_ASSERT_EQUAL(data_decoded_len[i], len);
        TEST_ASSERT_EQUAL_MEMORY(data_decoded[i], buffer, len);
    }
}

/******************************** End of file *********************************/
