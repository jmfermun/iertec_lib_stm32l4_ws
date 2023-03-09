/*******************************************************************************
 * @file itf_pwr.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 23 Jan 2022
 * @brief Power control system interface.
 * @ingroup itf_pwr
 ******************************************************************************/

/**
 * @addtogroup itf_pwr
 * @{
 */

#include "itf_pwr.h"

#include "FreeRTOS.h"
#include "task.h"

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Maximum number of peripherals supported by the power control system. */
#define H_ITF_PWR_MAX (32)

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** Current number of peripherals registered. */
static uint8_t h_itf_pwr_index;

/** Mask with the currently activated peripherals. */
static volatile uint32_t itf_pwr_active_flag;

/** Masks with the registered peripherals in each power level. */
static uint32_t itf_pwr_level_mask[ITF_PWR_LEVEL_COUNT];

/** Backup value of the RCC->CR register stored while in stop mode. */
static uint32_t itf_pwr_rcc_cr_save;

/** Backup value of the RCC->CFGR register stored while in stop mode. */
static uint32_t itf_pwr_rcc_cfgr_save;

#ifdef configMIN_RUN_BETWEEN_DEEP_SLEEPS

/** Minimum run time peripheral used to avoid erratum 2.3.21. */
static uint8_t h_itf_pwr_min_run;

#endif // configMIN_RUN_BETWEEN_DEEP_SLEEPS

/****************************************************************************//*
 * Public code
 ******************************************************************************/

bool
itf_pwr_init (void)
{
    h_itf_pwr_index     = 0;
    itf_pwr_active_flag = 0;

    // The level masks are negated to optimize the comparison
    for (size_t i = 0; i < ITF_PWR_LEVEL_COUNT; i++)
    {
        itf_pwr_level_mask[i] = ~0u;
    }

#ifdef configMIN_RUN_BETWEEN_DEEP_SLEEPS
    h_itf_pwr_min_run = itf_pwr_register(ITF_PWR_LEVEL_0);

    if (H_ITF_PWR_NONE == h_itf_pwr_min_run)
    {
        return false;
    }

    // Set the SysTick "load" register (or reload-value register) to support the
    // errata workaround.
    // The SysTick timer uses a 24-bit counting register. The longest minimum
    // run time is 15us according to the errata sheet. A 24-bit value (up to
    // 16.7M) is more than sufficient for 15us, no matter the core clock rate.
    configASSERT(configMIN_RUN_BETWEEN_DEEP_SLEEPS <= 0x00FFFFFFU
                 && configMIN_RUN_BETWEEN_DEEP_SLEEPS != 0);
    SysTick->LOAD = configMIN_RUN_BETWEEN_DEEP_SLEEPS;

    // Be sure SysTick uses the lowest interrupt priority.
    NVIC_SetPriority(SysTick_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY);
#endif

    // Be sure the MCU wakes up from stop mode on the same clock we normally use
    // as the core clock, if possible. Might as well give the MCU a head start
    // getting the clock going while waking from STOP.
    if ((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_HSI)
    {
        SET_BIT(RCC->CFGR, RCC_CFGR_STOPWUCK);
    }
    else if ((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_MSI)
    {
        CLEAR_BIT(RCC->CFGR, RCC_CFGR_STOPWUCK);
    }

    return true;
}

uint8_t
itf_pwr_register (itf_pwr_level_t level)
{
    uint8_t handle;

    if ((h_itf_pwr_index >= H_ITF_PWR_MAX) || (level >= ITF_PWR_LEVEL_COUNT))
    {
        handle = H_ITF_PWR_NONE;
    }
    else
    {
        handle                     = h_itf_pwr_index++;
        itf_pwr_level_mask[level] &= ~(1u << handle);
    }

    return handle;
}

void
itf_pwr_set_active (uint8_t h_itf_pwr)
{
    taskENTER_CRITICAL();
    itf_pwr_active_flag |= 1u << h_itf_pwr;
    taskEXIT_CRITICAL();
}

void
itf_pwr_set_inactive (uint8_t h_itf_pwr)
{
    taskENTER_CRITICAL();
    itf_pwr_active_flag &= ~(1u << h_itf_pwr);
    taskEXIT_CRITICAL();
}

void
itf_pwr_pre_sleep (void)
{
    itf_pwr_level_t level;

    if ((itf_pwr_active_flag & itf_pwr_level_mask[ITF_PWR_LEVEL_2]) == 0)
    {
        level = ITF_PWR_LEVEL_2;
        MODIFY_REG(PWR->CR1, PWR_CR1_LPMS, PWR_CR1_LPMS_STOP2);
    }
    else if ((itf_pwr_active_flag & itf_pwr_level_mask[ITF_PWR_LEVEL_1]) == 0)
    {
        level = ITF_PWR_LEVEL_1;
        MODIFY_REG(PWR->CR1, PWR_CR1_LPMS, PWR_CR1_LPMS_STOP1);
    }
    else
    {
        level = ITF_PWR_LEVEL_0;
    }

    HAL_SuspendTick();

    if (ITF_PWR_LEVEL_0 != level)
    {
        itf_pwr_rcc_cr_save   = RCC->CR;
        itf_pwr_rcc_cfgr_save = RCC->CFGR;

        SCB->SCR             |= SCB_SCR_SLEEPDEEP_Msk;
    }
    else
    {
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    }
}

void
itf_pwr_post_sleep (void)
{
    if (SCB->SCR & SCB_SCR_SLEEPDEEP_Msk)
    {
#ifdef configMIN_RUN_BETWEEN_DEEP_SLEEPS
        // Start a new SysTick timer period. We won't attempt to enter STOP mode
        // until the timer period ends. Note that we do start a new period here
        // unnecessarily if the CPU didn't actually enter stop mode (due to a
        // pending interrupt). That's OK.
        SysTick->VAL  = 0;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk
                        | SysTick_CTRL_TICKINT_Msk;

        // Mark the "min-run" peripheral as being now in use so that we won't
        // attempt STOP mode until it's no longer in use. See SysTick_Handler()
        // below.
        itf_pwr_active_flag |= 1u << h_itf_pwr_min_run;
#endif // configMIN_RUN_BETWEEN_DEEP_SLEEPS

        // We may have been in deep sleep. If we were, the hardware cleared
        // several enable bits in the CR, and it changed the selected system
        // clock in CFGR. Restore them now. If we're restarting the PLL as the
        // CPU clock here, the CPU will not wait for it. Instead, the CPU
        // continues executing from the wake-up clock (MSI in our case) until
        // the PLL is stable and then the CPU starts using the PLL.
        RCC->CR   = itf_pwr_rcc_cr_save;
        RCC->CFGR = itf_pwr_rcc_cfgr_save;

        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

        // This application bypasses the RTC shadow registers, so we don't need
        // to clear the sync flag for those registers. They are always out of
        // sync when coming out of deep sleep.
        // RTC->ISR &= ~RTC_ISR_RSF;
    }

    HAL_ResumeTick();
}

#ifdef configMIN_RUN_BETWEEN_DEEP_SLEEPS

// When the SysTick timer expires, we allow STOP mode again.
void
SysTick_Handler (void)
{
    // Stop the SysTick timer. We use it in "one-shot" mode to know when it's
    // safe to use STOP mode again. Then mark our "min-run" peripheral as no
    // longer in use.
    SysTick->CTRL = 0;

    UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

    itf_pwr_active_flag &= ~(1u << h_itf_pwr_min_run);
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

#endif // configMIN_RUN_BETWEEN_DEEP_SLEEPS

/** @} */

/******************************** End of file *********************************/
