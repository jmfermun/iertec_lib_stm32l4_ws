/*******************************************************************************
 * @file stm32l4xx_it.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 16 Jan 2023
 * @brief Interrupt Service Routines.
 ******************************************************************************/

#ifdef TEST_TARGET

#include "stm32l4xx_hal.h"

/*******************************************************************************
 * External global data
 ******************************************************************************/

extern TIM_HandleTypeDef htim6;

/*******************************************************************************
 * Public code
 ******************************************************************************/

/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{

}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{

}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{

}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void)
{

}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{

}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{

}

/**
 * @brief This function handles TIM6 global interrupt, DAC channel1 underrun
 * error interrupt.
 */
void TIM6_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim6);
}

#endif // TEST_TARGET
