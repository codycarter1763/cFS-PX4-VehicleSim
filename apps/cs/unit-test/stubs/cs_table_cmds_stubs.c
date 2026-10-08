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
 * Auto-Generated stub implementations for functions defined in cs_table_cmds header
 */

#include "cs_table_cmds.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for CS_DisableNameTableCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_DisableNameTableCmd(const CS_DisableNameTableCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_DisableNameTableCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_DisableNameTableCmd, const CS_DisableNameTableCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_DisableNameTableCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_DisableNameTableCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_DisableTablesCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_DisableTablesCmd(const CS_DisableTablesCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_DisableTablesCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_DisableTablesCmd, const CS_DisableTablesCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_DisableTablesCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_DisableTablesCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_EnableNameTableCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_EnableNameTableCmd(const CS_EnableNameTableCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_EnableNameTableCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_EnableNameTableCmd, const CS_EnableNameTableCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_EnableNameTableCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_EnableNameTableCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_EnableTablesCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_EnableTablesCmd(const CS_EnableTablesCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_EnableTablesCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_EnableTablesCmd, const CS_EnableTablesCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_EnableTablesCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_EnableTablesCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_RecomputeBaselineTableCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_RecomputeBaselineTableCmd(const CS_RecomputeBaselineTableCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_RecomputeBaselineTableCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_RecomputeBaselineTableCmd, const CS_RecomputeBaselineTableCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_RecomputeBaselineTableCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_RecomputeBaselineTableCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_ReportBaselineTableCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_ReportBaselineTableCmd(const CS_ReportBaselineTableCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_ReportBaselineTableCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_ReportBaselineTableCmd, const CS_ReportBaselineTableCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_ReportBaselineTableCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_ReportBaselineTableCmd, CFE_Status_t);
}
