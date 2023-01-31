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
#define debug_printf(X, ...) debug_printf_impl((X), ##__VA_ARGS__)

/**
 * @brief Performs a compile time assert. Code obtained from
 * https://www.pixelbeat.org/programming/gcc/static_assert.html.
 *
 * @param e Condition to check, for example "sizeof(int) == 4".
 */
#define DEBUG_ASSERT_STATIC DEBUG_ASSERT

/**
 * @brief Initialize the debug resources.
 *
 * @retval true If the debug resources are initialized correctly.
 * @retval false Otherwise.
 */
bool debug_init(void);

/**
 * @brief Deinitialize the debug resources.
 *
 * @retval true If the debug resources are deinitialized correctly.
 * @retval false Otherwise.
 */
bool debug_deinit(void);

/**
 * @brief Check if the interface was attached to an external device in the
 * initialization phase.
 *
 * @return true if the interface is connected, false otherwise.
 */
bool debug_is_attached(void);

/**
 * @brief Writes the C string pointed by format through the debug interface. The
 * function is thread safe.
 *
 * @param format Format string with the same meaning as in the standard printf
 * function.
 * @param ... Additional arguments with the same meaning as in the standard
 * printf function.
 */
void debug_printf_impl(const char * format, ...);

/**
 * @brief Send binary data through the debug interface.
 *
 * @param data Binary data to be sent.
 * @param len Number of bytes to send.
 */
void debug_write(const char * data, size_t len);

/**
 * @brief Send a char through the debug interface.
 *
 * @param ch Character to be sent.
 */
void debug_put_char(char ch);

/**
 * @brief Read a character from the debug interface.
 *
 * @return Character read.
 * @retval -1 An error occurs.
 * @retval -2 No character available.
 */
int debug_get_char(void);

/**
 * @brief Check if the debug interface is operative.
 *
 * @return true if it is initialized, false otherwise.
 */
bool debug_is_ready(void);

#endif // DEBUG_UTIL_H

/** @} */

/******************************** End of file *********************************/
