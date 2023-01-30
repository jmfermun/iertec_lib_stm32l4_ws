/*******************************************************************************
 * @file debug_util.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 30 Jan 2023
 * @brief Debug utilities test double.
 ******************************************************************************/

#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H

#include "unity.h"
#include "CException.h"

#include <stdbool.h>

#define debug_info(X,...)   UnityPrintF(__LINE__, (X), ##__VA_ARGS__)

#define DEBUG_ERROR_TRAP()  TEST_FAIL()

#define DEBUG_ASSERT_STATIC DEBUG_ASSERT

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

void debug_printf(const char *format, ...);

bool debug_init(void);

bool debug_deinit(void);

bool debug_is_attached(void);

void debug_write(const char *data, size_t len);

void debug_put_char(char ch);

int debug_get_char(void);

bool debug_ready(void);

#endif // DEBUG_UTIL_H

/******************************** End of file *********************************/
