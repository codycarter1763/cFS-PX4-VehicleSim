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
 * \file
 *   This file contains the source code for the Mavlink App.
 */

/*
** Include Files:
*/
#include "mavlink_app.h"
#include "mavlink_app_dispatch.h"
#include "mavlink_app_cmds.h"
#include "mavlink_app_eventids.h"
#include "mavlink_app_msgids.h"
#include "mavlink_app_msg.h"

#include "mavlink_app_eds_dispatcher.h"
#include "mavlink_app_eds_dictionary.h"

/*
 * Define a lookup table for MAVLINK app command codes
 */
/* clang-format off */
static const EdsDispatchTable_EdsComponent_MAVLINK_APP_Application_CFE_SB_Telecommand_t MAVLINK_TC_DISPATCH_TABLE =
{
    .CMD =
    {
        .NoopCmd_indication          = MAVLINK_APP_NoopCmd,
        .ResetCountersCmd_indication = MAVLINK_APP_ResetCountersCmd,
        .ProcessCmd_indication       = MAVLINK_APP_ProcessCmd,
        .DisplayParamCmd_indication  = MAVLINK_APP_DisplayParamCmd
    },
    .SEND_HK =
    {
        .indication = MAVLINK_APP_SendHkCmd
    }
};
/* clang-format on */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the MAVLINK    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void MAVLINK_APP_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_Status_t      Status;
    CFE_SB_MsgId_t    MsgId;
    CFE_MSG_Size_t    MsgSize;
    CFE_MSG_FcnCode_t MsgFc;

    Status = EdsDispatch_EdsComponent_MAVLINK_APP_Application_Telecommand(SBBufPtr, &MAVLINK_TC_DISPATCH_TABLE);

    if (Status != CFE_SUCCESS)
    {
        CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);
        CFE_MSG_GetSize(&SBBufPtr->Msg, &MsgSize);
        CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &MsgFc);
        ++MAVLINK_APP_Data.CommandErrorCounter;

        if (Status == CFE_STATUS_UNKNOWN_MSG_ID)
        {
            CFE_EVS_SendEvent(MAVLINK_APP_MID_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "MAVLINK: invalid command packet,MID = 0x%x",
                              (unsigned int)CFE_SB_MsgIdToValue(MsgId));
        }
        else if (Status == CFE_STATUS_WRONG_MSG_LENGTH)
        {
            CFE_EVS_SendEvent(MAVLINK_APP_CMD_LEN_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u",
                              (unsigned int)CFE_SB_MsgIdToValue(MsgId),
                              (unsigned int)MsgFc,
                              (unsigned int)MsgSize);
        }
        else
        {
            CFE_EVS_SendEvent(MAVLINK_APP_CC_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "MAVLINK: Invalid ground command code: CC = %d",
                              (int)MsgFc);
        }
    }
}
