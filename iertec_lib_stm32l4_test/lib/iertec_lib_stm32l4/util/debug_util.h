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

#include "sys_util.h"

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief See @ref debug_printf_impl.
 */
#define debug_info(X, ...) debug_printf_impl((X), ##__VA_ARGS__)

#ifndef NDEBUG

/** Lock the application in this point. */
#define DEBUG_ERROR_TRAP()               \
        do                               \
        {                                \
            __asm volatile ("BKPT #0");  \
            __asm volatile ("CPSID IF"); \
            for (;;);                    \
        } while (0)

/** Assert macro. If it fails, prints the file and line and locks the
 * application at this point. */
#define DEBUG_ASSERT(x)                                                       \
        do                                                                    \
        {                                                                     \
            if ((x) == 0)                                                     \
            {                                                                 \
                __asm volatile ("BKPT #0");                                   \
                debug_printf("Assert failed, file %s, line %d\r\n", __FILE__, \
                             __LINE__);                                       \
                __asm volatile ("CPSID IF");                                  \
                for (;;);                                                     \
            }                                                                 \
        } while (0)

/**
 * @brief Same as @ref debug_info, but only is executed in debug mode.
 */
#define debug_printf(X, ...)       debug_printf_impl((X), ##__VA_ARGS__)

#else

#define DEBUG_ERROR_TRAP()         sys_reset()

#define DEBUG_ASSERT(x)            do {} while (0)

#define debug_printf(X, ...)       do {} while (0)

#endif // NDEBUG

// Macros used by DEBUG_ASSERT_STATIC
#define DEBUG_ASSERT_CONCAT_(a, b) a##b
#define DEBUG_ASSERT_CONCAT(a, b)  DEBUG_ASSERT_CONCAT_(a, b)

// *INDENT-OFF*

/**
 * @brief Performs a compile time assert. Code obtained from
 * https://www.pixelbeat.org/programming/gcc/static_assert.html.
 *
 * @param[in] e Condition to check, for example "sizeof(int) == 4".
 */
#define DEBUG_ASSERT_STATIC(e) \
        ;enum { DEBUG_ASSERT_CONCAT(assert_line_, __LINE__) = 1/(int)(!!(e)) }

// *INDENT-ON*

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
 * @param[in] format Format string with the same meaning as in the standard
 * printf function.
 * @param[in] ... Additional arguments with the same meaning as in the standard
 * printf function.
 */
void debug_printf_impl(const char * format, ...);

/**
 * @brief Send binary data through the debug interface.
 *
 * @param[in] data Binary data to be sent.
 * @param[in] len Number of bytes to send.
 */
void debug_write(const char * data, size_t len);

/**
 * @brief Send a char through the debug interface.
 *
 * @param[in] ch Character to be sent.
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
