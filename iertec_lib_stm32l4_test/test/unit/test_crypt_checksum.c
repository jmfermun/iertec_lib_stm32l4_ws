/*******************************************************************************
 * @file test_crypt_checksum.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 27 Apr 2023
 * @brief Unit test for the module crypt_checksum.
 ******************************************************************************/

#include "crypt_checksum.h"

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

#define DATA_COUNT  (5)
#define CHUNK_SIZE  (8)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

static const char data_0[] = "123456789";
static const char data_1[] = "fo";
static const char data_2[] = "foobar";
static const char data_3[] = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
static const char data_4[] =
{
    0x24, 0x48, 0x6E, 0x56, 0x87, 0x62, 0x5A, 0xBD,
    0xBF, 0x17, 0xD9, 0xA2, 0xC4, 0x17, 0x1A, 0x01,
    0x94, 0xED, 0x8F, 0x1E, 0x11, 0xB3, 0xD7, 0x09,
    0x0C, 0xB6, 0xE9, 0x10, 0x6F, 0x22, 0xEE, 0x13,
    0xCA, 0xB3, 0x07, 0x05, 0x76, 0xC9, 0xFA, 0x31,
    0x6C, 0x08, 0x34, 0xFF, 0x8D, 0xC2, 0x6C, 0x38,
    0x00, 0x43, 0xE9, 0x54, 0x97, 0xAF, 0x50, 0x4B,
    0xD1, 0x41, 0xBA, 0x95, 0x31, 0x5A, 0x0B, 0x97,
};

// CRC32 value generated using https://checksumcalc.com/
static const uint8_t data_checksum_0 = 0x31;
static const uint8_t data_checksum_1 = 0x09;
static const uint8_t data_checksum_2 = 0x17;
static const uint8_t data_checksum_3 = 0x5C;
static const uint8_t data_checksum_4 = 0x04;

static const char * data[DATA_COUNT] =
{
    data_0,
    data_1,
    data_2,
    data_3,
    data_4,
};

static const size_t data_len[DATA_COUNT] =
{
    sizeof(data_0) - 1,
    sizeof(data_1) - 1,
    sizeof(data_2) - 1,
    sizeof(data_3) - 1,
    sizeof(data_4),
};

static const uint8_t data_checksum[DATA_COUNT] =
{
    data_checksum_0,
    data_checksum_1,
    data_checksum_2,
    data_checksum_3,
    data_checksum_4,
};

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_crypt_checksum_at_once(void)
{
    for (size_t i = 0; i < DATA_COUNT; i++)
    {
        uint32_t checksum = crypt_checksum((const uint8_t *)data[i], data_len[i], CRYPT_CHECKSUM_INIT_VAL);

        TEST_ASSERT_EQUAL_UINT32(data_checksum[i], checksum);
    }
}
void test_crypt_checksum_by_chuncks(void)
{
    for (size_t i = 0; i < DATA_COUNT; i++)
    {
        uint32_t checksum  = CRYPT_CHECKSUM_INIT_VAL;
        size_t chunks = data_len[i] / CHUNK_SIZE;
        size_t rem    = data_len[i] % CHUNK_SIZE;

        for (size_t j = 0; j < chunks; j++)
        {
            const uint8_t * data_chunk = (const uint8_t *)&data[i][j * CHUNK_SIZE];

            checksum = crypt_checksum(data_chunk, CHUNK_SIZE, checksum);
        }

        if (rem > 0)
        {
            const uint8_t * data_chunk = (const uint8_t *)&data[i][chunks * CHUNK_SIZE];

            checksum = crypt_checksum(data_chunk, rem, checksum);
        }

        TEST_ASSERT_EQUAL_UINT32(data_checksum[i], checksum);
    }
}

/******************************** End of file *********************************/
