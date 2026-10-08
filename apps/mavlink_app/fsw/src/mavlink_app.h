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
 * Main header file for the Mavlink application
 */

#ifndef MAVLINK_APP_H
#define MAVLINK_APP_H

/*
** Required header files.
*/
#include "cfe.h"
#include "cfe_config.h"

#include "mavlink_app_mission_cfg.h"
#include "mavlink_app_platform_cfg.h"

#include "mavlink_app_perfids.h"
#include "mavlink_app_msgids.h"



/************************************************************************
** Type Definitions
*************************************************************************/
typedef struct
{
    /* MAVLink parse state, written by the RX task */
    uint32 MsgCount;        /* complete, CRC-valid messages */
    uint32 ParseDrops;      /* packets the parser dropped (bad CRC, etc.) */
    uint32 HeartbeatCount;
    uint8  Armed;
    uint8  MainMode;        /* PX4 main mode, from custom_mode bits 16-23 */
    uint8  SubMode;         /* PX4 sub mode, from custom_mode bits 24-31 */
    uint8  SystemStatus;    /* MAV_STATE */
} MAVLINK_APP_ParseState_t;

typedef struct 
{
    uint8  CommandCounter;
    uint8  CommandErrorCounter;
    uint8  Armed;
    uint8  MainMode;
    uint8  SubMode;
    uint8  GpsFixType;
    uint8  GpsNumSatellites;
    int8   BatteryRemaining;
    float  BatteryVoltage;
    float  BatteryCurrent;
    uint32 HeartbeatCount;
} MAVLINK_APP_HkTlm_Payload_t;

typedef struct
{
    float  RollRad, PitchRad, YawRad;
    float  RollRateRadPs, PitchRateRadPs, YawRateRadPs;
    int32  LatDegE7;
    int32  LonDegE7;
    int32  RelativeAltitude;
    int16  VxCms, VyCms, VzCms;
    uint16 HeadingCDeg;
} MAVLINK_APP_NavTlm_Payload_t;  /* 44 bytes */

typedef struct
{
    uint8 Severity;
    uint8 TextLen;
    char  Text[50];
} MAVLINK_APP_Px4StatusText_Payload_t;

#include "mavlink_app_msg.h"

/*
** Global Data
*/
typedef struct
{
    /*
    ** Command interface counters...
    */
    uint8 CommandCounter;
    uint8 CommandErrorCounter;

    /*
    ** Housekeeping telemetry packet...
    */
    MAVLINK_APP_HkTlm_t HkTlm;
    MAVLINK_APP_NavTlm_t NavTlm;
    MAVLINK_APP_ParseState_t ParseState;
    MAVLINK_APP_Px4StatusText_Payload_t Px4StatusText;
    /*
    ** Run Status variable used in the main processing loop
    */
    uint32 RunStatus;

    /*
    ** Operational data (not reported in housekeeping)...
    */
    CFE_SB_PipeId_t CommandPipe;

    CFE_TBL_Handle_t TblHandles[MAVLINK_APP_PLATFORM_NUMBER_OF_TABLES];

    osal_id_t       SockId;
    OS_SockAddr_t   Px4Addr;
    CFE_ES_TaskId_t RxTaskId;
    uint32          RxPacketCount;
    uint32          RxByteCount;

    
} MAVLINK_APP_Data_t;

/*
** Global data structure
*/
extern MAVLINK_APP_Data_t MAVLINK_APP_Data;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (MAVLINK_APP_Main), these
**       functions are not called from any other source module.
*/
void         MAVLINK_APP_Main(void);
CFE_Status_t MAVLINK_APP_Init(void);

#endif /* MAVLINK_APP_H */
