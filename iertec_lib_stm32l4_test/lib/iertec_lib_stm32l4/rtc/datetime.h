/*******************************************************************************
 * @file datetime.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 13 Sep 2021
 * @brief Date and time data structure.
 * @ingroup datetime
 ******************************************************************************/

/**
 * @defgroup datetime datetime
 * @brief Date and time data structure.
 * @{
 */

#ifndef DATETIME_H
#define DATETIME_H

#include <stdint.h>

/** Date/time type */
typedef struct
{
    uint8_t year;       /**< 2 digit year. Range [1, 100], i.e. [2001, 2100]. */
    uint8_t month;      /**< Month. Range [1, 12]. */
    uint8_t day;        /**< Day. Range [1, 31]. */
    uint8_t hour;       /**< 24 h format hour. Range [0, 23]. */
    uint8_t minutes;    /**< Minutes. Range [0, 59]. */
    uint8_t seconds;    /**< Seconds. Range [0, 59]. */
    uint8_t cseconds;   /**< Centiseconds. Range [0, 99]. */
    int8_t  delta_hour; /**< Local time deviation in hours from UTC. Range [-24,
                         * 24]. */
} datetime_t;

#endif // DATETIME_H

/** @} */

/******************************** End of file *********************************/
