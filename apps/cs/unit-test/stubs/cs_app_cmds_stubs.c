/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in cs_app_cmds header
 */

#include "cs_app_cmds.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for CS_DisableAppsCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_DisableAppsCmd(const CS_DisableAppsCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_DisableAppsCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_DisableAppsCmd, const CS_DisableAppsCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_DisableAppsCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_DisableAppsCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_DisableNameAppCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_DisableNameAppCmd(const CS_DisableNameAppCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_DisableNameAppCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_DisableNameAppCmd, const CS_DisableNameAppCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_DisableNameAppCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_DisableNameAppCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_EnableAppsCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_EnableAppsCmd(const CS_EnableAppsCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_EnableAppsCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_EnableAppsCmd, const CS_EnableAppsCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_EnableAppsCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_EnableAppsCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_EnableNameAppCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_EnableNameAppCmd(const CS_EnableNameAppCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_EnableNameAppCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_EnableNameAppCmd, const CS_EnableNameAppCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_EnableNameAppCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_EnableNameAppCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_RecomputeBaselineAppCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_RecomputeBaselineAppCmd(const CS_RecomputeBaselineAppCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_RecomputeBaselineAppCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_RecomputeBaselineAppCmd, const CS_RecomputeBaselineAppCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_RecomputeBaselineAppCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_RecomputeBaselineAppCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_ReportBaselineAppCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_ReportBaselineAppCmd(const CS_ReportBaselineAppCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_ReportBaselineAppCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_ReportBaselineAppCmd, const CS_ReportBaselineAppCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_ReportBaselineAppCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_ReportBaselineAppCmd, CFE_Status_t);
}
