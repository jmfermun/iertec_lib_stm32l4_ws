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

#include "FreeRTOS.h"
#include "semphr.h"

/****************************************************************************//*
 * Type definitions
 ******************************************************************************/

/** @brief SPI instance data needed during transactions. */
typedef struct
{
    SPI_HandleTypeDef * handle;
    SemaphoreHandle_t   mutex;
    SemaphoreHandle_t   semaphore;
} itf_spi_instance_t;

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const itf_spi_config_t itf_spi_config[H_ITF_SPI_COUNT];

/** Instances of the available SPI interfaces. */
static itf_spi_instance_t itf_spi_instance[H_ITF_SPI_COUNT];

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Function to be called from the completion callbacks.
 *
 * @param hspi Pointer to a SPI_HandleTypeDef structure that contains the
 * configuration information for SPI module.
 */
static inline void itf_spi_give_semaphore(SPI_HandleTypeDef * h_spi);

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

    // Low level initialization
    if (NULL != config->init_ll)
    {
        config->init_ll();
    }

    // Save the SPI instance to be used
    instance->handle = config->handle;

    // Create the mutex and semaphore
    instance->mutex = xSemaphoreCreateMutex();
    instance->semaphore = xSemaphoreCreateBinary();

    if ((NULL == instance->mutex) || (NULL == instance->semaphore))
    {
        return false;
    }

    return true;
}

void
itf_spi_deinit (h_itf_spi_t h_itf_spi)
{
    if (h_itf_spi >= H_ITF_SPI_COUNT)
    {
        return;
    }

    itf_spi_instance_t * instance = &itf_spi_instance[h_itf_spi];

    if (NULL != instance->handle)
    {
        HAL_SPI_DeInit(instance->handle);
        instance->handle = NULL;
    }
}

bool
itf_spi_transaction (h_itf_spi_t h_itf_spi, const uint8_t * tx_data,
                     uint8_t * rx_data, size_t count)
{
    itf_spi_instance_t * instance = &itf_spi_instance[h_itf_spi];
    HAL_StatusTypeDef status;

    if ((NULL != tx_data) && (NULL != rx_data))
    {
        status = HAL_SPI_TransmitReceive_DMA(instance->handle, (uint8_t *)tx_data,
                                             rx_data, count);
    }
    else if (NULL != tx_data)
    {
        status = HAL_SPI_Transmit_DMA(instance->handle, (uint8_t *)tx_data,
                                      count);
    }
    else if (NULL != rx_data)
    {
        status = HAL_SPI_Receive_DMA(instance->handle, rx_data, count);
    }
    else
    {
        status = HAL_ERROR;
    }

    if (status != HAL_OK)
    {
        return false;
    }

    // Block until transmission completes
    xSemaphoreTake(instance->semaphore, portMAX_DELAY);

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
itf_spi_lock (h_itf_spi_t h_itf_spi)
{
    itf_spi_instance_t * instance = &itf_spi_instance[h_itf_spi];

    (void)xSemaphoreTake(instance->mutex, portMAX_DELAY);
}

void
itf_spi_unlock (h_itf_spi_t h_itf_spi)
{
    itf_spi_instance_t * instance = &itf_spi_instance[h_itf_spi];

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
itf_spi_give_semaphore(SPI_HandleTypeDef * h_spi)
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
