/*******************************************************************************
 * @file test_state.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 18 Aug 2023
 * @brief Test state machine states.
 * @ingroup test_state
 ******************************************************************************/

/**
 * @addtogroup test_state
 * @{
 */

#include "test_state.h"
#include "sys_util.h"
#include "debug_util.h"
#include "itf_wdgt.h"
#include "itf_rtc.h"
#include "itf_io.h"
#include "itf_uart.h"
#include "itf_debug.h"
#include "itf_spi.h"
#include "itf_i2c.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Utility to convert from milliseconds to ticks. */
#define TEST_STATE_MSEC_TO_TICKS(X)  ((X) / TEST_TICK_MSEC)

/** Elapsed time of 1 second. */
#define TEST_STATE_TIME_1_SEC        (TEST_STATE_MSEC_TO_TICKS(1000))

/** Elapsed time of 2 seconds. */
#define TEST_STATE_TIME_2_SEC        (TEST_STATE_MSEC_TO_TICKS(2000))

/** Elapsed time of 4 seconds. */
#define TEST_STATE_TIME_4_SEC        (TEST_STATE_MSEC_TO_TICKS(4000))

/****************************************************************************//*
 * Private code prototypes (state machine handlers)
 ******************************************************************************/

static bool test_state_init(fsm_t * const p_fsm);
static bool test_state_idle(fsm_t * const p_fsm);
static bool test_state_printf(fsm_t * const p_fsm);
static bool test_state_assert(fsm_t * const p_fsm);
static bool test_state_reset(fsm_t * const p_fsm);
static bool test_state_wdgt(fsm_t * const p_fsm);
static bool test_state_rtc(fsm_t * const p_fsm);
static bool test_state_io(fsm_t * const p_fsm);
static bool test_state_io_int(fsm_t * const p_fsm);
static bool test_state_uart(fsm_t * const p_fsm);
static bool test_state_debug_uart(fsm_t * const p_fsm);
static bool test_state_spi(fsm_t * const p_fsm);
static bool test_state_i2c(fsm_t * const p_fsm);
static bool test_state_power(fsm_t * const p_fsm);
static bool test_state_time(fsm_t * const p_fsm);

/****************************************************************************//*
 * Private data
 ******************************************************************************/


/** Test states array. */
const fsm_state_t test_state[TEST_STATE_COUNT] =
{
    {
        .handler_entry = NULL,
        .handler_state = test_state_init,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_INIT",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_idle,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_IDLE",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_printf,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_PRINTF",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_assert,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_ASSERT",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_reset,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_RESET",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_wdgt,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_WDGT",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_rtc,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_RTC",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_io,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_IO",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_io_int,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_IO_INT",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_uart,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_UART",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_debug_uart,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_DEBUG_UART",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_spi,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_SPI",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_i2c,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_I2C",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_power,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_POWER",
#endif // NDEBUG
    },
    {
        .handler_entry = NULL,
        .handler_state = test_state_time,
        .handler_exit  = NULL,
        .p_parent      = NULL,
        .level         = TEST_STATE_LEVEL,
#ifndef NDEBUG
        .name          = "TEST_STATE_TIME",
#endif // NDEBUG
    },
};

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief RTC callback.
 */
static void rtc_cb(void);

/**
 * @brief Button ISR.
 */
static void button_isr(void);

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static bool
test_state_init (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        uint32_t     reset_code;
        const char * reset_str;

        sys_get_reset_source(&reset_code, &reset_str);
        debug_printf("Reset, code: %u, desc: %s\r\n", reset_code, reset_str);

        switch (TEST_EXE)
        {
            default:
                // Do nothing
            break;

            case TEST_STATE_RTC:
                itf_rtc_set_callback(rtc_cb);
            break;

            case TEST_STATE_DEBUG_UART:
                // Connect PA15 to 3.3 V to detect the debug interface
                if (itf_debug_is_connected())
                {
                  itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
                }
                else
                {
                  itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_LOW);
                }
            break;
        }

        fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);

        return true;
    }

    return false;
}

static bool
test_state_idle (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
//        vApplicationPrintRtosInfo();
        itf_wdgt_feed();
        fsm_switch(p_fsm, &test_state[TEST_EXE]);

        return true;
    }

    return false;
}

static bool
test_state_printf (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        if (TEST_STATE_TIME_1_SEC <= p_fsm->tick_count)
        {
            int32_t  var_i32   = -2147483647;
            uint32_t var_u32   = 4294967295;
            uint32_t var_hex   = 0x89ABCDEF;
            float    var_f     = 1.234567f;
            char     var_str[] =
                "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

            debug_printf("Test printf: %ld, %lu, %08lx, %.6f, %s\r\n", var_i32,
                         var_u32, var_hex, var_f, var_str);

            fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);
        }

        return true;
    }

    return false;
}

static bool
test_state_assert (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        DEBUG_ASSERT(1 == 0);

        return true;
    }

    return false;
}

static bool
test_state_reset (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        if (TEST_STATE_TIME_1_SEC <= p_fsm->tick_count)
        {
            sys_reset();
        }

        return true;
    }

    return false;
}

