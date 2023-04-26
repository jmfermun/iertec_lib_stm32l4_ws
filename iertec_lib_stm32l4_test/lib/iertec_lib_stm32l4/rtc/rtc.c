/*******************************************************************************
 * @file rtc.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 11 Nov 2020
 * @brief Real time clock and calendar.
 * @ingroup rtc
 ******************************************************************************/

/**
 * @addtogroup rtc
 * @{
 */

#include "rtc.h"

#ifdef RTC_USE_EXT
#include "ext_rtc.h"
#else
#include "itf_rtc.h"
#endif // RTC_USE_EXT

/****************************************************************************//*
 * Constants and macros
 ******************************************************************************/

/** Maximum number of system years. Range is 0 to YEAR_MAX - 1. */
#define YEAR_MAX            (100)

/** February month. */
#define MONTH_FEB           (2)

/** Days of February in leap year. */
#define MONTH_FEB_LEAP_DAYS (29)

/** Number of years representable by 2 digits. */
#define YEARS_2_DIGIT       (100)

/** Total years needed to reach a leap year. */
#define YEARS_LEAP          (4)

/** Number of months in a year. */
#define MONTHS_YEAR         (12)

/** Number of hours in a day. */
#define HOURS_DAY           (24)

/** Number of minutes in a hour. */
#define MINUTES_HOUR        (60)

/** Number of seconds in a quadrennium (with its leap year). */
#define SECONDS_QUADRENNIUM (126230400UL)

/** Number of seconds in a non leap year. */
#define SECONDS_YEAR        (31536000UL)

/** Number of seconds in a day. */
#define SECONDS_DAY         (86400UL)

/** Number of seconds in an hour. */
#define SECONDS_HOUR        (3600UL)

/** Number of seconds in a minute. */
#define SECONDS_MINUTE      (60UL)

/** Number of centiseconds in a second. */
#define CSECONDS_SECOND     (100)

/** Number of days form the beginning of a non leap year until 00:00 of the
 * first day of "i" month (range 1 to 12). */
static const uint32_t days_until_month_no_leap[] =
{
    0,      // Not used
    0,      // January
    31,
    59,
    90,
    120,
    151,
    181,
    212,
    243,
    273,
    304,
    334,    // December
};

/** Number of days form the beginning of a leap year until 00:00 of the first
 * day of "i" month (range 1 to 12). */
static const uint32_t days_until_month_leap[] =
{
    0,      // Not used
    0,      // January
    31,
    60,
    91,
    121,
    152,
    182,
    213,
    244,
    274,
    305,
    335,    // December
};

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** Difference between local time and GMT in hours. Range [-24, 24]. */
static int8_t rtc_delta_hour = 0;

/****************************************************************************//*
 * Private code prototypes
 ******************************************************************************/

/**
 * @brief Convert the year format from internal to 2 digit calendar.
 *
 * @param[in] year Year in system format, i.e., 0 equals to YEAR_BASE.
 *
 * @return Calendar year in 2 digit format from AGNO_BASE to (YEAR_BASE +
 * YEAR_MAX - 1).
 */
static uint8_t rtc_system_to_calendar_year(uint8_t year);

/**
 * @brief Convert the year format from calendar to internal.
 *
 * @param[in] year Year in calendar format from AGNO_BASE to (YEAR_BASE +
 * YEAR_MAX - 1).
 *
 * @return Year in system format, i.e., 0 equals to YEAR_BASE.
 */
static uint8_t rtc_calendar_to_system_year(uint8_t year);

/**
 * @brief Convert a timestamp that consists in the number of seconds from
 * 00:00:00 of 1st January of YEAR_BASE to datetime_t format with the year in
 * system format.
 *
 * @param[out] datetime Pointer to the converted date time.
 * @param[in] timestamp Timestamp to convert.
 */
static void rtc_timestamp_to_datetime(datetime_t * datetime,
                                      uint32_t timestamp);

/**
 * @brief Convert a datetime_t format with the year in system format to a
 * timestamp that consists in the number of seconds from 00:00:00 of 1st January
 * of YEAR_BASE.
 *
 * @param[out] timestamp Pointer to converted timestamp.
 * @param[in] datetime Pointer to the date time to be converted.
 * @param[in] fmt_local true: input format is local time plus time zone;
 *                      false: input format is UTC time plus time zone.
 */
static void rtc_datetime_to_timestamp(uint32_t * timestamp,
                                      const datetime_t * datetime,
                                      bool fmt_local);

/**
 * @brief Check if the date values are correct.
 *
 * @param[in] datetime Date/time with the year in system format.
 *
 * @retval true If values are correct.
 * @retval false If values are not correct.
 */
static bool rtc_is_date_ok(const datetime_t * datetime);

