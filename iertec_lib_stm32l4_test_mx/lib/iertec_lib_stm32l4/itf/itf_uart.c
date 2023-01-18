/*******************************************************************************
 * @file itf_uart.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 22 Dec 2022
 * @brief UART interface driver.
 * @ingroup itf_uart
 ******************************************************************************/

/**
 * @addtogroup itf_uart
 * @{
 */

#include "itf_uart.h"
#include "itf_io.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"

#include <string.h>

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** UART reception buffer size. */
#define ITF_UART_BUFFER_RX_SIZE (32)

/** UART reception buffer threshold to disable RTS */
#define ITF_UART_RTS_OFF_THR    (8)

/** UART reception buffer threshold to enable RTS */
#define ITF_UART_RTS_ON_THR     (24)

/****************************************************************************//*
 * Type definitions
 ******************************************************************************/

/** RTS and CTS signals possible states */
typedef enum
{
    ITF_UART_XTS_STATE_OFF = 0,
    ITF_UART_XTS_STATE_ON,
    ITF_UART_XTS_STATE_NOT_USED,
} itf_uart_xts_state;

/** @brief UART instance data needed during transactions. */
typedef struct
{
    UART_HandleTypeDef * handle;
    uint32_t             timeout_ticks;
    SemaphoreHandle_t    sem_tx;
    uint8_t            * buffer_tx;
    size_t               len_tx;
    StreamBufferHandle_t buffer_rx;
    size_t               len_rx;
    h_itf_io_t           pin_rts;
    itf_uart_xts_state   rts_state;
} itf_uart_instance_t;

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Board configuration
extern const itf_uart_config_t itf_uart_config[H_ITF_UART_COUNT];

/** Instances of the available UART interfaces. */
static itf_uart_instance_t itf_uart_instance[H_ITF_UART_COUNT];

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_uart_init (h_itf_uart_t h_itf_uart)
{
    if (h_itf_uart >= H_ITF_UART_COUNT)
    {
        return false;
    }

    const itf_uart_config_t * config   = &itf_uart_config[h_itf_uart];
    itf_uart_instance_t *     instance = &itf_uart_instance[h_itf_uart];

    // Low level initialization
    if (NULL != config->init_ll)
    {
        config->init_ll();
    }

    if (itf_bsp_get_error())
    {
        return false;
    }

    // Save the UART instance to be used
    instance->handle = config->handle;
    instance->buffer_tx = NULL;
    instance->len_tx = 0;
    instance->len_rx = 0;

    // Create the transmission semaphore
    instance->sem_tx = xSemaphoreCreateBinary();

    if (NULL == instance->sem_tx)
    {
        return false;
    }

    // Create the reception FIFO buffer
    instance->buffer_rx = xStreamBufferCreate(ITF_UART_BUFFER_RX_SIZE,
                                              sizeof(uint8_t));

    if (NULL == instance->buffer_rx)
    {
        return false;
    }

    // Configure the reception timeout
    if (0 == config->timeout_msec)
    {
        instance->timeout_ticks = portMAX_DELAY;
    }
    else
    {
        instance->timeout_ticks = pdMS_TO_TICKS(config->timeout_msec);
    }

    // Configure hardware flow control. CTS is handled by the hardware
    // peripheral and RTS by the code logic
    instance->pin_rts = config->pin_rts;

    if (config->pin_rts != H_ITF_IO_NONE)
    {
        // Set RTS. The UART reception is disabled at the start
        instance->rts_state = ITF_UART_XTS_STATE_ON;
        itf_io_set_value(instance->pin_rts, ITF_IO_HIGH);
    }
    else
    {
        instance->rts_state = ITF_UART_XTS_STATE_NOT_USED;
    }

    return true;
}

