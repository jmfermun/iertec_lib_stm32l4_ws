/*******************************************************************************
 * @file lptimTickConfig.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 22 Dec 2022
 * @brief Configuration of the LPTIM used as tick source by FreeRTOS.
 ******************************************************************************/

#ifndef LPTIMTICKCONFIG_H
#define LPTIMTICKCONFIG_H

#if configUSE_TICKLESS_IDLE == 2

// Preprocessor code in lptimTick.c requires that configTICK_RATE_HZ be a preprocessor-friendly numeric
// literal.  As a result, the application *ignores* the CubeMX configuration of the FreeRTOS tick rate.
#undef  configTICK_RATE_HZ
#define configTICK_RATE_HZ  1000UL  // <-- Set FreeRTOS tick rate here, not in CubeMX.

// Don't bother installing xPortSysTickHandler() into the vector table or including it in the firmware
// image at all.  FreeRTOS doesn't use the SysTick timer nor xPortSysTickHandler() when lptimTick.c is
// providing the OS tick.
#undef  xPortSysTickHandler

// Without pre- and post-sleep processing, lptimTick.c uses only basic sleep mode during tickless idle.
// To utilize the stop modes and their dramatic reduction in power consumption, we employ an ultra-low-power
// driver to handle the pre- and post-sleep hooks.
#define configPRE_SLEEP_PROCESSING(X)  HAL_SuspendTick()
#define configPOST_SLEEP_PROCESSING(X) HAL_ResumeTick()

// Make sure the self-tests in our demo application capture tick-timing information as quickly as
// possible after each tick.  This interrupt priority ensures the OS tick ISR preempts other ISRs.  Without
// this configuration, interrupt latency causes a little extra jitter during the stress test.  See main.c.
// To see the effect of interrupt latency in the terminal output, remove or comment out this line.
// #define configTICK_INTERRUPT_PRIORITY   (configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1)

// The Nucleo board gives us Vdd=3.3V, so we need only 2us minimum run time to work around erratum
// 2.3.21.  Note that when we define this symbol, code in ulp.c owns the SysTick timer.  At the moment, our
// demo application doesn't have any relevant ISRs short enough to violate this minimum run time, so we
// don't enable the work around.  (The shortest possible run time between deep sleeps occurs with a very
// short ISR that interrupts STOP mode but doesn't interact with FreeRTOS at all. See lptimTick.c.)
#define configMIN_RUN_BETWEEN_DEEP_SLEEPS  ( ( 2U * configCPU_CLOCK_HZ ) / 1000000 )

#endif /* configUSE_TICKLESS_IDLE == 2 */

#endif // LPTIMTICKCONFIG_H

/******************************** End of file *********************************/
