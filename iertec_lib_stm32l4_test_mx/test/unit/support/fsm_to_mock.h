/*******************************************************************************
 * @file fsm_to_mock.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 02 Nov 2022
 * @brief Functions to mock for the fsm test.
 ******************************************************************************/

#ifndef FSM_TO_MOCK_H
#define FSM_TO_MOCK_H

#include "fsm.h"

void entry_handler_0(fsm_t * const p_fsm);
bool state_handler_0(fsm_t * const p_fsm);
void exit_handler_0(fsm_t * const p_fsm);

void entry_handler_1(fsm_t * const p_fsm);
bool state_handler_1(fsm_t * const p_fsm);
void exit_handler_1(fsm_t * const p_fsm);

void entry_handler_2(fsm_t * const p_fsm);
bool state_handler_2(fsm_t * const p_fsm);
void exit_handler_2(fsm_t * const p_fsm);

void entry_handler_1_0_0(fsm_t * const p_fsm);
bool state_handler_1_0_0(fsm_t * const p_fsm);
void exit_handler_1_0_0(fsm_t * const p_fsm);

void entry_handler_1_0_1(fsm_t * const p_fsm);
bool state_handler_1_0_1(fsm_t * const p_fsm);
void exit_handler_1_0_1(fsm_t * const p_fsm);

void entry_handler_1_1_0(fsm_t * const p_fsm);
bool state_handler_1_1_0(fsm_t * const p_fsm);
void exit_handler_1_1_0(fsm_t * const p_fsm);

void entry_handler_1_1_1(fsm_t * const p_fsm);
bool state_handler_1_1_1(fsm_t * const p_fsm);
void exit_handler_1_1_1(fsm_t * const p_fsm);

void entry_handler_2_0_0(fsm_t * const p_fsm);
bool state_handler_2_0_0(fsm_t * const p_fsm);
void exit_handler_2_0_0(fsm_t * const p_fsm);

void entry_handler_2_0_1(fsm_t * const p_fsm);
bool state_handler_2_0_1(fsm_t * const p_fsm);
void exit_handler_2_0_1(fsm_t * const p_fsm);

void entry_handler_2_1_0(fsm_t * const p_fsm);
bool state_handler_2_1_0(fsm_t * const p_fsm);
void exit_handler_2_1_0(fsm_t * const p_fsm);

void entry_handler_2_2_0(fsm_t * const p_fsm);
bool state_handler_2_2_0(fsm_t * const p_fsm);
void exit_handler_2_2_0(fsm_t * const p_fsm);

#endif // FSM_TO_MOCK_H

/** @} */

/******************************** End of file *********************************/
