/*******************************************************************************
 * @file debug_util.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 26 Oct 2022
 * @brief Debug utilities test double.
 ******************************************************************************/

#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H

#include "unity.h"
#include "CException.h"

#include <stdbool.h>

#define debug_info(X,...)   UnityPrintF(__LINE__, (X), ##__VA_ARGS__)

#define debug_printf(X,...) do {} while (0)

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

#endif // DEBUG_UTIL_H

/******************************** End of file *********************************/
