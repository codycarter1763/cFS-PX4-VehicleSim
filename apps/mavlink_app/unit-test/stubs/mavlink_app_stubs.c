/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in mavlink_app header
 */

#include "mavlink_app.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for MAVLINK_APP_Init()
 * ----------------------------------------------------
 */
CFE_Status_t MAVLINK_APP_Init(void)
{
    UT_GenStub_SetupReturnBuffer(MAVLINK_APP_Init, CFE_Status_t);

    UT_GenStub_Execute(MAVLINK_APP_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(MAVLINK_APP_Init, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for MAVLINK_APP_Main()
 * ----------------------------------------------------
 */
void MAVLINK_APP_Main(void)
{
    UT_GenStub_Execute(MAVLINK_APP_Main, Basic, NULL);
}