bool
itf_uart_deinit (h_itf_uart_t h_itf_uart)
{
    if (h_itf_uart >= H_ITF_UART_COUNT)
    {
        return false;
    }

    itf_uart_instance_t * instance = &itf_uart_instance[h_itf_uart];

    if (NULL != instance->handle)
    {
        itf_uart_read_disable(h_itf_uart);
    }

    if (HAL_UART_DeInit(instance->handle) != HAL_OK)
    {
        return false;
    }

    instance->handle = NULL;

    return true;
}

bool
itf_uart_write (h_itf_uart_t h_itf_uart, const char * data)
{
    bool   ret;
    size_t bytes_to_write = strlen(data);

#ifdef ITF_UART_PRINTF
    debug_printf("TX >> %s", data);
#endif // ITF_UART_PRINTF

    ret = itf_uart_write_bin(h_itf_uart, data, bytes_to_write);

    return ret;
}

bool
itf_uart_write_bin (h_itf_uart_t h_itf_uart, const char * data, size_t len)
{
    if (len > 0)
    {
        itf_uart_instance_t * instance = &itf_uart_instance[h_itf_uart];

        taskENTER_CRITICAL();

        instance->buffer_tx = (uint8_t*)data;
        instance->len_tx = len;

        // Enable the transmit data register empty interrupt
        ATOMIC_SET_BIT(instance->handle->Instance->CR1, USART_CR1_TXEIE);

        taskEXIT_CRITICAL();

        // Block until transmission completes
        xSemaphoreTake(instance->sem_tx, portMAX_DELAY);
    }

    return true;
}

void
itf_uart_read_enable (h_itf_uart_t h_itf_uart)
{
    itf_uart_instance_t *instance = &itf_uart_instance[h_itf_uart];

    // Clean the FIFO buffer
    for (size_t i = 0; i < ITF_UART_BUFFER_RX_SIZE; i++)
    {
        size_t len;
        uint8_t data;

        len = xStreamBufferReceive(instance->buffer_rx, &data, sizeof(data), 0);

        taskENTER_CRITICAL();
        instance->len_rx -= len;
        taskEXIT_CRITICAL();
    }

    taskENTER_CRITICAL();

    // Computation of UART mask to apply to RDR register.
    UART_MASK_COMPUTATION(instance->handle);

    instance->handle->ErrorCode = HAL_UART_ERROR_NONE;

    // Enable the UART error interrupt: frame error, noise error, overrun error
    ATOMIC_SET_BIT(instance->handle->Instance->CR3, USART_CR3_EIE);

    // Enable the UART parity error interrupt and data register not empty
    // interrupt
    if (instance->handle->Init.Parity != UART_PARITY_NONE)
    {
        ATOMIC_SET_BIT(instance->handle->Instance->CR1,
                       USART_CR1_PEIE | USART_CR1_RXNEIE);
    }
    else
    {
        ATOMIC_SET_BIT(instance->handle->Instance->CR1, USART_CR1_RXNEIE);
    }

    // Clear RTS
    if (instance->rts_state != ITF_UART_XTS_STATE_NOT_USED)
    {
        instance->rts_state = ITF_UART_XTS_STATE_OFF;
        itf_io_set_value(instance->pin_rts, ITF_IO_LOW);
    }

    taskEXIT_CRITICAL();
}

void
itf_uart_read_disable (h_itf_uart_t h_itf_uart)
{
    itf_uart_instance_t *instance = &itf_uart_instance[h_itf_uart];

    taskENTER_CRITICAL();

    // Disable the UART parity error and RXNE interrupts
    ATOMIC_CLEAR_BIT(instance->handle->Instance->CR1,
                     (USART_CR1_RXNEIE | USART_CR1_PEIE));

    // Disable the UART error interrupt: frame error, noise error, overrun error
    ATOMIC_CLEAR_BIT(instance->handle->Instance->CR3, USART_CR3_EIE);

    // Set RTS
    if (instance->rts_state != ITF_UART_XTS_STATE_NOT_USED)
    {
        instance->rts_state = ITF_UART_XTS_STATE_ON;
        itf_io_set_value(instance->pin_rts, ITF_IO_HIGH);
    }

    taskEXIT_CRITICAL();
}

