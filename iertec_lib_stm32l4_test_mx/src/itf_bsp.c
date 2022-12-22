/*******************************************************************************
 * @file itf_bsp.c
 * @author juanmanuel.fernandez@iertec.com
 * @date 22 Dec 2022
 * @brief Interfaces board support package.
 * @ingroup itf_bsp
 ******************************************************************************/

/**
 * @addtogroup itf_bsp
 * @{
 */

#include "itf_bsp.h"
#include "itf_io.h"
#include "main.h"

/****************************************************************************//*
 * itf_io board configuration
 ******************************************************************************/

/** Board configuration of the available I/O lines. */
const itf_io_config_t itf_io_config[H_ITF_IO_COUNT] =
{
    {
        .port = B1_GPIO_Port,
        .pin = B1_Pin,
    },
    {
        .port = LD4_GPIO_Port,
        .pin = LD4_Pin,
    },
};

/****************************************************************************//*
 * Public functions
 ******************************************************************************/

bool
itf_bsp_init (void)
{
#ifndef TEST
    bool ret = true;

    ret = itf_io_init() && ret;

    return ret;
#else // !TEST
    return false;
#endif // TEST
}

/** @} */

/******************************** End of file *********************************/
