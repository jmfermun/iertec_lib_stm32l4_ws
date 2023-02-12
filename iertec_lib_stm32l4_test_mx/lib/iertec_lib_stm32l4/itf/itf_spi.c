/*******************************************************************************
 * @file itf_spi.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 23 Dec 2022
 * @brief SPI interface driver.
 * @ingroup itf_spi
 ******************************************************************************/

/**
 * @addtogroup itf_spi
 * @{
 */

#include "itf_spi.h"
#include "itf_io.h"
#include "itf_pwr.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include <string.h>

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Chip Select on logic value. */
#define ITF_SPI_CS_ON  (ITF_IO_LOW)

/** Chip Select off logic value. */
#define ITF_SPI_CS_OFF (ITF_IO_HIGH)

/****************************************************************************//*
 * Type definitions
 ******************************************************************************/

/** @brief SPI instance data needed during transactions. */
typedef struct
{
    SPI_HandleTypeDef * handle;
    SemaphoreHandle_t   mutex;
    SemaphoreHandle_t   semaphore;
    uint8_t             h_itf_pwr;
    itf_spi_mode_t      mode;
} itf_spi_instance_t;

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const itf_spi_config_t itf_spi_config[H_ITF_SPI_COUNT];
extern const itf_spi_chip_config_t itf_spi_chip_config[H_ITF_SPI_CHIP_COUNT];

/** Instances of the available SPI interfaces. */
static itf_spi_instance_t itf_spi_instance[H_ITF_SPI_COUNT];

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Function to be called from the completion callbacks.
 *
 * @param[in] hspi Pointer to a SPI_HandleTypeDef structure that contains the
 * configuration information for SPI module.
 */
static inline void itf_spi_give_semaphore(const SPI_HandleTypeDef * h_spi);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_spi_init (h_itf_spi_t h_itf_spi)
{
    if (h_itf_spi >= H_ITF_SPI_COUNT)
    {
        return false;
    }

    const itf_spi_config_t * config   = &itf_spi_config[h_itf_spi];
    itf_spi_instance_t *     instance = &itf_spi_instance[h_itf_spi];

    if (NULL != instance->handle)
    {
        return false;
    }

    // Low level initialization
    if (NULL != config->init_ll)
    {
        config->init_ll();
    }

    if (itf_bsp_get_error())
    {
        return false;
    }

    // Save the SPI instance to be used
    instance->handle = config->handle;

    // Create the mutex and semaphore
    instance->mutex     = xSemaphoreCreateMutex();
    instance->semaphore = xSemaphoreCreateBinary();

    if ((NULL == instance->mutex) || (NULL == instance->semaphore))
    {
        return false;
    }

    instance->h_itf_pwr = itf_pwr_register(ITF_PWR_LEVEL_0);

    if (H_ITF_PWR_NONE == instance->h_itf_pwr)
    {
        return false;
    }

    // Update the currently configured SPI mode
    uint32_t cpol = instance->handle->Init.CLKPolarity;
    uint32_t cpha = instance->handle->Init.CLKPhase;

    if ((SPI_POLARITY_LOW == cpol) && (SPI_PHASE_1EDGE == cpha))
    {
        instance->mode = ITF_SPI_MODE_POL0_PHA0;
    }
    else if ((SPI_POLARITY_LOW == cpol) && (SPI_PHASE_2EDGE == cpha))
    {
        instance->mode = ITF_SPI_MODE_POL0_PHA1;
    }
    else if ((SPI_POLARITY_HIGH == cpol) && (SPI_PHASE_1EDGE == cpha))
    {
        instance->mode = ITF_SPI_MODE_POL1_PHA0;
    }
    else if ((SPI_POLARITY_HIGH == cpol) && (SPI_PHASE_2EDGE == cpha))
    {
        instance->mode = ITF_SPI_MODE_POL1_PHA1;
    }
    else
    {
        return false;
    }

    return true;
}

bool
itf_spi_deinit (h_itf_spi_t h_itf_spi)
{
    if (h_itf_spi >= H_ITF_SPI_COUNT)
    {
        return false;
    }

    itf_spi_instance_t * instance = &itf_spi_instance[h_itf_spi];

    if (HAL_SPI_DeInit(instance->handle) != HAL_OK)
    {
        return false;
    }

    instance->handle = NULL;

    return true;
}

