/*******************************************************************************
 * @file test_fsm.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 05 Nov 2022
 * @brief Unit tests for the finite state machine framework.
 ******************************************************************************/

#include "fsm.h"

#include "unity.h"

/****************************************************************************//*
 * Dependencies
 ******************************************************************************/

const char test_file_name[] = __FILE_NAME__;

// System dependencies
TEST_FILE("system_stm32l4xx.c")
TEST_FILE("stm32l4xx_it.c")
TEST_FILE("sysmem.c")

// HAL dependencies
TEST_FILE("stm32l4xx_hal.c")
TEST_FILE("stm32l4xx_hal_cortex.c")
TEST_FILE("stm32l4xx_hal_pwr_ex.c")
TEST_FILE("stm32l4xx_hal_pwr.c")
TEST_FILE("stm32l4xx_hal_rcc_ex.c")
TEST_FILE("stm32l4xx_hal_rcc.c")
TEST_FILE("stm32l4xx_hal_tim_ex.c")
TEST_FILE("stm32l4xx_hal_tim.c")
TEST_FILE("stm32l4xx_hal_timebase_tim.c")
TEST_FILE("main.c")

// Test support dependencies
TEST_FILE("test_main.c")

#include "mock_fsm_to_mock.h"

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

static void stop_ignore_handlers(void);

/****************************************************************************//*
 * Private data
 ******************************************************************************/

// Simple state machine
//              +------+------+
//              |             |
//              |    root     |
//              |             |
//              +----+-+------+
//                   | |
//            +------+ +-------+
//            |                |
//     +------+------+  +------+------+
//     |             |  |             |
//     |      0      |  |      1      |
//     |             |  |             |
//     +-------------+  +-------------+

static fsm_t simple_fsm =
{
    .name = "SIMPLE",
};

static const fsm_state_t simple_state[] =
{
    {
        .handler_entry = entry_handler_0,
        .handler_state = state_handler_0,
        .handler_exit  = exit_handler_0,
        .p_parent      = NULL,
        .level         = 0,
        .name          = "SIMPLE_STATE_0",
    },
    {
        .handler_entry = entry_handler_1,
        .handler_state = state_handler_1,
        .handler_exit  = exit_handler_1,
        .p_parent      = NULL,
        .level         = 0,
        .name          = "SIMPLE_STATE_0",
    },
};

// Hierarchical state machine
//     +------+------+
//     |             |
//     |    root     |
//     |             |
//     +------+------+
//            |
//     +------+------+
//     |             |
//     |      0      |
//     |             |
//     +------+------+
//            |
//     +------+------+
//     |             |
//     |      1      |
//     |             |
//     +------+------+
//            |
//     +------+------+
//     |             |
//     |      2      |
//     |             |
//     +-------------+

static fsm_t hierarchical_fsm =
{
    .name = "HIERARCHICAL",
};

static const fsm_state_t hierarchical_state_0[] =
{
    {
        .handler_entry = entry_handler_0,
        .handler_state = state_handler_0,
        .handler_exit  = exit_handler_0,
        .p_parent      = NULL,
        .level         = 0,
        .name          = "HIERARCHICAL_STATE_0_0",
    },
};

static const fsm_state_t hierarchical_state_1[] =
{
    {
        .handler_entry = entry_handler_1,
        .handler_state = state_handler_1,
        .handler_exit  = exit_handler_1,
        .p_parent      = &hierarchical_state_0[0],
        .level         = 1,
        .name          = "HIERARCHICAL_STATE_1_0",
    },
};

static const fsm_state_t hierarchical_state_2[] =
{
    {
        .handler_entry = entry_handler_2,
        .handler_state = state_handler_2,
        .handler_exit  = exit_handler_2,
        .p_parent      = &hierarchical_state_1[0],
        .level         = 2,
        .name          = "HIERARCHICAL_STATE_2_0",
    },
};

