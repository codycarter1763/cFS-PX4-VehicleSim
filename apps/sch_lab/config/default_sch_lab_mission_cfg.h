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
 * CFS SCH_LAB Application Mission Configuration Header File
 *
 * This is a compatibility header for the "mission_cfg.h" file that has
 * traditionally provided public config definitions for each CFS app.
 *
 * @note This file may be overridden/superceded by mission-provided definitions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */
#ifndef DEFAULT_SCH_LAB_MISSION_CFG_H
#define DEFAULT_SCH_LAB_MISSION_CFG_H

#include "sch_lab_interface_cfg_values.h"

/**
 * @brief The maximum number of schedule table entries
 */
#define SCH_LAB_MISSION_MAX_SCHEDULE_ENTRIES         SCH_LAB_MISSION_CFGVAL(MAX_SCHEDULE_ENTRIES)
#define DEFAULT_SCH_LAB_MISSION_MAX_SCHEDULE_ENTRIES 32

/**
 * @brief The maximum number of arguments to each schedule message entry
 *
 * This is allocated in units of 16 bit words.
 */
#define SCH_LAB_MISSION_MAX_ARGS_PER_ENTRY         SCH_LAB_MISSION_CFGVAL(MAX_ARGS_PER_ENTRY)
#define DEFAULT_SCH_LAB_MISSION_MAX_ARGS_PER_ENTRY 32

#endif /* DEFAULT_SCH_LAB_MISSION_CFG_H */