/**
 * @brief Check if the time values are correct.
 *
 * @param[in] datetime Date/time with the year in system format.
 *
 * @retval true If values are correct.
 * @retval false If values are not correct.
 */
static bool rtc_is_time_ok(const datetime_t * datetime);

/**
 * @brief Convert the date/time format from calendar to internal.
 *
 * @param[out] datetime_out Converted date/time.
 * @param[in] datetime_in Input date/time.
 */
static void rtc_convert_datetime(datetime_t * datetime_out,
                                 const datetime_t * datetime_in);

/****************************************************************************//*
 * Public code
 ******************************************************************************/

void
rtc_get_timestamp (uint32_t * timestamp, uint8_t * centiseconds)
{
#ifdef RTC_USE_EXT
    ext_rtc_get_time(timestamp, centiseconds);
#else
    itf_rtc_get_time(timestamp, centiseconds);
#endif // RTC_USE_EXT
}

void
rtc_get_epoch_timestamp (uint32_t * timestamp, uint8_t * centiseconds)
{
#ifdef RTC_USE_EXT
    ext_rtc_get_time(timestamp, centiseconds);
#else
    itf_rtc_get_time(timestamp, centiseconds);
#endif // RTC_USE_EXT

    *timestamp += EPOCH_BASE_TIME;
}

void
rtc_set_timestamp (uint32_t timestamp)
{
#ifdef RTC_USE_EXT
    ext_rtc_set_time(timestamp, 0);
#else
    itf_rtc_set_time(timestamp, 0);
#endif // RTC_USE_EXT
}

bool
rtc_set_datetime (const datetime_t * datetime, bool fmt_local)
{
    datetime_t dt;
    uint32_t   ts;

    rtc_convert_datetime(&dt, datetime);

    if (rtc_is_date_ok(&dt) && rtc_is_time_ok(&dt))
    {
        rtc_datetime_to_timestamp(&ts, &dt, fmt_local);
        rtc_delta_hour = datetime->delta_hour;

#ifdef RTC_USE_EXT
        ext_rtc_set_time(ts, datetime->cseconds);
#else
        itf_rtc_set_time(ts, datetime->cseconds);
#endif // RTC_USE_EXT

        return true;
    }

    return false;
}

void
rtc_get_datetime (datetime_t * datetime)
{
    uint32_t ts;
    uint8_t  tcs;

    rtc_get_timestamp(&ts, &tcs);
    datetime->delta_hour = rtc_delta_hour;
    rtc_timestamp_to_datetime(datetime, ts);
    datetime->year       = rtc_system_to_calendar_year(datetime->year);
    datetime->cseconds   = tcs;
}

int64_t
rtc_compare_datetime (const datetime_t * datetime_1, bool fmt_local_1,
                      const datetime_t * datetime_2, bool fmt_local_2)
{
    datetime_t dt_1;
    datetime_t dt_2;
    uint32_t   ts_1;
    uint32_t   ts_2;

    rtc_convert_datetime(&dt_1, datetime_1);
    rtc_datetime_to_timestamp(&ts_1, &dt_1, fmt_local_1);

    rtc_convert_datetime(&dt_2, datetime_2);
    rtc_datetime_to_timestamp(&ts_2, &dt_2, fmt_local_2);

    return (int64_t)ts_1 - (int64_t)ts_2;
}

/****************************************************************************//*
 * Private code
 ******************************************************************************/

static uint8_t
rtc_system_to_calendar_year (uint8_t year)
{
    return ((unsigned int)year + YEAR_BASE % YEARS_2_DIGIT) % YEARS_2_DIGIT;
}

static uint8_t
rtc_calendar_to_system_year (uint8_t year)
{
    // 2 digits year format
    if (year < YEARS_2_DIGIT)
    {
        if (year < YEAR_BASE % YEARS_2_DIGIT)
        {
            year += YEARS_2_DIGIT - (YEAR_BASE % YEARS_2_DIGIT);
        }
        else
        {
            year -= YEAR_BASE % YEARS_2_DIGIT;
        }
    }

    // If year is above (YEAR_BASE + YEAR_MAX - 1), set maximum year
    if (year >= YEAR_MAX)
    {
        year = YEAR_MAX - 1;
    }

    return year;
}

