/*******************************************************************************
 * @file fsm_def.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 02 Nov 2022
 * @brief Finite State Machine definitions.
 * @ingroup fsm_def
 ******************************************************************************/

/**
 * @defgroup fsm_def fsm_def
 * @brief Finite State Machine definitions.
 * @{
 */

#ifndef FSM_DEF_H
#define FSM_DEF_H

// Maximum state hierarchy level
#define FSM_STATE_LEVEL_MAX (1)

// General events
#define FSM_EVENT_TICK      (1u)
#define FSM_EVENT_ERROR     (1u << 1)

#endif // FSM_DEF_H

/** @} */

/******************************** End of file *********************************/
