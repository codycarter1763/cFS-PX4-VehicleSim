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
 *   This file contains the source code for the Mavlink App Ground Command-handling functions
 */

/*
** Include Files:
*/
#include "mavlink_app.h"
#include "mavlink_app_cmds.h"
#include "mavlink_app_msgids.h"
#include "mavlink_app_eventids.h"
#include "mavlink_app_version.h"
#include "mavlink_app_tbl.h"
#include "mavlink_app_utils.h"
#include "mavlink_app_msg.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
#include <common/mavlink.h>
#pragma GCC diagnostic pop

/* The sample_lib module provides the MAVLINK_Function() prototype */
#include "sample_lib.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t MAVLINK_APP_SendHkCmd(const MAVLINK_APP_SendHkCmd_t *Msg)
{
    int i;

    /*
    ** Get command execution counters...
    */
    MAVLINK_APP_Data.HkTlm.Payload.CommandErrorCounter = MAVLINK_APP_Data.CommandErrorCounter;
    MAVLINK_APP_Data.HkTlm.Payload.CommandCounter      = MAVLINK_APP_Data.CommandCounter;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(MAVLINK_APP_Data.HkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(MAVLINK_APP_Data.HkTlm.TelemetryHeader), true);

    /*
    ** Manage any pending table loads, validations, etc.
    */
    for (i = 0; i < MAVLINK_APP_PLATFORM_NUMBER_OF_TABLES; i++)
    {
        CFE_TBL_Manage(MAVLINK_APP_Data.TblHandles[i]);
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* MAVLINK NOOP commands                                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t MAVLINK_APP_NoopCmd(const MAVLINK_APP_NoopCmd_t *Msg)
{
    MAVLINK_APP_Data.CommandCounter++;

    CFE_EVS_SendEvent(MAVLINK_APP_NOOP_INF_EID,
                      CFE_EVS_EventType_INFORMATION,
                      "MAVLINK: NOOP command %s",
                      MAVLINK_APP_VERSION);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t MAVLINK_APP_ResetCountersCmd(const MAVLINK_APP_ResetCountersCmd_t *Msg)
{
    MAVLINK_APP_Data.CommandCounter      = 0;
    MAVLINK_APP_Data.CommandErrorCounter = 0;

    CFE_EVS_SendEvent(MAVLINK_APP_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, "MAVLINK: RESET command");

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function Process Ground Station Command                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t MAVLINK_APP_ProcessCmd(const MAVLINK_APP_ProcessCmd_t *Msg)
{
    CFE_Status_t               Status;
    void                      *TblAddr;
    MAVLINK_APP_ExampleTable_t *TblPtr;
    const char                *TableName = "MAVLINK_APP.ExampleTable";

    /* Mavlink Use of Example Table */
    MAVLINK_APP_Data.CommandCounter++;
    Status = CFE_TBL_GetAddress(&TblAddr, MAVLINK_APP_Data.TblHandles[0]);
    if (Status < CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Mavlink App: Fail to get table address: 0x%08lx", (unsigned long)Status);
    }
    else
    {
        TblPtr = TblAddr;
        CFE_ES_WriteToSysLog("Mavlink App: Example Table Value 1: %d  Value 2: %d", TblPtr->Int1, TblPtr->Int2);

        MAVLINK_APP_GetCrc(TableName);

        Status = CFE_TBL_ReleaseAddress(MAVLINK_APP_Data.TblHandles[0]);
        if (Status != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("Mavlink App: Fail to release table address: 0x%08lx", (unsigned long)Status);
        }
        else
        {
            /* Invoke a function provided by MAVLINK_APP_LIB */
            SAMPLE_LIB_Function();
        }
    }

    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* A simple example command that displays a passed-in value                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t MAVLINK_APP_DisplayParamCmd(const MAVLINK_APP_DisplayParamCmd_t *Msg)
{
    MAVLINK_APP_Data.CommandCounter++;
    CFE_EVS_SendEvent(MAVLINK_APP_VALUE_INF_EID,
                      CFE_EVS_EventType_INFORMATION,
                      "MAVLINK_APP: ValU32=%lu, ValI16=%d, ValStr=%s",
                      (unsigned long)Msg->Payload.ValU32,
                      (int)Msg->Payload.ValI16,
                      Msg->Payload.ValStr);

    return CFE_SUCCESS;
}

CFE_Status_t MAVLINK_APP_StartMissionCmd(
    const MAVLINK_APP_StartMissionCmd_t *Msg)
{
    mavlink_message_t MavMsg;
    uint8 Buffer[MAVLINK_MAX_PACKET_LEN];
    uint16 Length;
    int32 Status;

    (void)Msg;

    mavlink_msg_command_long_pack(
        1,
        200,
        &MavMsg,
        1,
        1,
        MAV_CMD_MISSION_START,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0);

    Length = mavlink_msg_to_send_buffer(Buffer, &MavMsg);

    Status = OS_SocketSendTo(
        MAVLINK_APP_Data.SockId,
        Buffer,
        Length,
        &MAVLINK_APP_Data.Px4Addr);

    if (Status < 0)
    {
        MAVLINK_APP_Data.CommandErrorCounter++;

        CFE_EVS_SendEvent(MAVLINK_APP_START_MISSION_ERR_EID,
                          CFE_EVS_EventType_ERROR,
                          "MAVLINK: Failed to send MISSION_START: %ld",
                          (long)Status);

        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    MAVLINK_APP_Data.CommandCounter++;

    CFE_EVS_SendEvent(MAVLINK_APP_START_MISSION_INF_EID,
                      CFE_EVS_EventType_INFORMATION,
                      "MAVLINK: MISSION_START sent to PX4");

    return CFE_SUCCESS;
}