// Complex state machine
//                                   +--------------+
//                                   |              |
//                                   |     root     |
//                                   |              |
//                                   +-+----+-----+-+
//                                     |    |     |
//                                     |    |     +-------------------------+
//                                     |    |                               |
//                     +---------------+    +------------+                  |
//                     |                                 |                  |
//              +------+------+                   +------+------+    +------+------+
//              |             |                   |             |    |             |
//              |     0_0     |                   |     0_1     |    |     0_2     |
//              |             |                   |             |    |             |
//              +----+-+------+                   +---+----+----+    +-------------+
//                   | |                              |    |
//          +--------+ +-------+                   +--+    +--------+
//          |                  |                   |                |
//     +----+--------+  +------+------+     +------+------+  +------+------+
//     |             |  |             |     |             |  |             |
//     |    1_0_0    |  |    1_0_1    |     |    1_1_0    |  |    1_1_1    |
//     |             |  |             |     |             |  |             |
//     +-------------+  +---+---+-----+     +------+------+  +------+------+
//                          |   |                  |                |
//                          |   |                  |                |
//                   +------+   +---+              |                |
//                   |              |              |                |
//             +-----+-----+  +-----+-----+  +-----+-----+    +-----+-----+
//             |   2_0_0   |  |   2_0_1   |  |   2_1_0   |    |   2_2_0   |
//             +-----------+  +-----------+  +-----------+    +-----------+

static fsm_t complex_fsm =
{
    .name = "COMPLEX",
};

static const fsm_state_t complex_state_0[] =
{
    {
        .handler_entry = entry_handler_0,
        .handler_state = state_handler_0,
        .handler_exit  = exit_handler_0,
        .p_parent      = NULL,
        .level         = 0,
        .name          = "COMPLEX_STATE_0_0",
    },
    {
        .handler_entry = entry_handler_1,
        .handler_state = state_handler_1,
        .handler_exit  = exit_handler_1,
        .p_parent      = NULL,
        .level         = 0,
        .name          = "COMPLEX_STATE_0_1",
    },
    {
        .handler_entry = entry_handler_2,
        .handler_state = state_handler_2,
        .handler_exit  = exit_handler_2,
        .p_parent      = NULL,
        .level         = 0,
        .name          = "COMPLEX_STATE_0_2",
    },
};

static const fsm_state_t complex_state_1_0[] =
{
    {
        .handler_entry = entry_handler_1_0_0,
        .handler_state = state_handler_1_0_0,
        .handler_exit  = exit_handler_1_0_0,
        .p_parent      = &complex_state_0[0],
        .level         = 1,
        .name          = "COMPLEX_STATE_1_0_0",
    },
    {
        .handler_entry = entry_handler_1_0_1,
        .handler_state = state_handler_1_0_1,
        .handler_exit  = exit_handler_1_0_1,
        .p_parent      = &complex_state_0[0],
        .level         = 1,
        .name          = "COMPLEX_STATE_1_0_1",
    },
};

static const fsm_state_t complex_state_1_1[] =
{
    {
        .handler_entry = entry_handler_1_1_0,
        .handler_state = state_handler_1_1_0,
        .handler_exit  = exit_handler_1_1_0,
        .p_parent      = &complex_state_0[1],
        .level         = 1,
        .name          = "COMPLEX_STATE_1_1_0",
    },
    {
        .handler_entry = entry_handler_1_1_1,
        .handler_state = state_handler_1_1_1,
        .handler_exit  = exit_handler_1_1_1,
        .p_parent      = &complex_state_0[1],
        .level         = 1,
        .name          = "COMPLEX_STATE_1_1_1",
    },
};

static const fsm_state_t complex_state_2_0[] =
{
    {
        .handler_entry = entry_handler_2_0_0,
        .handler_state = state_handler_2_0_0,
        .handler_exit  = exit_handler_2_0_0,
        .p_parent      = &complex_state_1_0[1],
        .level         = 2,
        .name          = "COMPLEX_STATE_2_0_0",
    },
    {
        .handler_entry = entry_handler_2_0_1,
        .handler_state = state_handler_2_0_1,
        .handler_exit  = exit_handler_2_0_1,
        .p_parent      = &complex_state_1_0[1],
        .level         = 2,
        .name          = "COMPLEX_STATE_2_0_1",
    },
};

static const fsm_state_t complex_state_2_1[] =
{
    {
        .handler_entry = entry_handler_2_1_0,
        .handler_state = state_handler_2_1_0,
        .handler_exit  = exit_handler_2_1_0,
        .p_parent      = &complex_state_1_1[0],
        .level         = 2,
        .name          = "COMPLEX_STATE_2_1_0",
    },
};

