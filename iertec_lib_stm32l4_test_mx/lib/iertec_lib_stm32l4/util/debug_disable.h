/*******************************************************************************
 * @file debug_disable.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 19 Oct 2022
 * @brief Debug utility to disable debug_printf in the current module.
 * @ingroup debug_util
 ******************************************************************************/

/**
 * @defgroup debug_util debug_util
 * @brief Debug utility to disable debug_printf in the current module.
 * @{
 */

#ifdef debug_printf
#undef debug_printf
#endif // debug_printf

#define debug_printf(X, ...) do {} while (0)

/** @} */

/******************************** End of file *********************************/
