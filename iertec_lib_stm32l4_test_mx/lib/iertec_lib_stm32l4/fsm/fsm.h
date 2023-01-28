/*******************************************************************************
 * @file fsm.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 02 Nov 2022
 * @brief Finite State Machine framework.
 * @ingroup fsm
 ******************************************************************************/

/**
 * @defgroup fsm fsm
 * @brief Finite State Machine framework.
 * @{
 */

#ifndef FSM_H
#define FSM_H

#include "fsm_def.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef FSM_STATE_LEVEL_MAX
/** Maximum state hierarchy level. */
#define FSM_STATE_LEVEL_MAX (0)
#endif // FSM_STATE_LEVEL_MAX

#ifndef FSM_EVENT_TICK
/** Tick event is mandatory. */
#define FSM_EVENT_TICK      (1u)
#endif // FSM_EVENT_TICK

// Forward declaration of the state machine type.
typedef struct fsm_t fsm_t;

// Forward declaration of the state type.
typedef struct fsm_state_t fsm_state_t;

/**
 * @brief Type of the function used by a state handler.
 *
 * @param[in] p_fsm Pointer to the finite state machine instance.
 *
 * @retval true Event has been processed.
 * @retval false Event has not been processed.
 */
typedef bool (* fsm_handler_state_t)(fsm_t * const p_fsm);

/**
 * @brief Type of the function used by a state handler.
 *
 * @param[in] p_fsm Pointer to the finite state machine instance.
 */
typedef void (* fsm_handler_action_t)(fsm_t * const p_fsm);

/**
 * @brief Type of the function used to fire events.
 *
 * @param[in] events Events to be fired.
 */
typedef void (* fsm_handler_event_t)(uint32_t events);

/** @brief State type. */
struct fsm_state_t
{
    /** Pointer to the activity entry handler. */
    fsm_handler_action_t handler_entry;

    /** Pointer to the state handler. */
    fsm_handler_state_t handler_state;

    /** Pointer to the activity exit handler. */
    fsm_handler_action_t handler_exit;

    /** Pointer to a possible parent state. */
    const fsm_state_t * const p_parent;

    /** State hierarchy level. */
    uint8_t level;

#ifndef NDEBUG
    /** Name of the state for debugging purposes. */
    const char * const name;
#endif // NDEBUG
};

/** @brief Finite state machine type. */
struct fsm_t
{
    /** Pointer to the current state. */
    const fsm_state_t * p_state;

    /** Function used to fire events from handlers. */
    const fsm_handler_event_t handler_event;

    /** Current events to be processed.*/
    uint32_t events;

    /** Number of ticks passed in the current state. */
    uint32_t tick_count;

#ifndef NDEBUG
    /** Name of the state for debugging purposes. */
    const char * const name;

    /** Flag used to enable the debugging of the state machine instance. */
    bool b_debug_enable;
#endif // NDEBUG
};

/**
 * @brief Initialize the state machine to the indicated state and execute its
 * entry handler if it is defined.
 *
 * @param[in] p_fsm Pointer to the state machine instance.
 * @param[in] p_state_init Initial state.
 */
void fsm_init(fsm_t * const p_fsm, const fsm_state_t * p_state_init);

/**
 * @brief Process the current state of the state machine.
 *
 * @param[in] p_fsm Pointer to the state machine instance.
 * @param[in] events Events to be processed in the current state.
 *
 * @retval true Event has been processed.
 * @retval false Event has not been processed.
 */
bool fsm_process(fsm_t * const p_fsm, uint32_t events);

/**
 * @brief Switch to a state without traversing hierarchical states.
 *
 * @param[in] p_fsm Pointer to the state machine instance.
 * @param[in] p_state_target Target state.
 */
void fsm_switch(fsm_t * const p_fsm, const fsm_state_t * p_state_target);

/**
 * @brief Switch to a state traversing hierarchical states.
 *
 * @param[in] p_fsm Pointer to the state machine instance.
 * @param[in] p_state_target Target state.
 */
void fsm_traverse(fsm_t * const p_fsm, const fsm_state_t * p_state_target);

#endif // FSM_H

/** @} */

/******************************** End of file *********************************/
