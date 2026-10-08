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
 *   Specification for the CFS Checksum macro constants that can
 *   be configured from one mission to another
 */
#ifndef CS_INTERFACE_CFG_H
#define CS_INTERFACE_CFG_H

#include "cs_interface_cfg_values.h"

/**
 * \defgroup cfscsmissioncfg CFS Checksum Mission Configuration
 * \{
 */

/**
 * \brief default CRC algorithm
 *
 *  \par  Description:
 *        This parameter is the algorithm used by CS to checksum
 *        the requested data.
 *
 *  \par Limits:
 *         This parameter is limited to either #CFE_MISSION_ES_DEFAULT_CRC,
 *         or  #CFE_ES_CrcType_CRC_16
 */
#define CS_DEFAULT_ALGORITHM                   CS_INTERFACE_CFGVAL(DEFAULT_ALGORITHM)
#define DEFAULT_CS_INTERFACE_DEFAULT_ALGORITHM CFE_MISSION_ES_DEFAULT_CRC

/**
 * \brief  Maximum number of entries in the EEPROM table to checksum
 *
 *  \par  Description:
 *        Maximum number of entries that can be in the table of
 *        EEPROM areas to checksum.
 *
 *  \par Limits:
 *     This parameter is limited by the  uint16 datatype that defines it.
 *     This parameter is limited to 65535.
 */
#define CS_MAX_NUM_EEPROM_TABLE_ENTRIES                   CS_INTERFACE_CFGVAL(MAX_NUM_EEPROM_TABLE_ENTRIES)
#define DEFAULT_CS_INTERFACE_MAX_NUM_EEPROM_TABLE_ENTRIES 16

/**
 * \brief  Maximum number of entries in the Memory table to checksum
 *
 *  \par  Description:
 *        Maximum number of entries that can be in the table of
 *        Memory areas to checksum.
 *
 *  \par Limits:
 *     This parameter is limited by the  uint16 datatype that defines it.
 *     This parameter is limited to 65535.
 */
#define CS_MAX_NUM_MEMORY_TABLE_ENTRIES                   CS_INTERFACE_CFGVAL(MAX_NUM_MEMORY_TABLE_ENTRIES)
#define DEFAULT_CS_INTERFACE_MAX_NUM_MEMORY_TABLE_ENTRIES 16

/**
 * \brief Maximum number of tables to checksum
 *
 *  \par  Description:
 *        Maximum number of entries in the table of tables to checksum
 *
 *  \par Limits:
 *       This parameter is limited by the maximum number of tables allowed
 *       in the system. This parameter is limited to #CFE_PLATFORM_TBL_MAX_NUM_TABLES
 */
#define CS_MAX_NUM_TABLES_TABLE_ENTRIES                   CS_INTERFACE_CFGVAL(MAX_NUM_TABLES_TABLE_ENTRIES)
#define DEFAULT_CS_INTERFACE_MAX_NUM_TABLES_TABLE_ENTRIES 24

/**
 * \brief Maximum number of applications to checksum
 *
 *  \par  Description:
 *        Maximum number of entries in the table of applications to checksum
 *
 *  \par Limits:
 *       This parameter is limited by the maximum number of applications allowed
 *       in the system. This parameter is limited to #CFE_PLATFORM_ES_MAX_APPLICATIONS
 */
#define CS_MAX_NUM_APP_TABLE_ENTRIES                   CS_INTERFACE_CFGVAL(MAX_NUM_APP_TABLE_ENTRIES)
#define DEFAULT_CS_INTERFACE_MAX_NUM_APP_TABLE_ENTRIES 24

/**\}*/

#endif
