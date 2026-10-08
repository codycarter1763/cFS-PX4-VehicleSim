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
 *   TO_LAB Application Topic IDs
 */
#ifndef TO_LAB_TOPICIDS_H
#define TO_LAB_TOPICIDS_H

#include "to_lab_topicid_values.h"

#define TO_LAB_MISSION_CMD_TOPICID         TO_LAB_MISSION_TIDVAL(CMD)
#define DEFAULT_TO_LAB_MISSION_CMD_TOPICID 0x80

#define TO_LAB_MISSION_SEND_HK_TOPICID         TO_LAB_MISSION_TIDVAL(SEND_HK)
#define DEFAULT_TO_LAB_MISSION_SEND_HK_TOPICID 0x81

#define TO_LAB_MISSION_HK_TLM_TOPICID         TO_LAB_MISSION_TIDVAL(HK_TLM)
#define DEFAULT_TO_LAB_MISSION_HK_TLM_TOPICID 0x80

#define TO_LAB_MISSION_DATA_TYPES_TOPICID         TO_LAB_MISSION_TIDVAL(DATA_TYPES)
#define DEFAULT_TO_LAB_MISSION_DATA_TYPES_TOPICID 0x81


#endif
