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
 * This file contains several user-configurable parameters
 *
 * @author Jaclyn Beck
 * @date 2015/06/24 15:30:00
 */
#ifndef _serial_platform_cfg_h_
#define _serial_platform_cfg_h_

#define SBN_SERIAL_MAX_CHAR_NAME 32 /**< How long the device name can be in the SbnPeerData file */

#define SBN_SERIAL_CHILD_STACK_SIZE 2048 /**< Stack size that each child task gets */

#define SBN_SERIAL_CHILD_TASK_PRIORITY 70 /**< Priority of the child tasks */

#define SBN_SERIAL_PEER_HEARTBEAT 5
#define SBN_SERIAL_PEER_TIMEOUT   10
#endif
