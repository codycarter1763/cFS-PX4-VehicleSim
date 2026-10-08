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
 *   Specification for the CFE Executive Services (CFE_ES) command function codes
 *
 * @note
 *   This file should be strictly limited to the command/function code (CC)
 *   macro definitions.  Other definitions such as enums, typedefs, or other
 *   macros should be placed in the msgdefs.h or msg.h files.
 */
#ifndef DEFAULT_TO_LAB_FCNCODE_VALUES_H
#define DEFAULT_TO_LAB_FCNCODE_VALUES_H

/************************************************************************
 * Macro Definitions
 ************************************************************************/

#define TO_LAB_CCVAL(x) TO_LAB_FunctionCode_##x

enum TO_LAB_FunctionCode_
{
    TO_LAB_FunctionCode_NOOP            = 0,
    TO_LAB_FunctionCode_RESET_STATUS    = 1,
    TO_LAB_FunctionCode_ADD_PKT         = 2,
    TO_LAB_FunctionCode_SEND_DATA_TYPES = 3,
    TO_LAB_FunctionCode_REMOVE_PKT      = 4,
    TO_LAB_FunctionCode_REMOVE_ALL_PKT  = 5,
    TO_LAB_FunctionCode_OUTPUT_ENABLE   = 6,
};

#endif
