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
 *   MAVLINK_APP Application Message IDs
 */
#ifndef DEFAULT_MAVLINK_APP_MSGIDS_H
#define DEFAULT_MAVLINK_APP_MSGIDS_H

#include "cfe_core_api_base_msgids.h"
#include "mavlink_app_msgid_values.h"
#include "mavlink_app_topicids.h"

#define MAVLINK_APP_CMD_MID     MAVLINK_APP_CMD_PLATFORM_MIDVAL(CMD)
#define MAVLINK_APP_SEND_HK_MID MAVLINK_APP_CMD_PLATFORM_MIDVAL(SEND_HK)
#define MAVLINK_APP_HK_TLM_MID  MAVLINK_APP_TLM_PLATFORM_MIDVAL(HK_TLM)
#define MAVLINK_APP_NAV_TLM_MID MAVLINK_APP_TLM_PLATFORM_MIDVAL(NAV_TLM)
#define MAVLINK_APP_MAVLINK_TLM_MID MAVLINK_APP_TLM_PLATFORM_MIDVAL(MAVLINK_TLM)
#define MAVLINK_APP_MISSION_CMD_MID     CFE_PLATFORM_CMD_TOPICID_TO_MIDV(MAVLINK_APP_MISSION_CMD_TOPICID)
#define MAVLINK_APP_MISSION_SEND_HK_MID CFE_PLATFORM_CMD_TOPICID_TO_MIDV(MAVLINK_APP_MISSION_SEND_HK_TOPICID)
#define MAVLINK_APP_MISSION_HK_TLM_MID  CFE_PLATFORM_TLM_TOPICID_TO_MIDV(MAVLINK_APP_MISSION_HK_TLM_TOPICID)
#define MAVLINK_APP_MISSION_NAV_TLM_MID CFE_PLATFORM_TLM_TOPICID_TO_MIDV(MAVLINK_APP_MISSION_NAV_TLM_TOPICID)
#define MAVLINK_APP_MISSION_MAVLINK_TLM_MID CFE_PLATFORM_TLM_TOPICID_TO_MIDV(MAVLINK_APP_MISSION_MAVLINK_TLM_TOPICID)
#endif
