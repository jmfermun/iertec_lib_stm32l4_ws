/*******************************************************************************
 * @file rtc_timer.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 19 Apr 2023
 * @brief Generic timers that can use any tick interrupt source.
 * @ingroup rtc_timer
 ******************************************************************************/

/**
 * @addtogroup rtc_timer
 * @{
 */

#include "rtc_timer.h"
#include "sys_util.h"
#include "debug_util.h"

/****************************************************************************//*
 * Private data
 ******************************************************************************/

/** List of active timers. */
static rtc_timer_t * rtc_timer_list;

/****************************************************************************//*
 * Public code
 ******************************************************************************/

void
rtc_timer_init (void)
{
    rtc_timer_list = NULL;
}

void
rtc_timer_config (rtc_timer_t * timer, uint32_t id, rtc_timer_fn fn)
{
    DEBUG_ASSERT(timer != NULL);

    SYS_ENTER_CRITICAL();

    timer->active   = false;
    timer->ticks    = 0;
    timer->next     = NULL;
    timer->next_exp = NULL;
    timer->id       = id;
    timer->fn       = fn;

    SYS_EXIT_CRITICAL();
}

void
rtc_timer_start (rtc_timer_t * timer, uint32_t ticks)
{
    DEBUG_ASSERT(timer != NULL);

    // At least 1 tick
    if (0 == ticks)
    {
        ticks++;
    }

    SYS_ENTER_CRITICAL();

    if (timer->active)
    {
        rtc_timer_stop(timer);
    }

    if (rtc_timer_list != NULL)
    {
        if (ticks < rtc_timer_list->ticks)
        {
            // This timer will be the first one in the list
            timer->ticks = ticks;

            // The ticks of the old first timer change to be relative to the new
            // first timer ticks
            rtc_timer_list->ticks -= ticks;

            // Link them
            timer->next    = rtc_timer_list;
            rtc_timer_list = timer;
        }
        else
        {
            // The timer will be located in an intermediate position or at the
            // end
            rtc_timer_t * t_prev = rtc_timer_list;
            rtc_timer_t * t_next = rtc_timer_list->next;

            timer->ticks = ticks - rtc_timer_list->ticks;

            // Search the timer position. Iterate while there are timers in the
            // list and the new timer ticks are greater than the checked one
            while ((t_next != NULL) && (timer->ticks >= t_next->ticks))
            {
                timer->ticks -= t_next->ticks;
                t_prev        = t_next;
                t_next        = t_next->next;
            }

            // Insert the timer after t_prev and before t_next
            t_prev->next = timer;
            timer->next  = t_next;

            // The next timer to the new one must recalculate its ticks relative
            // to the new one
            if (t_next != NULL)
            {
                t_next->ticks -= timer->ticks;
            }
        }
    }
    else
    {
        // If there are no timers active, set this one as the first
        timer->ticks   = ticks;
        timer->next    = NULL;
        rtc_timer_list = timer;
    }

    timer->active = true;

    SYS_EXIT_CRITICAL();
}

void
rtc_timer_stop (rtc_timer_t * timer)
{
    DEBUG_ASSERT(timer != NULL);

    SYS_ENTER_CRITICAL();

    if (timer->active)
    {
        if (timer == rtc_timer_list)
        {
            rtc_timer_list = rtc_timer_list->next;

            if (rtc_timer_list != NULL)
            {
                // Add the stopped timer ticks to the next timer
                rtc_timer_list->ticks += timer->ticks;
            }
        }
        else
        {
            rtc_timer_t * t_prev = rtc_timer_list;

            // Search the preceding timer
            while ((t_prev != NULL) && (t_prev->next != timer))
            {
                t_prev = t_prev->next;
            }

            // The following condition should be true always
            if (t_prev != NULL)
            {
                t_prev->next = timer->next;

                if (timer->next != NULL)
                {
                    // Add the stopped timer ticks to the next timer
                    timer->next->ticks += timer->ticks;
                }
            }
        }

        timer->ticks  = 0;
        timer->active = false;
    }

    SYS_EXIT_CRITICAL();
}

void
rtc_timer_tick (void)
{
    // Check that an expiration has occurred
    if ((rtc_timer_list != NULL) && (--rtc_timer_list->ticks == 0))
    {
        rtc_timer_t * list_exp = rtc_timer_list;
        rtc_timer_t * t;

        // Compose an expired list with all the consecutive timers that have
        // their ticks equal to 0
        do
        {
            // Remove the expired timer from the timer list
            t              = rtc_timer_list;
            rtc_timer_list = rtc_timer_list->next;

            // Deactivate the expired timer
            t->active = false;

            // Link the expired timer with the next one if it has expired too
            if ((rtc_timer_list != NULL) && (rtc_timer_list->ticks == 0))
            {
                t->next_exp = rtc_timer_list;
            }
            else
            {
                t->next_exp = NULL;
            }
        } while (t->next_exp != NULL);

        // Now call to all the expired timer handlers
        while (list_exp != NULL)
        {
            if ((list_exp->active == false) && (list_exp->fn != NULL))
            {
                list_exp->fn(list_exp);
            }

            // Go to the next expired timer
            list_exp = list_exp->next_exp;
        }
    }
}

/** @} */

/******************************** End of file *********************************/