static const fsm_state_t complex_state_2_2[] =
{
    {
        .handler_entry = entry_handler_2_2_0,
        .handler_state = state_handler_2_2_0,
        .handler_exit  = exit_handler_2_2_0,
        .p_parent      = &complex_state_1_1[1],
        .level         = 2,
        .name          = "COMPLEX_STATE_2_2_0",
    },
};

/****************************************************************************//*
 * Tests
 ******************************************************************************/

void test_fsm_simple_init(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &simple_state[0],
        .events = 0,
        .tick_count = 0,
        .name = "SIMPLE",
    };

    stop_ignore_handlers();
    entry_handler_0_Expect(&exp_fsm);

    fsm_init(&simple_fsm, &simple_state[0]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &simple_fsm, sizeof(fsm_t));
}

void test_fsm_simple_event_handled(void)
{
    const uint32_t exp_events = 0b101;
    const fsm_t exp_fsm =
    {
        .p_state = &simple_state[0],
        .events = 0b101,
        .tick_count = 1, // Increased by tick event
        .name = "SIMPLE",
    };

    stop_ignore_handlers();
    state_handler_0_ExpectAndReturn(&exp_fsm, true);

    TEST_ASSERT_TRUE(fsm_process(&simple_fsm, exp_events));
    TEST_ASSERT_EQUAL(0, simple_fsm.events);
}

void test_fsm_simple_event_not_handled(void)
{
    const uint32_t exp_events = 0b1001;
    const fsm_t exp_fsm =
    {
        .p_state = &simple_state[0],
        .events = 0b1001,
        .tick_count = 2, // Increased by tick event
        .name = "SIMPLE",
    };

    stop_ignore_handlers();
    state_handler_0_ExpectAndReturn(&exp_fsm, false);

    TEST_ASSERT_FALSE(fsm_process(&simple_fsm, exp_events));
    TEST_ASSERT_EQUAL(0, simple_fsm.events);
}

void test_fsm_simple_transition_switch(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &simple_state[1],
        .events = 0,
        .tick_count = 0,
        .name = "SIMPLE",
    };

    stop_ignore_handlers();
    exit_handler_0_Expect(&exp_fsm);
    entry_handler_1_Expect(&exp_fsm);

    fsm_switch(&simple_fsm, &simple_state[1]);
    TEST_ASSERT_EQUAL_PTR(&simple_state[1], simple_fsm.p_state);
}

void test_fsm_simple_transition_traverse(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &simple_state[0],
        .events = 0,
        .tick_count = 0,
        .name = "SIMPLE",
    };

    simple_fsm.tick_count = 1;

    stop_ignore_handlers();
    exit_handler_1_Expect(&exp_fsm);
    entry_handler_0_Expect(&exp_fsm);

    fsm_traverse(&simple_fsm, &simple_state[0]);
    TEST_ASSERT_EQUAL_PTR(&simple_state[0], simple_fsm.p_state);
}

void test_fsm_hierarchical_init(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &hierarchical_state_2[0],
        .events = 0,
        .tick_count = 0,
        .name = "HIERARCHICAL",
    };

    stop_ignore_handlers();
    entry_handler_0_Expect(&exp_fsm);
    entry_handler_1_Expect(&exp_fsm);
    entry_handler_2_Expect(&exp_fsm);

    fsm_init(&hierarchical_fsm, &hierarchical_state_2[0]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &hierarchical_fsm, sizeof(fsm_t));
}

void test_fsm_hierarchical_handled_by_0(void)
{
    const uint32_t exp_events = 0b101;
    const fsm_t exp_fsm =
    {
        .p_state = &hierarchical_state_2[0],
        .events = 0b101,
        .tick_count = 1, // Increased by tick event
        .name = "HIERARCHICAL",
    };

    stop_ignore_handlers();
    state_handler_0_ExpectAndReturn(&exp_fsm, true);

    TEST_ASSERT_TRUE(fsm_process(&hierarchical_fsm, exp_events));
    TEST_ASSERT_EQUAL(0, simple_fsm.events);
}

