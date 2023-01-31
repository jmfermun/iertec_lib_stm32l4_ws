/*******************************************************************************
 * @file version.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 31 Jan 2023
 * @brief Device firmware version.
 * @ingroup version
 ******************************************************************************/

/**
 * @defgroup version version
 * @brief Device firmware version.
 * @{
 */

#ifndef VERSION_H
#define VERSION_H

/** Current firmware version major number. */
#define VERSION_MAJOR 1

/** Current firmware version minor number. */
#define VERSION_MINOR 0

/** Current firmware version patch number. */
#define VERSION_PATCH 0

/** Utility to convert the passed parameter to a string. */
#define STR_HELPER(x) #x

/** Utility to convert the content of the indicated define to a string. */
#define STR(x)        STR_HELPER(x)

#endif // VERSION_H

/** @} */

/**
 * @page changelog Changelog
 *
 * Notes:
 *  - Each version entry indicates the date with the format YYYY/MM/DD.
 *  - Versions are ordered from most recent to older.
 *
 * Changes:
 *  - 2022/01/31 version 1.0.0:
 *      - Initial version.
 */

/******************************** End of file *********************************/