static bool
test_state_wdgt (fsm_t * const p_fsm)
{
    return true;
}

static bool
test_state_rtc (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);

        return true;
    }

    return true;
}

static bool
test_state_io (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        if (itf_io_get_value(H_ITF_IO_BUTTON_1) == ITF_IO_LOW)
        {
            itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
        }
        else
        {
            itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_LOW);
        }

        return true;
    }

    return true;
}

static bool
test_state_io_int (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        if (1 == p_fsm->tick_count)
        {
            itf_io_set_int_cb(H_ITF_IO_BUTTON_1, button_isr);
        }
        else if (TEST_STATE_TIME_2_SEC == p_fsm->tick_count)
        {
            itf_io_set_int_cb(H_ITF_IO_BUTTON_1, NULL);
        }
        else if (TEST_STATE_TIME_4_SEC <= p_fsm->tick_count)
        {
            fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);
        }

        return true;
    }

    return true;
}

static bool
test_state_uart (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        char data[32];
        bool ret;
        size_t len;

        ret = itf_uart_write(H_ITF_UART_DEBUG, "Hola\r\n");

        DEBUG_ASSERT(ret);

        len = itf_uart_read(H_ITF_UART_DEBUG, data, 32);
        ret = itf_uart_write_bin(H_ITF_UART_DEBUG, data, len);

        DEBUG_ASSERT(ret);

        fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);

        return true;
    }

    return true;
}

static bool
test_state_debug_uart (fsm_t * const p_fsm)
{
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        int d_char = debug_get_char();

        if (d_char < 0)
        {
            debug_printf("%d\r\n", d_char);
        }
        else
        {
            debug_printf("%c\r\n", d_char);
        }

        if (TEST_STATE_TIME_4_SEC <= p_fsm->tick_count)
        {
            fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);
        }

        return true;
    }

    return true;
}

static bool
test_state_spi (fsm_t * const p_fsm)
{
    // Connect PA7 to PA6
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        if (TEST_STATE_TIME_1_SEC <= p_fsm->tick_count)
        {
            uint8_t data_tx[100];
            uint8_t data_rx[100] = {0};

            for (size_t i = 0; i < 100; i++)
            {
                data_tx[i] = i;
                data_rx[i] = 0;
            }

            bool ret = itf_spi_transaction(H_ITF_SPI_0, data_tx, data_rx, 100);

            DEBUG_ASSERT(ret);

            for (size_t i = 0; i < 100; i++)
            {
                DEBUG_ASSERT(data_tx[i] == data_rx[i]);
            }

            fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);
        }

        return true;
    }

    return false;
}

static bool
test_state_i2c (fsm_t * const p_fsm)
{
    // Connect PB8 to  SCL
    // Connect PB9 to SDA
    if (p_fsm->events & TEST_EVENT_TICK)
    {
        if (TEST_STATE_TIME_1_SEC <= p_fsm->tick_count)
        {
            uint8_t data_tx[7] = {0x04, 30, 30, 12, 15, 6, 25};
            uint8_t data_rx[6] = {0};
            bool ret = itf_i2c_transaction(H_ITF_I2C_0, 0x51, data_tx, 7, NULL, 0);

            DEBUG_ASSERT(ret);

            ret = itf_i2c_transaction(H_ITF_I2C_0, 0x51, data_tx, 1, data_rx, 6);

            DEBUG_ASSERT(ret);

            for (size_t i = 1; i < 7; i++)
            {
                DEBUG_ASSERT(data_tx[i] == data_rx[i - 1]);
            }

            fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);
        }

        return true;
    }

    return false;
}

static bool
test_state_power (fsm_t * const p_fsm)
{
    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
    sys_delay_usec(500000);
    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_LOW);
    sys_sleep_msec(2000);

    fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);

    return true;
}

static bool
test_state_time (fsm_t * const p_fsm)
{
    // Test time
    uint32_t time_1 = sys_get_timestamp();
    sys_sleep_msec(1000);
    uint32_t time_2 = sys_get_timestamp();
    uint32_t time_diff = time_2 - time_1;

    debug_printf("Time sys_sleep_msec %u\r\n", time_diff);

    time_1 = sys_get_timestamp();
    HAL_Delay(1000);
    time_2 = sys_get_timestamp();
    time_diff = time_2 - time_1;

    debug_printf("Time HAL_Delay %u\r\n", time_diff);

    time_1 = sys_get_timestamp();
    sys_delay_usec(1000000);
    time_2 = sys_get_timestamp();
    time_diff = time_2 - time_1;

    debug_printf("Time sys_delay_usec %u\r\n", time_diff);

    fsm_switch(p_fsm, &test_state[TEST_STATE_IDLE]);

    return true;
}

static void
rtc_cb (void)
{
    itf_io_toggle_value(H_ITF_IO_LED_GREEN);
}

static void
button_isr (void)
{
    itf_io_toggle_value(H_ITF_IO_LED_GREEN);
}

/** @} */

/******************************** End of file *********************************/