bool
itf_spi_transaction (h_itf_spi_t h_itf_spi, const uint8_t * tx_data,
                     uint8_t * rx_data, size_t count)
{
    itf_spi_instance_t * instance = &itf_spi_instance[h_itf_spi];
    HAL_StatusTypeDef    status;

    itf_pwr_set_active(instance->h_itf_pwr);

    if ((NULL != tx_data) && (NULL != rx_data))
    {
        status = HAL_SPI_TransmitReceive_DMA(instance->handle,
                                             (uint8_t *)tx_data, rx_data,
                                             count);
    }
    else if (NULL != tx_data)
    {
        status = HAL_SPI_Transmit_DMA(instance->handle, (uint8_t *)tx_data,
                                      count);
    }
    else if (NULL != rx_data)
    {
        // HAL library uses the contents of rx_data buffer as tx_data, so clean
        // the buffer
        memset(rx_data, 0, count);

        status = HAL_SPI_Receive_DMA(instance->handle, rx_data, count);
    }
    else
    {
        status = HAL_ERROR;
    }

    if (status != HAL_OK)
    {
        itf_pwr_set_inactive(instance->h_itf_pwr);

        return false;
    }

    // Block until transmission completes
    xSemaphoreTake(instance->semaphore, portMAX_DELAY);

    itf_pwr_set_inactive(instance->h_itf_pwr);

    if (instance->handle->ErrorCode != HAL_SPI_ERROR_NONE)
    {
        return false;
    }

    return true;
}

void
itf_spi_flush (h_itf_spi_t h_itf_spi)
{
    itf_spi_instance_t * instance = &itf_spi_instance[h_itf_spi];

    while (__HAL_SPI_GET_FLAG(instance->handle, SPI_FLAG_BSY))
    {
        // Wait until SPI hardware module is done
    }
}

void
itf_spi_select (h_itf_spi_chip_t h_itf_spi_chip)
{
    const itf_spi_chip_config_t * config = &itf_spi_chip_config[h_itf_spi_chip];
    itf_spi_instance_t * instance = &itf_spi_instance[config->h_itf_spi];

    (void)xSemaphoreTake(instance->mutex, portMAX_DELAY);

    // Change mode if necessary
    if (config->mode != instance->mode)
    {
        instance->mode = config->mode;

        switch (config->mode)
        {
            case ITF_SPI_MODE_POL0_PHA0:
                instance->handle->Init.CLKPolarity = SPI_POLARITY_LOW;
                instance->handle->Init.CLKPhase = SPI_PHASE_1EDGE;
            break;

            case ITF_SPI_MODE_POL0_PHA1:
                instance->handle->Init.CLKPolarity = SPI_POLARITY_LOW;
                instance->handle->Init.CLKPhase = SPI_PHASE_2EDGE;
            break;

            case ITF_SPI_MODE_POL1_PHA0:
                instance->handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
                instance->handle->Init.CLKPhase = SPI_PHASE_1EDGE;
            break;

            case ITF_SPI_MODE_POL1_PHA1:
            default:
                instance->handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
                instance->handle->Init.CLKPhase = SPI_PHASE_2EDGE;
            break;
        }

        // Disable SPI peripheral
        __HAL_SPI_DISABLE(instance->handle);

        // Modify SPI mode
        uint32_t mode_mask = SPI_CR1_CPOL_Msk | SPI_CR1_CPHA_Msk;
        uint32_t mode_value = instance->handle->Init.CLKPolarity
                              | instance->handle->Init.CLKPhase;
        MODIFY_REG(instance->handle->Instance->CR1, mode_mask, mode_value);
    }

    // Enable SPI peripheral if necessary
    if ((instance->handle->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
    {
        __HAL_SPI_ENABLE(instance->handle);
    }

    if (H_ITF_IO_NONE != config->pin_cs)
    {
        itf_io_set_value(config->pin_cs, ITF_SPI_CS_ON);
    }
}

void
itf_spi_deselect (h_itf_spi_chip_t h_itf_spi_chip)
{
    const itf_spi_chip_config_t * config = &itf_spi_chip_config[h_itf_spi_chip];
    itf_spi_instance_t * instance = &itf_spi_instance[config->h_itf_spi];

    if (H_ITF_IO_NONE != config->pin_cs)
    {
        itf_io_set_value(config->pin_cs, ITF_SPI_CS_OFF);
    }

    (void)xSemaphoreGive(instance->mutex);
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

void
HAL_SPI_TxCpltCallback (SPI_HandleTypeDef * h_spi)
{
    itf_spi_give_semaphore(h_spi);
}

void
HAL_SPI_RxCpltCallback (SPI_HandleTypeDef * h_spi)
{
    itf_spi_give_semaphore(h_spi);
}

void
HAL_SPI_TxRxCpltCallback (SPI_HandleTypeDef * h_spi)
{
    itf_spi_give_semaphore(h_spi);
}

void
HAL_SPI_ErrorCallback (SPI_HandleTypeDef * h_spi)
{
    itf_spi_give_semaphore(h_spi);
}

static inline void
itf_spi_give_semaphore (const SPI_HandleTypeDef * h_spi)
{
    BaseType_t           b_yield  = pdFALSE;
    itf_spi_instance_t * instance = NULL;

    for (size_t i = 0; i < H_ITF_SPI_COUNT; i++)
    {
        if (itf_spi_instance[i].handle == h_spi)
        {
            instance = &itf_spi_instance[i];
            break;
        }
    }

    if (NULL != instance)
    {
        // Notify to task the end of the UART transaction
        xSemaphoreGiveFromISR(instance->semaphore, &b_yield);
    }

    portYIELD_FROM_ISR(b_yield);
}

/** @} */

/******************************** End of file *********************************/
