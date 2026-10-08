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
#ifndef DEFAULT_CS_MSGIDS_H
#define DEFAULT_CS_MSGIDS_H

#include "cs_msgid_values.h"

/**
 * \defgroup cfscscmdmid CFS Checksum Command Message IDs
 * \{
 */

#define CS_CMD_MID              CFE_PLATFORM_CS_CMD_MIDVAL(CMD)
#define CS_SEND_HK_MID          CFE_PLATFORM_CS_CMD_MIDVAL(SEND_HK)
#define CS_BACKGROUND_CYCLE_MID CFE_PLATFORM_CS_CMD_MIDVAL(BACKGROUND_CYCLE)

/**\}*/

/**
 * \defgroup cfscstlmmid CFS Checksum Telemetry Message IDs
 * \{
 */

#define CS_HK_TLM_MID CFE_PLATFORM_CS_TLM_MIDVAL(HK_TLM)

/**\}*/

#endif
