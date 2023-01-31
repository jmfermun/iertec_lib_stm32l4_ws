/*******************************************************************************
 * @file debug_util.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 24 Jan 2023
 * @brief Debug utilities.
 * @ingroup debug_util
 ******************************************************************************/

/**
 * @defgroup debug_util debug_util
 * @brief Debug utilities.
 * @{
 */

#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H

#include "unity.h"
#include "CException.h"

#include <stdbool.h>

/**
 * @brief See @ref debug_printf_impl.
 */
#define debug_info(X,...) UnityPrintF(__LINE__, (X), ##__VA_ARGS__)

/** Lock the application in this point. */
#define DEBUG_ERROR_TRAP()                                                   \
    do                                                                       \
    {                                                                        \
        TEST_PRINTF("Error trap, file %s, line %d\r\n", __FILE__, __LINE__); \
        Throw(0);                                                            \
    } while (0)

/** Assert macro. If it fails, prints the file and line and locks the
 * application at this point. */
#define DEBUG_ASSERT(x)                                                  \
    do                                                                   \
    {                                                                    \
        if ((x) == 0)                                                    \
        {                                                                \
            TEST_PRINTF("Assert failed, file %s, line %d\r\n", __FILE__, \
                         __LINE__);                                      \
            Throw(0);                                                    \
        }                                                                \
    } while (0)

/**
 * @brief Same as @ref debug_info, but only is executed in debug mode.
 */
#define debug_printf(X,...) do {} while (0)

/**
 * @brief Performs a compile time assert. Code obtained from
 * https://www.pixelbeat.org/programming/gcc/static_assert.html.
 *
 * @param e Condition to check, for example "sizeof(int) == 4".
 */
#define DEBUG_ASSERT_STATIC DEBUG_ASSERT

#endif // DEBUG_UTIL_H

/** @} */

/******************************** End of file *********************************/
