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
#define _GNU_SOURCE
#include "mavlink_app.h"
#include "mavlink_app_cmds.h"
#include "mavlink_app_utils.h"
#include "mavlink_app_eventids.h"
#include "mavlink_app_dispatch.h"
#include "mavlink_app_tbl.h"
#include "mavlink_app_version.h"

#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
#include <common/mavlink.h>
#pragma GCC diagnostic pop

/*
** global data
*/
MAVLINK_APP_Data_t MAVLINK_APP_Data;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/*                                                                            */
/* Application entry point and main process loop                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void MAVLINK_APP_Main(void)
{
    CFE_Status_t     status;
    CFE_SB_Buffer_t *SBBufPtr;

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(MAVLINK_APP_PERF_ID);

    /*
    ** Perform application-specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = MAVLINK_APP_Init();
    if (status != CFE_SUCCESS)
    {
        MAVLINK_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** Mavlink App Runloop
    */
    while (CFE_ES_RunLoop(&MAVLINK_APP_Data.RunStatus) == true)
    {
        /*
        ** Performance Log Exit Stamp
        */
        CFE_ES_PerfLogExit(MAVLINK_APP_PERF_ID);

        /* Pend on receipt of command packet */
        status = CFE_SB_ReceiveBuffer(&SBBufPtr, MAVLINK_APP_Data.CommandPipe, CFE_SB_PEND_FOREVER);

        /*
        ** Performance Log Entry Stamp
        */
        CFE_ES_PerfLogEntry(MAVLINK_APP_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            MAVLINK_APP_TaskPipe(SBBufPtr);
        }
        else
        {
            CFE_EVS_SendEvent(MAVLINK_APP_PIPE_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "MAVLINK APP: SB Pipe Read Error, App Will Exit");

            MAVLINK_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /*
    ** Performance Log Exit Stamp
    */
    CFE_ES_PerfLogExit(MAVLINK_APP_PERF_ID);

    CFE_ES_ExitApp(MAVLINK_APP_Data.RunStatus);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                            */
/* UDP receive and MAVLink parse task                                         */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static void MAVLINK_APP_Rx(void)
{
    uint8             input_buffer[DEFAULT_MAVLINK_APP_Rx_BUFFER_SIZE];
    OS_SockAddr_t     remote;
    int32             n;
    int32             i;
    mavlink_message_t msg;
    mavlink_status_t  status;

    memset(&status, 0, sizeof(status));

    OS_printf("[MAVLINK_APP] RX task started\n");

    while (MAVLINK_APP_Data.RunStatus == CFE_ES_RunStatus_APP_RUN)
    {
        n = OS_SocketRecvFrom(MAVLINK_APP_Data.SockId, input_buffer,
                              sizeof(input_buffer), &remote, 1000);
        if (n <= 0)
        {
            continue; /* timeout or error: re-check RunStatus */
        }

        MAVLINK_APP_Data.RxPacketCount++;
        MAVLINK_APP_Data.RxByteCount += (uint32)n;

        /* A datagram can hold more than one message, so feed every byte */
        for (i = 0; i < n; i++)
        {
            if (!mavlink_parse_char(MAVLINK_COMM_0, input_buffer[i], &msg, &status))
            {
                continue; /* message not complete yet */
            }

            MAVLINK_APP_Data.ParseState.MsgCount++;

            switch (msg.msgid)
            {
                case MAVLINK_MSG_ID_HEARTBEAT:
                {
                    mavlink_heartbeat_t heartbeat;
                    mavlink_msg_heartbeat_decode(&msg, &heartbeat);

                    MAVLINK_APP_Data.ParseState.HeartbeatCount++;
                    MAVLINK_APP_Data.ParseState.Armed = (heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) ? 1 : 0;
                    MAVLINK_APP_Data.ParseState.MainMode = (heartbeat.custom_mode >> 16) & 0xFF;
                    MAVLINK_APP_Data.ParseState.SubMode = (heartbeat.custom_mode >> 24) & 0xFF;
                    MAVLINK_APP_Data.ParseState.SystemStatus = heartbeat.system_status;

                    MAVLINK_APP_Data.HkTlm.Payload.Armed = MAVLINK_APP_Data.ParseState.Armed;
                    MAVLINK_APP_Data.HkTlm.Payload.MainMode = MAVLINK_APP_Data.ParseState.MainMode;
                    MAVLINK_APP_Data.HkTlm.Payload.SubMode = MAVLINK_APP_Data.ParseState.SubMode;
                    MAVLINK_APP_Data.HkTlm.Payload.HeartbeatCount = MAVLINK_APP_Data.ParseState.HeartbeatCount;
                    
                    CFE_SB_TimeStampMsg(CFE_MSG_PTR(MAVLINK_APP_Data.HkTlm.TelemetryHeader));
                    CFE_SB_TransmitMsg(CFE_MSG_PTR(MAVLINK_APP_Data.HkTlm.TelemetryHeader), true);

                    break;
                }

                case MAVLINK_MSG_ID_ATTITUDE:
                {
                    mavlink_attitude_t attitude;
                    mavlink_msg_attitude_decode(&msg, &attitude);

                    MAVLINK_APP_Data.NavTlm.Payload.RollRad = attitude.roll;
                    MAVLINK_APP_Data.NavTlm.Payload.PitchRad = attitude.pitch;
                    MAVLINK_APP_Data.NavTlm.Payload.YawRad = attitude.yaw;  
                    MAVLINK_APP_Data.NavTlm.Payload.RollRateRadPs = attitude.rollspeed;
                    MAVLINK_APP_Data.NavTlm.Payload.PitchRateRadPs = attitude.pitchspeed;
                    MAVLINK_APP_Data.NavTlm.Payload.YawRateRadPs = attitude.yawspeed;
                    break;
                }

                case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
                {
                    mavlink_global_position_int_t global_position;
                    mavlink_msg_global_position_int_decode(&msg, &global_position);

                    MAVLINK_APP_Data.NavTlm.Payload.LatDegE7 = global_position.lat;
                    MAVLINK_APP_Data.NavTlm.Payload.LonDegE7 = global_position.lon;
                    MAVLINK_APP_Data.NavTlm.Payload.RelativeAltitude = global_position.relative_alt;
                    MAVLINK_APP_Data.NavTlm.Payload.VxCms = global_position.vx;
                    MAVLINK_APP_Data.NavTlm.Payload.VyCms = global_position.vy;
                    MAVLINK_APP_Data.NavTlm.Payload.VzCms = global_position.vz;
                    MAVLINK_APP_Data.NavTlm.Payload.HeadingCDeg = global_position.hdg;
                    
                    /* Only post SB messages at 5Hz instead of 50Hz*/
                    static uint8 NavCounter = 0;

                    NavCounter++;

                    if (NavCounter >= 10)
                    {
                        NavCounter = 0;
                        CFE_SB_TimeStampMsg(CFE_MSG_PTR(MAVLINK_APP_Data.NavTlm.TelemetryHeader));
                        CFE_SB_TransmitMsg(CFE_MSG_PTR(MAVLINK_APP_Data.NavTlm.TelemetryHeader), true);
                    }
                    break;
                }

                case MAVLINK_MSG_ID_GPS_RAW_INT:
                {
                    mavlink_gps_raw_int_t gps_raw;
                    mavlink_msg_gps_raw_int_decode(&msg, &gps_raw);

                    MAVLINK_APP_Data.HkTlm.Payload.GpsFixType = gps_raw.fix_type;
                    MAVLINK_APP_Data.HkTlm.Payload.GpsNumSatellites = gps_raw.satellites_visible;
                    break;
                }

                case MAVLINK_MSG_ID_SYS_STATUS:
                {
                    mavlink_sys_status_t sys_status;
                    mavlink_msg_sys_status_decode(&msg, &sys_status);

                    MAVLINK_APP_Data.HkTlm.Payload.BatteryRemaining = sys_status.battery_remaining;
                    MAVLINK_APP_Data.HkTlm.Payload.BatteryVoltage = sys_status.voltage_battery / 1000.0f; /* mV to V */
                    MAVLINK_APP_Data.HkTlm.Payload.BatteryCurrent = sys_status.current_battery / 100.0f;  /* cA to A */
                    break;
                }

                case MAVLINK_MSG_ID_STATUSTEXT:
                {
                    mavlink_statustext_t statustext;
                    mavlink_msg_statustext_decode(&msg, &statustext);

                    MAVLINK_APP_Data.Px4StatusText.Severity = statustext.severity;
                    

                    memcpy(MAVLINK_APP_Data.Px4StatusText.Text, statustext.text, 50);

                    MAVLINK_APP_Data.Px4StatusText.Text[49] = '\0';

                    MAVLINK_APP_Data.Px4StatusText.TextLen = strlen(MAVLINK_APP_Data.Px4StatusText.Text);
                    CFE_EVS_SendEvent(
                        MAVLINK_APP_PX4_STATUSTEXT_EID,
                        CFE_EVS_EventType_INFORMATION,
                        "PX4 STATUSTEXT: severity=%u text=[%s]",
                        (unsigned int)MAVLINK_APP_Data.Px4StatusText.Severity, MAVLINK_APP_Data.Px4StatusText.Text);

                        break;
}

                default:
                    break;
            }
        }

        MAVLINK_APP_Data.ParseState.ParseDrops = status.packet_rx_drop_count;
    }
}

static CFE_Status_t MAVLINK_APP_StartUdp(void)
{
    OS_SockAddr_t addr;
    int32         rc;

    rc = OS_SocketOpen(&MAVLINK_APP_Data.SockId, OS_SocketDomain_INET, OS_SocketType_DATAGRAM);
    if (rc != OS_SUCCESS)
    {
        OS_printf("[MAVLINK_APP] OS_SocketOpen failed: %d\n", (int)rc);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    OS_SocketAddrInit(&addr, OS_SocketDomain_INET);
    OS_SocketAddrSetPort(&addr, DEFAULT_MAVLINK_APP_UDP_PORT);

    rc = OS_SocketBind(MAVLINK_APP_Data.SockId, &addr);
    if (rc != OS_SUCCESS)
    {
        OS_printf("[MAVLINK_APP] OS_SocketBind failed: %d (port in use?)\n", (int)rc);
        return CFE_STATUS_EXTERNAL_RESOURCE_FAIL;
    }

    OS_SocketAddrInit(&MAVLINK_APP_Data.Px4Addr, OS_SocketDomain_INET);
        OS_SocketAddrFromString(&MAVLINK_APP_Data.Px4Addr, "127.0.0.1");
    OS_SocketAddrSetPort(&MAVLINK_APP_Data.Px4Addr, DEFAULT_PX4_UDP_PORT);
    
    return CFE_ES_CreateChildTask(&MAVLINK_APP_Data.RxTaskId, "MAVLINK_RX",
                                  MAVLINK_APP_Rx, NULL,
                                  CFE_PLATFORM_ES_DEFAULT_STACK_SIZE, 100, 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* Initialization                                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t MAVLINK_APP_Init(void)
{
    CFE_Status_t status;
    char         VersionString[MAVLINK_APP_CFG_MAX_VERSION_STR_LEN];

    /* Zero out the global data structure */
    memset(&MAVLINK_APP_Data, 0, sizeof(MAVLINK_APP_Data));

    MAVLINK_APP_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Register the events
    */
    status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Mavlink App: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
    }
    else
    {
        /*
         ** Initialize housekeeping packet (clear user data area).
         */
        CFE_MSG_Init(CFE_MSG_PTR(MAVLINK_APP_Data.HkTlm.TelemetryHeader),
                     CFE_SB_ValueToMsgId(MAVLINK_APP_HK_TLM_MID),
                     sizeof(MAVLINK_APP_Data.HkTlm));

        CFE_MSG_Init(CFE_MSG_PTR(MAVLINK_APP_Data.NavTlm.TelemetryHeader),
             CFE_SB_ValueToMsgId(MAVLINK_APP_NAV_TLM_MID),
             sizeof(MAVLINK_APP_Data.NavTlm));
        
        
        CFE_SB_MsgId_t HkMsgId;
CFE_SB_MsgId_t NavMsgId;

CFE_MSG_GetMsgId(
    CFE_MSG_PTR(MAVLINK_APP_Data.HkTlm.TelemetryHeader),
    &HkMsgId);

CFE_MSG_GetMsgId(
    CFE_MSG_PTR(MAVLINK_APP_Data.NavTlm.TelemetryHeader),
    &NavMsgId);

CFE_EVS_SendEvent(
    MAVLINK_APP_INIT_INF_EID,
    CFE_EVS_EventType_INFORMATION,
    "MIDs: HK=0x%04X NAV=0x%04X",
    CFE_SB_MsgIdToValue(HkMsgId),
    CFE_SB_MsgIdToValue(NavMsgId));

        /*
         ** Create Software Bus message pipe.
         */
        status = CFE_SB_CreatePipe(&MAVLINK_APP_Data.CommandPipe,
                                   MAVLINK_APP_PLATFORM_PIPE_DEPTH,
                                   MAVLINK_APP_PLATFORM_PIPE_NAME);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(MAVLINK_APP_CR_PIPE_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "Mavlink App: Error creating SB Command Pipe, RC = 0x%08lX",
                              (unsigned long)status);
        }
    }

    if (status == CFE_SUCCESS) {
        if (MAVLINK_APP_StartUdp() != CFE_SUCCESS)
        {
            OS_printf("[MAVLINK_APP] UDP setup failed, continuing without it\n");
        }
    }

    if (status == CFE_SUCCESS)
    {
        /*
        ** Subscribe to Housekeeping request commands
        */
        status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(MAVLINK_APP_SEND_HK_MID), MAVLINK_APP_Data.CommandPipe);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(MAVLINK_APP_SUB_HK_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "Mavlink App: Error Subscribing to HK request, RC = 0x%08lX",
                              (unsigned long)status);
        }
    }

    if (status == CFE_SUCCESS)
    {
        /*
        ** Subscribe to ground command packets
        */
        status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(MAVLINK_APP_CMD_MID), MAVLINK_APP_Data.CommandPipe);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(MAVLINK_APP_SUB_CMD_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "Mavlink App: Error Subscribing to Commands, RC = 0x%08lX",
                              (unsigned long)status);
        }
    }

    if (status == CFE_SUCCESS)
    {
        /*
        ** Register Example Table(s)
        */
        status = CFE_TBL_Register(&MAVLINK_APP_Data.TblHandles[0],
                                  "ExampleTable",
                                  sizeof(MAVLINK_APP_ExampleTable_t),
                                  CFE_TBL_OPT_DEFAULT,
                                  MAVLINK_APP_TblValidationFunc);
        if (status != CFE_SUCCESS)
        {
            CFE_EVS_SendEvent(MAVLINK_APP_TABLE_REG_ERR_EID,
                              CFE_EVS_EventType_ERROR,
                              "Mavlink App: Error Registering Example Table, RC = 0x%08lX",
                              (unsigned long)status);
        }
        else
        {
            status = CFE_TBL_Load(MAVLINK_APP_Data.TblHandles[0], CFE_TBL_SRC_FILE, MAVLINK_APP_PLATFORM_TABLE_FILE);
        }

        CFE_Config_GetVersionString(VersionString,
                                    MAVLINK_APP_CFG_MAX_VERSION_STR_LEN,
                                    "Mavlink App",
                                    MAVLINK_APP_VERSION,
                                    MAVLINK_APP_BUILD_CODENAME,
                                    MAVLINK_APP_LAST_OFFICIAL);

        CFE_EVS_SendEvent(MAVLINK_APP_INIT_INF_EID,
                          CFE_EVS_EventType_INFORMATION,
                          "Mavlink App Initialized.%s",
                          VersionString);
    }

    return status;
}
