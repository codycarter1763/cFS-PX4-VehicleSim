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
 * Auto-Generated stub implementations for functions defined in fm_dispatch header
 */

#include "fm_dispatch.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for FM_ProcessGroundCommand()
 * ----------------------------------------------------
 */
void FM_ProcessGroundCommand(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(FM_ProcessGroundCommand, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(FM_ProcessGroundCommand, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_TaskPipe()
 * ----------------------------------------------------
 */
void FM_TaskPipe(const CFE_SB_Buffer_t *BufPtr)
{
    UT_GenStub_AddParam(FM_TaskPipe, const CFE_SB_Buffer_t *, BufPtr);

    UT_GenStub_Execute(FM_TaskPipe, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for FM_VerifyCmdLength()
 * ----------------------------------------------------
 */
bool FM_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    UT_GenStub_SetupReturnBuffer(FM_VerifyCmdLength, bool);

    UT_GenStub_AddParam(FM_VerifyCmdLength, const CFE_MSG_Message_t *, MsgPtr);
    UT_GenStub_AddParam(FM_VerifyCmdLength, size_t, ExpectedLength);

    UT_GenStub_Execute(FM_VerifyCmdLength, Basic, NULL);

    return UT_GenStub_GetReturnValue(FM_VerifyCmdLength, bool);
}