size_t
itf_uart_read (h_itf_uart_t h_itf_uart, char *data, size_t max_len)
{
    itf_uart_instance_t *instance = &itf_uart_instance[h_itf_uart];
    char read_byte = 0;
    size_t i = 0;

    // Read characters until '\n' is received
    do
    {
        size_t s_len = xStreamBufferReceive(instance->buffer_rx, &read_byte,
                                            sizeof(read_byte),
                                            instance->timeout_ticks);

        if (s_len == sizeof(read_byte))
        {
             taskENTER_CRITICAL();

             instance->len_rx--;

             // Check to clear RTS
             if ((instance->rts_state == ITF_UART_XTS_STATE_ON) &&
                 (instance->len_rx < ITF_UART_RTS_OFF_THR))
             {
                 instance->rts_state = ITF_UART_XTS_STATE_OFF;
                 itf_io_set_value(instance->pin_rts, ITF_IO_LOW);
             }

             taskEXIT_CRITICAL();

             data[i++] = read_byte;

             // Special case for the start of
             if (i == 2 && data[0] == '>' && data[1] == ' ')
             {
                 break;
             }
        }
        else
        {
            i = 0;
            break;
        }
    } while ((read_byte != '\n') && (i < max_len));

    // If the response is correct and the line is not empty, NULL terminate it
    // and increment the size by 1 to take into account the NULL character
    if ((i > 0) && (i < (max_len - 1))
        && !((i == 2) && (data[0] == '\r') && (data[1] == '\n')))
    {
        data[i++] = '\0';

#ifdef ITF_UART_PRINTF
        debug_printf("RX << %s", data);
#endif // ITF_UART_PRINTF

        return i;
    }

    // If the response is incorrect, return an empty string
    data[0] = '\0';

    return 0;
}

size_t
itf_uart_read_bin (h_itf_uart_t h_itf_uart, char *data, size_t max_len)
{
    itf_uart_instance_t *instance = &itf_uart_instance[h_itf_uart];
    char read_byte = 0;
    size_t i = 0;

    // Read bytes until the indicated number of bytes is reached or until a
    // timeout occurs
    do
    {
        size_t s_len = xStreamBufferReceive(instance->buffer_rx, &read_byte,
                                            sizeof(read_byte),
                                            instance->timeout_ticks);

        if (s_len == sizeof(read_byte))
        {
            taskENTER_CRITICAL();

            instance->len_rx--;

            // Check to clear RTS
            if ((instance->rts_state == ITF_UART_XTS_STATE_ON) &&
                (instance->len_rx < ITF_UART_RTS_OFF_THR))
            {
                instance->rts_state = ITF_UART_XTS_STATE_OFF;
                itf_io_set_value(instance->pin_rts, ITF_IO_LOW);
            }

            taskEXIT_CRITICAL();

            data[i++] = read_byte;
        }
        else
        {
            break;
        }
    } while (i < max_len);

    return i;
}

size_t
itf_uart_read_count (h_itf_uart_t h_itf_uart)
{
    itf_uart_instance_t * instance = &itf_uart_instance[h_itf_uart];
    size_t count;

    taskENTER_CRITICAL();

    count = instance->len_rx;

    taskEXIT_CRITICAL();

    return count;
}

