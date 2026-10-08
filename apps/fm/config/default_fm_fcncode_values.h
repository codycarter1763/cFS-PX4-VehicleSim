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
 *   Specification for the CFS File Manager (FM) command function codes
 *
 * @note
 *   This file should be strictly limited to the command/function code (CC)
 *   macro definitions.  Other definitions such as enums, typedefs, or other
 *   macros should be placed in the msgdefs.h or msg.h files.
 */

#ifndef DEFAULT_FM_FCNCODE_VALUES_H
#define DEFAULT_FM_FCNCODE_VALUES_H

/* ====== */
/* Macros */
/* ====== */

#define FM_CCVAL(x) FM_FunctionCode_##x

/* ================ */
/* Type Definitions */
/* ================ */

enum FM_FunctionCode
{
    FM_FunctionCode_NOOP                     = 0,
    FM_FunctionCode_RESET_COUNTERS           = 1,
    FM_FunctionCode_COPY_FILE                = 2,
    FM_FunctionCode_MOVE_FILE                = 3,
    FM_FunctionCode_RENAME_FILE              = 4,
    FM_FunctionCode_DELETE_FILE              = 5,
    FM_FunctionCode_DELETE_ALL_FILES         = 7,
    FM_FunctionCode_DECOMPRESS_FILE          = 8,
    FM_FunctionCode_CONCAT_FILES             = 9,
    FM_FunctionCode_GET_FILE_INFO            = 10,
    FM_FunctionCode_GET_OPEN_FILES           = 11,
    FM_FunctionCode_CREATE_DIRECTORY         = 12,
    FM_FunctionCode_DELETE_DIRECTORY         = 13,
    FM_FunctionCode_GET_DIR_LIST_FILE        = 14,
    FM_FunctionCode_GET_DIR_LIST_PKT         = 15,
    FM_FunctionCode_MONITOR_FILESYSTEM_SPACE = 16,
    FM_FunctionCode_SET_TABLE_STATE          = 17,
    FM_FunctionCode_SET_PERMISSIONS          = 19,
};

#endif /* DEFAULT_FM_FCNCODE_VALUES_H */