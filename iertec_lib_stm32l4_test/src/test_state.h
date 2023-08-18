/*******************************************************************************
 * @file test_state.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 18 Aug 2023
 * @brief Test state machine states.
 * @ingroup test_state
 ******************************************************************************/

/**
 * @defgroup test_state test_state
 * @brief Test state machine states.
 * @{
 */

#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "fsm.h"

/** Tests execution control. */
#define TEST_EXE TEST_STATE_TIME

/** @brief List of states of the TEST state machine. */
typedef enum
{
    TEST_STATE_INIT = 0,
    TEST_STATE_IDLE,
    TEST_STATE_PRINTF,
    TEST_STATE_ASSERT,
    TEST_STATE_RESET,
    TEST_STATE_WDGT,
    TEST_STATE_RTC,
    TEST_STATE_IO,
    TEST_STATE_IO_INT,
    TEST_STATE_UART,
    TEST_STATE_DEBUG_UART,
    TEST_STATE_SPI,
    TEST_STATE_I2C,
    TEST_STATE_POWER,
    TEST_STATE_TIME,
    TEST_STATE_COUNT,
} test_state_t;

/** TEST states array. */
extern const fsm_state_t test_state[TEST_STATE_COUNT];

#endif // TEST_STATE_H

/** @} */

/******************************** End of file *********************************/
