/*******************************************************************************
 * @file int_flash.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 03 May 2023
 * @brief Microcontroller internal flash driver.
 * @ingroup itf_flash
 ******************************************************************************/

/**
 * @addtogroup itf_flash
 * @{
 */

#include "itf_flash.h"

#include "stm32l4xx_hal.h"

#include <string.h>

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Size of a word. */
#define ITF_FLASH_WORD_BYTES  (4)

/** Size of a double word. */
#define ITF_FLASH_DWORD_BYTES (8)

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Get the page of a given address.
 *
 * @param[in] addr Address of the FLASH memory.
 *
 * @return The page of a given address.
 */
static uint32_t itf_flash_get_page(uint32_t addr);

/**
 * @brief Get the bank of a given address.
 *
 * @param[in] addr Address of the FLASH memory.
 *
 * @return The bank of a given address.
 */
static uint32_t itf_flash_get_bank(uint32_t addr);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_flash_erase (uint32_t address, size_t length)
{
    bool                   ret;
    FLASH_EraseInitTypeDef erase_init_struct;
    uint32_t               erase_error = 0;

    ret = HAL_FLASH_Unlock() == HAL_OK;

    if (ret)
    {
        // Clear OPTVERR bit set on virgin samples
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

        uint32_t page_first = itf_flash_get_page(address);
        uint32_t page_end   = itf_flash_get_page(address + length - 1)
                              - page_first + 1;

        // Fill erase init structure
        erase_init_struct.TypeErase = FLASH_TYPEERASE_PAGES;
        erase_init_struct.Banks     = itf_flash_get_bank(address);
        erase_init_struct.Page      = page_first;
        erase_init_struct.NbPages   = page_end;

        // Perform the erase operation
        ret = HAL_FLASHEx_Erase(&erase_init_struct, &erase_error) == HAL_OK;
    }

    ret = HAL_FLASH_Lock() == HAL_OK;

    return ret;
}

bool
itf_flash_write (uint32_t address, const uint8_t * data, size_t length)
{
    bool ret;

    ret = HAL_FLASH_Unlock() == HAL_OK;

    for (size_t i = 0; ret && (i < length); i += ITF_FLASH_DWORD_BYTES)
    {
        uint64_t dword = *((uint64_t *)&data[i]);

        ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address + i,
                                dword) == HAL_OK;
    }

    ret = HAL_FLASH_Lock() == HAL_OK;

    return ret;
}

bool
itf_flash_read (uint32_t address, uint8_t * data, size_t length)
{
    for (size_t i = 0; i < length; i += ITF_FLASH_WORD_BYTES)
    {
        uint32_t word = *(__IO uint32_t *)(address + i);

        (void)memcpy(data + i, &word, ITF_FLASH_WORD_BYTES);
    }

    return true;
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static uint32_t
itf_flash_get_page (uint32_t addr)
{
    uint32_t page;

#ifdef FLASH_BANK_2
    if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
        // Bank 1
        page = (addr - FLASH_BASE) / FLASH_PAGE_SIZE;
    }
    else
    {
        // Bank 2
        page = (addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
    }
#else
    // Bank 1
    page = (addr - FLASH_BASE) / FLASH_PAGE_SIZE;
#endif

    return page;
}

static uint32_t
itf_flash_get_bank (uint32_t addr)
{
    uint32_t bank;

#ifdef FLASH_BANK_2
    if (addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
        // Bank 1
        bank = FLASH_BANK_1;
    }
    else
    {
        // Bank 2
        bank = FLASH_BANK_2;
    }
#else
    // Bank 1
    bank = FLASH_BANK_1;
#endif

    return bank;
}

/** @} */

/******************************** End of file *********************************/
