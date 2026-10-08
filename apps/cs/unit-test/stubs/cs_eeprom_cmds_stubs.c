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
 * Auto-Generated stub implementations for functions defined in cs_eeprom_cmds header
 */

#include "cs_eeprom_cmds.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for CS_DisableEepromCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_DisableEepromCmd(const CS_DisableEepromCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_DisableEepromCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_DisableEepromCmd, const CS_DisableEepromCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_DisableEepromCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_DisableEepromCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_DisableEntryIDEepromCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_DisableEntryIDEepromCmd(const CS_DisableEntryIDEepromCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_DisableEntryIDEepromCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_DisableEntryIDEepromCmd, const CS_DisableEntryIDEepromCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_DisableEntryIDEepromCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_DisableEntryIDEepromCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_EnableEepromCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_EnableEepromCmd(const CS_EnableEepromCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_EnableEepromCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_EnableEepromCmd, const CS_EnableEepromCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_EnableEepromCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_EnableEepromCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_EnableEntryIDEepromCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_EnableEntryIDEepromCmd(const CS_EnableEntryIDEepromCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_EnableEntryIDEepromCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_EnableEntryIDEepromCmd, const CS_EnableEntryIDEepromCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_EnableEntryIDEepromCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_EnableEntryIDEepromCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_GetEntryIDEepromCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_GetEntryIDEepromCmd(const CS_GetEntryIDEepromCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_GetEntryIDEepromCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_GetEntryIDEepromCmd, const CS_GetEntryIDEepromCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_GetEntryIDEepromCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_GetEntryIDEepromCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_RecomputeBaselineEepromCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_RecomputeBaselineEepromCmd(const CS_RecomputeBaselineEepromCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_RecomputeBaselineEepromCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_RecomputeBaselineEepromCmd, const CS_RecomputeBaselineEepromCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_RecomputeBaselineEepromCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_RecomputeBaselineEepromCmd, CFE_Status_t);
}

/*
 * ----------------------------------------------------
 * Generated stub function for CS_ReportBaselineEntryIDEepromCmd()
 * ----------------------------------------------------
 */
CFE_Status_t CS_ReportBaselineEntryIDEepromCmd(const CS_ReportBaselineEntryIDEepromCmd_t *CmdPtr)
{
    UT_GenStub_SetupReturnBuffer(CS_ReportBaselineEntryIDEepromCmd, CFE_Status_t);

    UT_GenStub_AddParam(CS_ReportBaselineEntryIDEepromCmd, const CS_ReportBaselineEntryIDEepromCmd_t *, CmdPtr);

    UT_GenStub_Execute(CS_ReportBaselineEntryIDEepromCmd, Basic, NULL);

    return UT_GenStub_GetReturnValue(CS_ReportBaselineEntryIDEepromCmd, CFE_Status_t);
}