void test_fsm_hierarchical_handled_by_1(void)
{
    const uint32_t exp_events = 0b1001;
    const fsm_t exp_fsm =
    {
        .p_state = &hierarchical_state_2[0],
        .events = 0b1001,
        .tick_count = 2, // Increased by tick event
        .name = "HIERARCHICAL",
    };

    stop_ignore_handlers();
    state_handler_0_ExpectAndReturn(&exp_fsm, false);
    state_handler_1_ExpectAndReturn(&exp_fsm, true);

    TEST_ASSERT_TRUE(fsm_process(&hierarchical_fsm, exp_events));
    TEST_ASSERT_EQUAL(0, simple_fsm.events);
}

void test_fsm_hierarchical_handled_by_2(void)
{
    const uint32_t exp_events = 0b1000;
    const fsm_t exp_fsm =
    {
        .p_state = &hierarchical_state_2[0],
        .events = 0b1000,
        .tick_count = 2,
        .name = "HIERARCHICAL",
    };

    stop_ignore_handlers();
    state_handler_0_ExpectAndReturn(&exp_fsm, false);
    state_handler_1_ExpectAndReturn(&exp_fsm, false);
    state_handler_2_ExpectAndReturn(&exp_fsm, true);

    TEST_ASSERT_TRUE(fsm_process(&hierarchical_fsm, exp_events));
    TEST_ASSERT_EQUAL(0, simple_fsm.events);
}

void test_fsm_hierarchical_not_handled(void)
{
    const uint32_t exp_events = 0b1100;
    const fsm_t exp_fsm =
    {
        .p_state = &hierarchical_state_2[0],
        .events = 0b1100,
        .tick_count = 2,
        .name = "HIERARCHICAL",
    };

    stop_ignore_handlers();
    state_handler_0_ExpectAndReturn(&exp_fsm, false);
    state_handler_1_ExpectAndReturn(&exp_fsm, false);
    state_handler_2_ExpectAndReturn(&exp_fsm, false);

    TEST_ASSERT_FALSE(fsm_process(&hierarchical_fsm, exp_events));
    TEST_ASSERT_EQUAL(0, simple_fsm.events);
}

