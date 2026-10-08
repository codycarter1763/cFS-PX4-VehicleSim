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
 *   Specification for the CFS Checksum constants for message IDs
 */
#ifndef CS_TOPICIDS_H
#define CS_TOPICIDS_H

#include "cs_topicid_values.h"

/**
 * \defgroup cfscscmdtid CFS Checksum Command Topic IDs
 * \{
 */

#define CFE_MISSION_CS_CMD_TOPICID                      CFE_MISSION_CS_TIDVAL(CMD)
#define DEFAULT_CFE_MISSION_CS_CMD_TOPICID              0x9F /**< \brief CS Command Topic ID */
#define CFE_MISSION_CS_SEND_HK_TOPICID                  CFE_MISSION_CS_TIDVAL(SEND_HK)
#define DEFAULT_CFE_MISSION_CS_SEND_HK_TOPICID          0xA0 /**< \brief CS Housekeeping Request Topic ID */
#define CFE_MISSION_CS_BACKGROUND_CYCLE_TOPICID         CFE_MISSION_CS_TIDVAL(BACKGROUND_CYCLE)
#define DEFAULT_CFE_MISSION_CS_BACKGROUND_CYCLE_TOPICID 0xA1 /**< \brief CS Background Cycle Topic ID */

/**\}*/

/**
 * \defgroup cfscstlmtid CFS Checksum Telemetry Topic IDs
 * \{
 */

#define CFE_MISSION_CS_HK_TLM_TOPICID         CFE_MISSION_CS_TIDVAL(HK_TLM)
#define DEFAULT_CFE_MISSION_CS_HK_TLM_TOPICID 0xA4 /**< \brief CS Housekeeping Telemetry Topic ID */

/**\}*/

#endif
