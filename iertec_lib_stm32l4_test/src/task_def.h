/*******************************************************************************
 * @file task_def.h
 * @author juanmanuel.fernandez@iertec.com
 * @date 07 Dec 2022
 * @brief Task common definitions.
 * @ingroup task_def
 ******************************************************************************/

/**
 * @defgroup task_def task_def
 * @brief Task common definitions.
 * @{
 */

#ifndef TASK_DEF_H
#define TASK_DEF_H

/** Task identifiers. */
typedef enum
{
    TASK_ID_0 = 0,
    TASK_ID_1,
    TASK_ID_2,
    TASK_ID_3,
    TASK_ID_COUNT,
    TASK_ID_NONE = -1,
} task_id_t;

#endif // TASK_DEF_H

/** @} */

/******************************** End of file *********************************/
