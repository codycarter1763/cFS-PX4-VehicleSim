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
#ifndef SCH_LAB_INTERNAL_CFG_H
#define SCH_LAB_INTERNAL_CFG_H

#include "sch_lab_mission_cfg.h"
#include "sch_lab_internal_cfg_values.h"

/*
 * There is no extra encapsulation here, this header only
 * defines the default file name to use for the SCH table
 */
#define SCH_LAB_PLATFORM_TBL_DEFAULT_FILE         SCH_LAB_PLATFORM_CFGVAL(TBL_DEFAULT_FILE)
#define DEFAULT_SCH_LAB_PLATFORM_TBL_DEFAULT_FILE "/cf/sch_lab_table.tbl"

#endif /* SCH_LAB_INTERFACE_CFG_H */