static void
rtc_timestamp_to_datetime (datetime_t * datetime, uint32_t timestamp)
{
    uint32_t         day_julian;
    uint32_t         year_quadrennium;
    uint32_t         i;
    const uint32_t * days_until_month;

    // Add the delta hour to set local time from UTC timestamp
    timestamp += (int32_t)datetime->delta_hour * SECONDS_HOUR;

    // Get the number of complete quadrenniums (with its leap year) and remove
    // them from the timestamp
    datetime->year = (timestamp / SECONDS_QUADRENNIUM) * 4;
    timestamp     %= SECONDS_QUADRENNIUM;

    // Get the rest of years that does not constitute a quadrennium. There is an
    // abnormality with the 31th of December of the last year, because it is
    // always leap. It will belong to the next year when dividing by
    // SECONDS_YEAR, giving the abnormal result of 4. The abnormality is
    // corrected here and the leap year will be taken into account later.
    year_quadrennium = timestamp / SECONDS_YEAR;

    if (year_quadrennium == 4)
    {
        year_quadrennium = 3;
    }

    // Add the number of additional years and remove them from the timestamp
    datetime->year += year_quadrennium;
    timestamp      -= year_quadrennium * SECONDS_YEAR;

    // Select the correct days until month table
    days_until_month = (datetime->year % 4 == 3)
                       ? days_until_month_leap : days_until_month_no_leap;

    // The remaining timestamp contains the number of seconds from the beginning
    // of the current year. Get the Julian day and remove it from the timestamp.
    // The remaining timestamp contains the number of seconds of the current day
    day_julian = timestamp / SECONDS_DAY + 1;
    timestamp %= SECONDS_DAY;

    for (i = MONTHS_YEAR; day_julian <= days_until_month[i]; i--)
    {
        // Get the month through the Julian day
    }

    // Fill the remaining datetime structure
    datetime->month   = i;
    datetime->day     = day_julian - days_until_month[i];
    datetime->hour    = timestamp / SECONDS_HOUR;
    datetime->minutes = (timestamp % SECONDS_HOUR) / SECONDS_MINUTE;
    datetime->seconds = timestamp % SECONDS_MINUTE;
}

static void
rtc_datetime_to_timestamp (uint32_t * timestamp, const datetime_t * datetime,
                           bool fmt_local)
{
    const uint32_t * days_until_month;

    // Count the number of seconds of all previous years and the additional
    // seconds of each previous leap year
    *timestamp = (uint32_t)datetime->year * SECONDS_YEAR
                 + (uint32_t)(datetime->year >> 2) * SECONDS_DAY;

    // Select the correct days until month table
    days_until_month = (datetime->year % 4 == 3)
                       ? days_until_month_leap : days_until_month_no_leap;

    // Add the number of seconds of all days of the previous months of the
    // current year
    *timestamp += days_until_month[datetime->month] * SECONDS_DAY;

    // Add the number of seconds of all the previous days of the current month
    *timestamp += (datetime->day - 1) * SECONDS_DAY;

    // Add the number of seconds of the current day
    *timestamp += (uint32_t)datetime->hour * SECONDS_HOUR
                  + (uint32_t)datetime->minutes * SECONDS_MINUTE
                  + (uint32_t)datetime->seconds;

    if (fmt_local)
    {
        // Subtract the delta hour to set UTC timestamp from local time
        *timestamp -= (int32_t)datetime->delta_hour * SECONDS_HOUR;
    }
}

static bool
rtc_is_date_ok (const datetime_t * datetime)
{
    static const uint8_t month_days[] =
    {
        31,
        28,
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31,
    };
    bool ret;

    ret = (datetime->month > 0)
          && (datetime->day > 0)
          && (datetime->month <= MONTHS_YEAR)
          && (datetime->year < YEAR_MAX)
          && ((datetime->day <= month_days[datetime->month - 1])
              || ((datetime->month == MONTH_FEB)
                  && (datetime->day == MONTH_FEB_LEAP_DAYS)
                  && (datetime->year % YEARS_LEAP == YEARS_LEAP - 1)));

    return ret;
}

static bool
rtc_is_time_ok (const datetime_t * datetime)
{
    bool ret;

    ret = (datetime->hour < HOURS_DAY)
          && (datetime->minutes < MINUTES_HOUR)
          && (datetime->seconds < SECONDS_MINUTE)
          && (datetime->cseconds < CSECONDS_SECOND)
          && (datetime->delta_hour >= -HOURS_DAY)
          && (datetime->delta_hour <= HOURS_DAY);

    return ret;
}

static void
rtc_convert_datetime (datetime_t * datetime_out, const datetime_t * datetime_in)
{
    datetime_out->year       = rtc_calendar_to_system_year(datetime_in->year);
    datetime_out->month      = datetime_in->month;
    datetime_out->day        = datetime_in->day;
    datetime_out->hour       = datetime_in->hour;
    datetime_out->minutes    = datetime_in->minutes;
    datetime_out->seconds    = datetime_in->seconds;
    datetime_out->cseconds   = datetime_in->cseconds;
    datetime_out->delta_hour = datetime_in->delta_hour;
}

/** @} */

/******************************** End of file *********************************/