void test_fsm_complex_init(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_2_0[0],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    stop_ignore_handlers();
    entry_handler_0_Expect(&exp_fsm);
    entry_handler_1_0_1_Expect(&exp_fsm);
    entry_handler_2_0_0_Expect(&exp_fsm);

    fsm_init(&complex_fsm, &complex_state_2_0[0]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

void test_fsm_complex_traverse_2_0_0_to_2_0_1(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_2_0[1],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    stop_ignore_handlers();
    exit_handler_2_0_0_Expect(&exp_fsm);
    entry_handler_2_0_1_Expect(&exp_fsm);

    fsm_traverse(&complex_fsm, &complex_state_2_0[1]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

void test_fsm_complex_traverse_2_0_1_to_2_2_0(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_2_2[0],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    stop_ignore_handlers();
    exit_handler_2_0_1_Expect(&exp_fsm);
    exit_handler_1_0_1_Expect(&exp_fsm);
    exit_handler_0_Expect(&exp_fsm);
    entry_handler_1_Expect(&exp_fsm);
    entry_handler_1_1_1_Expect(&exp_fsm);
    entry_handler_2_2_0_Expect(&exp_fsm);

    fsm_traverse(&complex_fsm, &complex_state_2_2[0]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

void test_fsm_complex_traverse_2_2_0_to_2_1_0(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_2_1[0],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    stop_ignore_handlers();
    exit_handler_2_2_0_Expect(&exp_fsm);
    exit_handler_1_1_1_Expect(&exp_fsm);
    entry_handler_1_1_0_Expect(&exp_fsm);
    entry_handler_2_1_0_Expect(&exp_fsm);

    fsm_traverse(&complex_fsm, &complex_state_2_1[0]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

void test_fsm_complex_traverse_2_1_0_to_0_2(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_0[2],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    stop_ignore_handlers();
    exit_handler_2_1_0_Expect(&exp_fsm);
    exit_handler_1_1_0_Expect(&exp_fsm);
    exit_handler_1_Expect(&exp_fsm);
    entry_handler_2_Expect(&exp_fsm);

    fsm_traverse(&complex_fsm, &complex_state_0[2]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

void test_fsm_complex_traverse_0_2_to_2_0_1(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_2_0[1],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    stop_ignore_handlers();
    exit_handler_2_Expect(&exp_fsm);
    entry_handler_0_Expect(&exp_fsm);
    entry_handler_1_0_1_Expect(&exp_fsm);
    entry_handler_2_0_1_Expect(&exp_fsm);

    fsm_traverse(&complex_fsm, &complex_state_2_0[1]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

void test_fsm_complex_traverse_2_0_1_to_1_0_0(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_1_0[0],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    stop_ignore_handlers();
    exit_handler_2_0_1_Expect(&exp_fsm);
    exit_handler_1_0_1_Expect(&exp_fsm);
    entry_handler_1_0_0_Expect(&exp_fsm);

    fsm_traverse(&complex_fsm, &complex_state_1_0[0]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

void test_fsm_complex_traverse_1_0_0_to_2_0_0(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_2_0[0],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    stop_ignore_handlers();
    exit_handler_1_0_0_Expect(&exp_fsm);
    entry_handler_1_0_1_Expect(&exp_fsm);
    entry_handler_2_0_0_Expect(&exp_fsm);

    fsm_traverse(&complex_fsm, &complex_state_2_0[0]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

void test_fsm_complex_traverse_1_0_0_to_2_2_0(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_2_2[0],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    // Restore state to 1_0_0
    complex_fsm.p_state = &complex_state_1_0[0];

    stop_ignore_handlers();
    exit_handler_1_0_0_Expect(&exp_fsm);
    exit_handler_0_Expect(&exp_fsm);
    entry_handler_1_Expect(&exp_fsm);
    entry_handler_1_1_1_Expect(&exp_fsm);
    entry_handler_2_2_0_Expect(&exp_fsm);

    fsm_traverse(&complex_fsm, &complex_state_2_2[0]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

void test_fsm_complex_traverse_2_2_0_to_1_0_0(void)
{
    const fsm_t exp_fsm =
    {
        .p_state = &complex_state_1_0[0],
        .events = 0,
        .tick_count = 0,
        .name = "COMPLEX",
    };

    stop_ignore_handlers();
    exit_handler_2_2_0_Expect(&exp_fsm);
    exit_handler_1_1_1_Expect(&exp_fsm);
    exit_handler_1_Expect(&exp_fsm);
    entry_handler_0_Expect(&exp_fsm);
    entry_handler_1_0_0_Expect(&exp_fsm);

    fsm_traverse(&complex_fsm, &complex_state_1_0[0]);
    TEST_ASSERT_EQUAL_MEMORY(&exp_fsm, &complex_fsm, sizeof(fsm_t));
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void stop_ignore_handlers(void)
{
    entry_handler_0_StopIgnore();
    state_handler_0_StopIgnore();
    exit_handler_0_StopIgnore();
    entry_handler_1_StopIgnore();
    state_handler_1_StopIgnore();
    exit_handler_1_StopIgnore();
    entry_handler_2_StopIgnore();
    state_handler_2_StopIgnore();
    exit_handler_2_StopIgnore();
    entry_handler_1_0_0_StopIgnore();
    state_handler_1_0_0_StopIgnore();
    exit_handler_1_0_0_StopIgnore();
    entry_handler_1_0_1_StopIgnore();
    state_handler_1_0_1_StopIgnore();
    exit_handler_1_0_1_StopIgnore();
    entry_handler_1_1_0_StopIgnore();
    state_handler_1_1_0_StopIgnore();
    exit_handler_1_1_0_StopIgnore();
    entry_handler_1_1_1_StopIgnore();
    state_handler_1_1_1_StopIgnore();
    exit_handler_1_1_1_StopIgnore();
    entry_handler_2_0_0_StopIgnore();
    state_handler_2_0_0_StopIgnore();
    exit_handler_2_0_0_StopIgnore();
    entry_handler_2_0_1_StopIgnore();
    state_handler_2_0_1_StopIgnore();
    exit_handler_2_0_1_StopIgnore();
    entry_handler_2_1_0_StopIgnore();
    state_handler_2_1_0_StopIgnore();
    exit_handler_2_1_0_StopIgnore();
    entry_handler_2_2_0_StopIgnore();
    state_handler_2_2_0_StopIgnore();
    exit_handler_2_2_0_StopIgnore();
}

/******************************** End of file *********************************/
