/*******************************************************************************
 * @file itf_io.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 22 Dec 2022
 * @brief Input/Output digital signals interface.
 * @ingroup itf_io
 ******************************************************************************/

/**
 * @addtogroup itf_io
 * @{
 */

#include "itf_io.h"

#include <stddef.h>

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const itf_io_config_t   itf_io_config[H_ITF_IO_COUNT];
extern const itf_bsp_init_ll_t itf_io_init_ll;

/** Registered interrupt callbacks. */
static itf_io_int_cb_t itf_io_int_cb[H_ITF_IO_INT_COUNT];

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_io_init (void)
{
    // Low level initialization
    if (NULL != itf_io_init_ll)
    {
        itf_io_init_ll();
    }

    if (itf_bsp_get_error())
    {
        return false;
    }

    // Initialize interrupt handlers
    for (unsigned int i = 0u; i < H_ITF_IO_INT_COUNT; i++)
    {
        uint32_t exti_line = itf_io_config[i].pin;

        // Set interrupt callback to NULL
        itf_io_int_cb[i] = NULL;

        // Disable interrupt
        EXTI->IMR1 &= ~(exti_line);
        __HAL_GPIO_EXTI_CLEAR_IT(exti_line);
    }

    return true;
}

void
itf_io_set_int_cb (h_itf_io_t h_itf_io, itf_io_int_cb_t cb)
{
    if (h_itf_io < H_ITF_IO_INT_COUNT)
    {
        const itf_io_config_t * config    = &itf_io_config[h_itf_io];
        uint32_t                exti_line = config->pin;

        itf_io_int_cb[h_itf_io] = cb;

        if (cb != NULL)
        {
            // Enable interrupt
            __HAL_GPIO_EXTI_CLEAR_IT(exti_line);
            EXTI->IMR1 |= exti_line;
        }
        else
        {
            // Disable interrupt
            EXTI->IMR1 &= ~(exti_line);
            __HAL_GPIO_EXTI_CLEAR_IT(exti_line);
        }
    }
}

void
itf_io_set_value (h_itf_io_t h_itf_io, uint8_t value)
{
    const itf_io_config_t * config = &itf_io_config[h_itf_io];
    GPIO_PinState           state;

    if (ITF_IO_LOW == value)
    {
        state = GPIO_PIN_RESET;
    }
    else
    {
        state = GPIO_PIN_SET;
    }

    HAL_GPIO_WritePin(config->port, config->pin, state);
}

uint8_t
itf_io_get_value (h_itf_io_t h_itf_io)
{
    const itf_io_config_t * config = &itf_io_config[h_itf_io];
    GPIO_PinState           state;
    uint8_t                 value;

    state = HAL_GPIO_ReadPin(config->port, config->pin);

    if (GPIO_PIN_RESET == state)
    {
        value = ITF_IO_LOW;
    }
    else
    {
        value = ITF_IO_HIGH;
    }

    return value;
}

void
itf_io_toggle_value (h_itf_io_t h_itf_io)
{
    const itf_io_config_t * config = &itf_io_config[h_itf_io];

    HAL_GPIO_TogglePin(config->port, config->pin);
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

void
HAL_GPIO_EXTI_Callback (uint16_t pin_id)
{
    for (unsigned int i = 0u; i < H_ITF_IO_INT_COUNT; i++)
    {
        if (pin_id == itf_io_config[i].pin)
        {
            itf_io_int_cb_t cb = itf_io_int_cb[i];

            if (cb != NULL)
            {
                cb();
            }

            break;
        }
    }
}

/** @} */

/******************************** End of file *********************************/
