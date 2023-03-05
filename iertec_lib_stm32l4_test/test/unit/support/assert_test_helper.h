/*******************************************************************************
 * @file assert_test_helper.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 08 Dec 2022
 * @brief Test assert helpers. References:
 *     - http://www.electronvector.com/blog/unit-testing-with-asserts
 ******************************************************************************/

#ifndef ASSERT_TEST_HELPER_H
#define ASSERT_TEST_HELPER_H

#include "CException.h"

#define TEST_ASSERT_PASS_ASSERT(X)                                    \
    {                                                                 \
        CEXCEPTION_T e;                                               \
        Try                                                           \
        {                                                             \
            X;                                                        \
        }                                                             \
        Catch(e)                                                      \
        {                                                             \
            TEST_FAIL_MESSAGE("Code under test failed an assertion"); \
        }                                                             \
    }

#define TEST_ASSERT_FAIL_ASSERT(X)                               \
    {                                                            \
        CEXCEPTION_T e;                                          \
        Try                                                      \
        {                                                        \
            X;                                                   \
            TEST_FAIL_MESSAGE("Code under test did not assert"); \
        }                                                        \
        Catch(e)                                                 \
        {                                                        \
        }                                                        \
    }

#endif // ASSERT_TEST_HELPER_H

/******************************** End of file *********************************/

