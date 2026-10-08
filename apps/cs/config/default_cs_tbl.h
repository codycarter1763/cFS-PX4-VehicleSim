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
 *   Specification for the CFS Checksum tables.
 */
#ifndef DEFAULT_CS_TBL_H
#define DEFAULT_CS_TBL_H

/**************************************************************************
 **
 ** Include section
 **
 **************************************************************************/
#include "cs_tbldefs.h"

/**************************************************************************
 **
 ** Macro definitions
 **
 **************************************************************************/

/**
 * \brief table names for definition tables
 * \{
 */
#define CS_DEF_EEPROM_TABLE_NAME "DefEepromTbl"
#define CS_DEF_MEMORY_TABLE_NAME "DefMemoryTbl"
#define CS_DEF_TABLES_TABLE_NAME "DefTablesTbl"
#define CS_DEF_APP_TABLE_NAME    "DefAppTbl"
/**\}*/

/**
 * \brief names for the results tables
 * \{
 */
#define CS_RESULTS_EEPROM_TABLE_NAME "ResEepromTbl"
#define CS_RESULTS_MEMORY_TABLE_NAME "ResMemoryTbl"
#define CS_RESULTS_TABLES_TABLE_NAME "ResTablesTbl"
#define CS_RESULTS_APP_TABLE_NAME    "ResAppTbl"
/**\}*/

#endif
