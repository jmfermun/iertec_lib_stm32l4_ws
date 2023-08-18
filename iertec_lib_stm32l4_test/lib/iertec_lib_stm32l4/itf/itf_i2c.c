/*******************************************************************************
 * @file itf_i2c.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 17 Jan 2023
 * @brief I2C interface driver.
 * @ingroup itf_i2c
 ******************************************************************************/

/**
 * @addtogroup itf_i2c
 * @{
 */

#include "itf_i2c.h"
#include "itf_pwr.h"

#include "FreeRTOS.h"
#include "semphr.h"

/****************************************************************************//*
 * Type definitions
 ******************************************************************************/

/** @brief I2C instance data needed during transactions. */
typedef struct
{
    I2C_HandleTypeDef * handle;
    SemaphoreHandle_t   mutex;
    SemaphoreHandle_t   semaphore;
    uint8_t             h_itf_pwr;
} itf_i2c_instance_t;

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const itf_i2c_config_t itf_i2c_config[H_ITF_I2C_COUNT];

/** Instances of the available I2C interfaces. */
static volatile itf_i2c_instance_t itf_i2c_instance[H_ITF_I2C_COUNT];

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Function to be called from the completion callbacks.
 *
 * @param[in] hi2c Pointer to a I2C_HandleTypeDef structure that contains the
 * configuration information for I2C module.
 */
static inline void itf_i2c_give_semaphore(const I2C_HandleTypeDef * h_i2c);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_i2c_init (h_itf_i2c_t h_itf_i2c)
{
    if (h_itf_i2c >= H_ITF_I2C_COUNT)
    {
        return false;
    }

    const itf_i2c_config_t *      config   = &itf_i2c_config[h_itf_i2c];
    volatile itf_i2c_instance_t * instance = &itf_i2c_instance[h_itf_i2c];

    // Low level initialization
    if (NULL != config->init_ll)
    {
        config->init_ll();
    }

    if (itf_bsp_get_error())
    {
        return false;
    }

    // Save the I2C instance to be used
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

    return true;
}

bool
itf_i2c_deinit (h_itf_i2c_t h_itf_i2c)
{
    if (h_itf_i2c >= H_ITF_I2C_COUNT)
    {
        return false;
    }

    volatile itf_i2c_instance_t * instance = &itf_i2c_instance[h_itf_i2c];

    if (HAL_I2C_DeInit(instance->handle) != HAL_OK)
    {
        return false;
    }

    instance->handle = NULL;

    return true;
}

bool
itf_i2c_transaction (h_itf_i2c_t h_itf_i2c, uint8_t slave_address,
                     const uint8_t * tx_data, size_t tx_count,
                     uint8_t * rx_data, size_t rx_count)
{
    volatile itf_i2c_instance_t * instance = &itf_i2c_instance[h_itf_i2c];
    HAL_StatusTypeDef             status   = HAL_OK;
    bool                          ret;

    // Slave address must be shifted to left
    slave_address <<= 1;

    (void)xSemaphoreTake(instance->mutex, portMAX_DELAY);

    itf_pwr_set_active(instance->h_itf_pwr);

    if ((NULL != tx_data) && (NULL != rx_data))
    {
        status = HAL_I2C_Master_Seq_Transmit_DMA(instance->handle,
                                                 slave_address,
                                                 (uint8_t *)tx_data, tx_count,
                                                 I2C_FIRST_FRAME);

        if (HAL_OK == status)
        {
            // Block until transaction completes
            (void)xSemaphoreTake(instance->semaphore, portMAX_DELAY);

            if (HAL_I2C_ERROR_NONE != instance->handle->ErrorCode)
            {
                status = HAL_ERROR;
            }
        }

        if (HAL_OK == status)
        {
            status = HAL_I2C_Master_Seq_Receive_DMA(instance->handle,
                                                    slave_address,
                                                    (uint8_t *)rx_data,
                                                    rx_count, I2C_LAST_FRAME);

            if (HAL_OK == status)
            {
                // Block until transaction completes
                (void)xSemaphoreTake(instance->semaphore, portMAX_DELAY);

                if (HAL_I2C_ERROR_NONE != instance->handle->ErrorCode)
                {
                    status = HAL_ERROR;
                }
            }
        }
    }
    else if (NULL != tx_data)
    {
        status = HAL_I2C_Master_Transmit_DMA(instance->handle, slave_address,
                                             (uint8_t *)tx_data, tx_count);

        if (HAL_OK == status)
        {
            // Block until transaction completes
            (void)xSemaphoreTake(instance->semaphore, portMAX_DELAY);

            if (HAL_I2C_ERROR_NONE != instance->handle->ErrorCode)
            {
                status = HAL_ERROR;
            }
        }
    }
    else if (NULL != rx_data)
    {
        status = HAL_I2C_Master_Receive_DMA(instance->handle, slave_address,
                                            (uint8_t *)rx_data, rx_count);

        if (HAL_OK == status)
        {
            // Block until transaction completes
            (void)xSemaphoreTake(instance->semaphore, portMAX_DELAY);

            if (HAL_I2C_ERROR_NONE != instance->handle->ErrorCode)
            {
                status = HAL_ERROR;
            }
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    itf_pwr_set_inactive(instance->h_itf_pwr);

    if (HAL_OK == status)
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    (void)xSemaphoreGive(instance->mutex);

    return ret;
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

void
HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef * h_i2c)
{
    itf_i2c_give_semaphore(h_i2c);
}

void
HAL_I2C_MasterRxCpltCallback (I2C_HandleTypeDef * h_i2c)
{
    itf_i2c_give_semaphore(h_i2c);
}

void
HAL_I2C_ErrorCallback (I2C_HandleTypeDef * h_i2c)
{
    itf_i2c_give_semaphore(h_i2c);
}

static inline void
itf_i2c_give_semaphore (const I2C_HandleTypeDef * h_i2c)
{
    BaseType_t                    b_yield  = pdFALSE;
    volatile itf_i2c_instance_t * instance = NULL;

    for (size_t i = 0u; i < H_ITF_I2C_COUNT; i++)
    {
        if (itf_i2c_instance[i].handle == h_i2c)
        {
            instance = &itf_i2c_instance[i];
            break;
        }
    }

    if (NULL != instance)
    {
        // Notify to task the end of the UART transaction
        (void)xSemaphoreGiveFromISR(instance->semaphore, &b_yield);
    }

    portYIELD_FROM_ISR(b_yield);
}

/** @} */

/******************************** End of file *********************************/