void
itf_uart_isr (h_itf_uart_t h_itf_uart)
{
    BaseType_t            b_yield    = pdFALSE;
    itf_uart_instance_t * instance   = &itf_uart_instance[h_itf_uart];
    bool                  b_rx_error = false;

    // Read the interrupt flag and interrupt enable bits
    uint32_t isr_flags = READ_REG(instance->handle->Instance->ISR);
    uint32_t cr1_its   = READ_REG(instance->handle->Instance->CR1);
    uint32_t cr3_its   = READ_REG(instance->handle->Instance->CR3);

    // Parity error
    if ((isr_flags & USART_ISR_PE) && (cr1_its & USART_CR1_PEIE))
    {
        __HAL_UART_CLEAR_FLAG(instance->handle, UART_CLEAR_PEF);
        instance->handle->ErrorCode |= HAL_UART_ERROR_PE;
        b_rx_error = true;
    }

    // Frame error
    if ((isr_flags & USART_ISR_FE) && (cr3_its & USART_CR3_EIE))
    {
        __HAL_UART_CLEAR_FLAG(instance->handle, UART_CLEAR_FEF);
        instance->handle->ErrorCode |= HAL_UART_ERROR_FE;
        b_rx_error = true;
    }

    // Noise error
    if ((isr_flags & USART_ISR_NE) && (cr3_its & USART_CR3_EIE))
    {
        __HAL_UART_CLEAR_FLAG(instance->handle, UART_CLEAR_NEF);
        instance->handle->ErrorCode |= HAL_UART_ERROR_NE;
        b_rx_error = true;
    }

    // Over-Run error
    if ((isr_flags & USART_ISR_ORE) && (cr1_its & USART_CR1_RXNEIE)
        && (cr3_its & USART_CR3_EIE))
    {
        __HAL_UART_CLEAR_FLAG(instance->handle, UART_CLEAR_OREF);
        instance->handle->ErrorCode |= HAL_UART_ERROR_ORE;
        b_rx_error = true;
    }

    // Receiver timeout
    if ((isr_flags & USART_ISR_RTOF) && (cr1_its & USART_CR1_RTOIE))
    {
        __HAL_UART_CLEAR_FLAG(instance->handle, UART_CLEAR_RTOF);
        instance->handle->ErrorCode |= HAL_UART_ERROR_RTO;
        b_rx_error = true;
    }

    // Read data register not empty
    if ((isr_flags & USART_ISR_RXNE) && (cr1_its & USART_CR1_RXNEIE))
    {
        uint8_t data = (uint8_t)(READ_REG(instance->handle->Instance->RDR)
                                 & instance->handle->Mask);

        if (!b_rx_error)
        {
            instance->len_rx += xStreamBufferSendFromISR(instance->buffer_rx,
                                                         &data, sizeof(data),
                                                         &b_yield);
        }

        // Check to set RTS
        if ((instance->len_rx > ITF_UART_RTS_ON_THR) &&
            (instance->rts_state == ITF_UART_XTS_STATE_OFF))
        {
            instance->rts_state = ITF_UART_XTS_STATE_ON;
            itf_io_set_value(instance->pin_rts, ITF_IO_HIGH);
        }

        // Clear RXNE interrupt flag
        __HAL_UART_SEND_REQ(instance->handle, UART_RXDATA_FLUSH_REQUEST);
    }

    // Transmit data register empty
    if ((isr_flags & USART_ISR_TXE) && (cr1_its & USART_CR1_TXEIE))
    {
        uint8_t data = *instance->buffer_tx++;

        instance->handle->Instance->TDR = data;
        instance->len_tx--;

        if (instance->len_tx == 0)
        {
            instance->buffer_tx = NULL;

            // Disable the UART transmit data register empty interrupt
            ATOMIC_CLEAR_BIT(instance->handle->Instance->CR1, USART_CR1_TXEIE);

            // Enable the UART transmit complete interrupt
            ATOMIC_SET_BIT(instance->handle->Instance->CR1, USART_CR1_TCIE);
        }
    }

    // Transmission complete
    if ((isr_flags & USART_ISR_TC) && (cr1_its & USART_CR1_TCIE))
    {
        // Disable the UART Transmit Complete Interrupt
        ATOMIC_CLEAR_BIT(instance->handle->Instance->CR1, USART_CR1_TCIE);

        // Notify to task the end of the UART transaction
        xSemaphoreGiveFromISR(instance->sem_tx, &b_yield);
    }

    portYIELD_FROM_ISR(b_yield);
}

/** @} */

/******************************** End of file *********************************/
