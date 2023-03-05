/*******************************************************************************
 * @file rtc.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 11 Nov 2020
 * @brief Real time clock and calendar.
 * @ingroup rtc
 ******************************************************************************/

/**
 * @defgroup rtc rtc
 * @brief Real time clock and calendar.
 * @{
 */

#ifndef RTC_H
#define RTC_H

#include "datetime.h"

#include <stdint.h>
#include <stdbool.h>

/** First system year. It must be the following year to a leap year. */
#define YEAR_BASE       (2001)

/** System 0 timestamp (1/1/2001 00:00:00) in epoch timestamp (since 1/1/1970
 * 00:00:00). */
#define EPOCH_BASE_TIME ((uint32_t)978307200)

/**
 * @brief Get the system timestamp that consists in the number of seconds from
 * 00:00:00 of 1st January of YEAR_BASE.
 *
 * @param[out] timestamp Current timestamp value.
 * @param[out] centiseconds Centiseconds of the current timestamp.
 */
void rtc_get_timestamp(uint32_t * timestamp, uint8_t * centiseconds);

/**
 * @brief Get the epoch timestamp that consists in the number of seconds from
 * 00:00:00 of 1st January of 1970.
 *
 * @param[out] timestamp Current timestamp value.
 * @param[out] centiseconds Centiseconds of the current timestamp.
 */
void rtc_get_epoch_timestamp(uint32_t * timestamp, uint8_t * centiseconds);

/**
 * @brief Set the system timestamp that consists in the number of seconds from
 * 00:00:00 of 1st January of YEAR_BASE.
 *
 * @param[in] timestamp Timestamp value to set.
 */
void rtc_set_timestamp(uint32_t timestamp);

/**
 * @brief Set the current date/time of the system.
 *
 * @param[in] datetime Pointer to date/time to set.
 * @param[in] fmt_local true: input format is local time plus time zone;
 *                      false: input format is UTC time plus time zone.
 *
 * @retval true If system date/time is changed.
 * @retval false If the passed date/time is incorrect.
 */
bool rtc_set_datetime(const datetime_t * datetime, bool fmt_local);

/**
 * @brief Get the current date/time of the system.
 *
 * @param[out] datetime Pointer where to store the current date/time.
 */
void rtc_get_datetime(datetime_t * datetime);

/**
 * @brief Compare 2 dates.
 *
 * @param[in] datetime_1 First date to compare.
 * @param[in] datetime_2 Second date to compare.
 *
 * @retval 1 If datetime_1 is greater than datetime_2.
 * @retval 0 If datetime_1 is equal to datetime_2.
 * @retval -1 If datetime_1 is smaller than datetime_2.
 */
int rtc_compare_datetime(const datetime_t * datetime_1,
                         const datetime_t * datetime_2);

#endif // RTC_H

/** @} */

/******************************** End of file *********************************/
