/*******************************************************************************
 * @file fsm.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 02 Nov 2022
 * @brief Finite State Machine framework.
 * @ingroup fsm
 ******************************************************************************/

/**
 * @addtogroup fsm
 * @{
 */

#include "fsm.h"
#include "debug_util.h"

/****************************************************************************//*
 * Public code
 ******************************************************************************/

void
fsm_init (fsm_t * const p_fsm, const fsm_state_t * p_state_init)
{
    DEBUG_ASSERT(NULL != p_fsm);
    DEBUG_ASSERT(p_state_init != NULL);

    const fsm_state_t * p_state_path[FSM_STATE_LEVEL_MAX + 1];
    size_t              index = 0;

    p_fsm->p_state    = p_state_init;
    p_fsm->events     = 0;
    p_fsm->tick_count = 0;

    // Loop until we reach top level
    do
    {
        p_state_path[index++] = p_state_init;
        p_state_init          = p_state_init->p_parent;
    } while (NULL != p_state_init);

    // Check for array out of bounds
    DEBUG_ASSERT(index <= FSM_STATE_LEVEL_MAX + 1);

    // Process states from parent to child until one state detects the event
    do
    {
        if (NULL != p_state_path[--index]->handler_entry)
        {
            p_state_path[index]->handler_entry(p_fsm);
        }
    } while (index > 0);
}

bool
fsm_process (fsm_t * const p_fsm, uint32_t events)
{
    DEBUG_ASSERT(NULL != p_fsm);
    DEBUG_ASSERT(NULL != p_fsm->p_state);
    DEBUG_ASSERT(0 != events);

    bool ret;

    // Fill new events
    p_fsm->events = events;

    // Check if a new tick has been generated
    if (FSM_EVENT_TICK & events)
    {
        p_fsm->tick_count++;
    }

#ifndef NDEBUG
    if (p_fsm->b_debug_enable)
    {
        debug_printf("FSM %s, state %s, events %u, tick %u\r\n", p_fsm->name,
                     p_fsm->p_state->name, p_fsm->events, p_fsm->tick_count);
    }
#endif // NDEBUG

    // If current state is top level
    if (0 == p_fsm->p_state->level)
    {
        DEBUG_ASSERT(NULL != p_fsm->p_state->handler_state);

        ret = p_fsm->p_state->handler_state(p_fsm);
    }
    else
    {
        const fsm_state_t * p_state_path[FSM_STATE_LEVEL_MAX + 1];
        const fsm_state_t * p_state = p_fsm->p_state;
        size_t              index   = 0;

        // Loop until we reach top level
        do
        {
            DEBUG_ASSERT(NULL != p_state->handler_state);

            p_state_path[index++] = p_state;
            p_state               = p_state->p_parent;
        } while (NULL != p_state);

        // Check for array out of bounds
        DEBUG_ASSERT(index <= FSM_STATE_LEVEL_MAX + 1);

        // Process states from parent to child until one state detects the event
        do
        {
            ret = p_state_path[--index]->handler_state(p_fsm);
        } while (!ret && (index > 0));
    }

    // Clear events
    p_fsm->events = 0;

    return ret;
}

void
fsm_switch (fsm_t * const p_fsm, const fsm_state_t * p_state_target)
{
    DEBUG_ASSERT(NULL != p_fsm);
    DEBUG_ASSERT(NULL != p_state_target);

    const fsm_state_t * p_state_source = p_fsm->p_state;

    p_fsm->p_state    = p_state_target;
    p_fsm->tick_count = 0;

    if (NULL != p_state_source->handler_exit)
    {
#ifndef NDEBUG
        if (p_fsm->b_debug_enable)
        {
            debug_printf("FSM %s, exit %s\r\n", p_fsm->name,
                         p_state_source->name);
        }
#endif // NDEBUG

        p_state_source->handler_exit(p_fsm);
    }

    if (NULL != p_state_target->handler_entry)
    {
#ifndef NDEBUG
        if (p_fsm->b_debug_enable)
        {
            debug_printf("FSM %s, entry %s\r\n", p_fsm->name,
                         p_state_target->name);
        }
#endif // NDEBUG

        p_state_target->handler_entry(p_fsm);
    }
}

void
fsm_traverse (fsm_t * const p_fsm, const fsm_state_t * p_state_target)
{
    DEBUG_ASSERT(NULL != p_fsm);
    DEBUG_ASSERT(NULL != p_state_target);

    const fsm_state_t * p_state_source = p_fsm->p_state;
    const fsm_state_t * p_state_path[FSM_STATE_LEVEL_MAX + 1];
    size_t              index          = 0;

    p_fsm->p_state    = p_state_target;
    p_fsm->tick_count = 0;

    // Loop until we reach a common parent
    do
    {
        DEBUG_ASSERT(NULL != p_state_target);

        // Loop until surpass target level
        while ((NULL != p_state_source)
               && (p_state_source->level >= p_state_target->level))
        {
            if (NULL != p_state_source->handler_exit)
            {
#ifndef NDEBUG
                if (p_fsm->b_debug_enable)
                {
                    debug_printf("FSM %s, exit %s\r\n", p_fsm->name,
                                 p_state_source->name);
                }
#endif // NDEBUG

                p_state_source->handler_exit(p_fsm);
            }

            p_state_source = p_state_source->p_parent;
        }

        p_state_path[index++] = p_state_target;
        p_state_target        = p_state_target->p_parent;
    } while (p_state_source != p_state_target);

    do
    {
        if (NULL != p_state_path[--index]->handler_entry)
        {
#ifndef NDEBUG
            if (p_fsm->b_debug_enable)
            {
                debug_printf("FSM %s, entry %s\r\n", p_fsm->name,
                             p_state_path[index]->name);
            }
#endif // NDEBUG

            p_state_path[index]->handler_entry(p_fsm);
        }
    } while (index > 0);
}

/** @} */

/******************************** End of file *********************************/
