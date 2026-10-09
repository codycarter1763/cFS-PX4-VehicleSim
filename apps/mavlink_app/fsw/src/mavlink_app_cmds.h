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
 *   This file contains the prototypes for the Mavlink App Ground Command-handling functions
 */

#ifndef MAVLINK_APP_CMDS_H
#define MAVLINK_APP_CMDS_H

/*
** Required header files.
*/
#include "cfe_error.h"
#include "mavlink_app_msg.h"

CFE_Status_t MAVLINK_APP_SendHkCmd(const MAVLINK_APP_SendHkCmd_t *Msg);
CFE_Status_t MAVLINK_APP_NoopCmd(const MAVLINK_APP_NoopCmd_t *Msg);
CFE_Status_t MAVLINK_APP_ResetCountersCmd(const MAVLINK_APP_ResetCountersCmd_t *Msg);
CFE_Status_t MAVLINK_APP_ProcessCmd(const MAVLINK_APP_ProcessCmd_t *Msg);
CFE_Status_t MAVLINK_APP_DisplayParamCmd(const MAVLINK_APP_DisplayParamCmd_t *Msg);
CFE_Status_t MAVLINK_APP_StartMissionCmd(const MAVLINK_APP_StartMissionCmd_t *Msg);
CFE_Status_t MAVLINK_APP_InjectGPSFailureCmd(const MAVLINK_APP_InjectGPSFailureCmd_t *Msg);
CFE_Status_t MAVLINK_APP_RestoreGPSFailureCmd(const MAVLINK_APP_RestoreGPSFailureCmd_t *Msg);

#endif /* MAVLINK_APP_CMDS_H */
