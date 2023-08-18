/*******************************************************************************
 * @file test.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 18 Aug 2023
 * @brief Test task.
 * @ingroup test
 ******************************************************************************/

/**
 * @addtogroup test
 * @{
 */

#include "test.h"
#include "test_state.h"
#include "fsm.h"
#include "debug_util.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "timers.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Test task priority. */
#define TEST_TASK_PRIORITY    (2)

/** Test task stack size. */
#define TEST_TASK_STACK_SIZE  (1024)

/****************************************************************************//*
 * Private code prototypes (state machine handlers)
 ******************************************************************************/

/**
 * @brief Fire test events.
 *
 * @param[in] events Events to be fired.
 */
static void test_fire_event(uint32_t events);

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** Test state machine event handler. */
static EventGroupHandle_t h_test_event;

/** Clock handler used for the test state machine tick. */
static TimerHandle_t h_test_tick_clock;

/** Test state machine instance. */
static fsm_t test_fsm =
{
    .p_state        = NULL,
    .handler_event  = test_fire_event,
#ifndef NDEBUG
    .name           = "TEST",
    .b_debug_enable = false,
#endif // NDEBUG
};

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Initialize the task.
 */
static void test_task_init(void);

/**
 * @brief Task thread.
 *
 * @param[in] parameters Task arguments (ignored).
 */
static void test_task_thread(void * parameters);

/**
 * @brief Clock callback.
 *
 * @param[in] timer_id Expired timer identifier.
 */
static void test_clock_cb(TimerHandle_t timer_id);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
test_init (void)
{
    BaseType_t task_ret;

    // Construct the task
    task_ret = xTaskCreate(test_task_thread, "TEST", TEST_TASK_STACK_SIZE, NULL,
                           TEST_TASK_PRIORITY, NULL);

    if (task_ret != pdPASS)
    {
        return false;
    }

    // Construct the events handler
    h_test_event = xEventGroupCreate();

    if (!h_test_event)
    {
        return false;
    }

    // Construct the tick clock
    h_test_tick_clock = xTimerCreate("m2m_tick", pdMS_TO_TICKS(TEST_TICK_MSEC),
                                    pdTRUE, (void *)TEST_EVENT_TICK,
                                    test_clock_cb);

    if (!h_test_tick_clock)
    {
        return false;
    }

    return true;
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static void
test_task_init (void)
{
    fsm_init(&test_fsm, &test_state[TEST_STATE_INIT]);
    xTimerStart(h_test_tick_clock, portMAX_DELAY);
}

static void
test_task_thread (void * parameters)
{
    (void)parameters;

    test_task_init();

    for (;;)
    {
        EventBits_t events = xEventGroupWaitBits(h_test_event, TEST_EVENT_ALL,
                                                 pdTRUE, pdFALSE,
                                                 portMAX_DELAY);

        if (!fsm_process(&test_fsm, events))
        {
            DEBUG_ERROR_TRAP();
        }
    }
}

static void
test_clock_cb (TimerHandle_t timer_id)
{
    uint32_t events = (uint32_t)pvTimerGetTimerID(timer_id);

    (void)xEventGroupSetBits(h_test_event, (EventBits_t)events);
}

static void
test_fire_event (uint32_t events)
{
    (void)xEventGroupSetBits(h_test_event, (EventBits_t)events);
}

/** @} */

/******************************** End of file *********************************/
