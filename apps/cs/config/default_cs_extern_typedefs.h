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
 *   Specification for the CFS Checksum command and telemetry
 *   messages.
 */
#ifndef DEFAULT_CS_EXTERN_TYPEDEFS_H
#define DEFAULT_CS_EXTERN_TYPEDEFS_H

/**
 * \name CS Checksum Type Number Definitions
 * \{
 */

typedef enum
{
    CS_ChecksumType_CFECORE      = 0, /**< \brief cFE Core checksum */
    CS_ChecksumType_OSCORE       = 1, /**< \brief OS Core checksum */
    CS_ChecksumType_EEPROM_TABLE = 2, /**< \brief EEPROM checksum */
    CS_ChecksumType_MEMORY_TABLE = 3, /**< \brief Memory checksum */
    CS_ChecksumType_TABLES_TABLE = 4, /**< \brief Tables checksum */
    CS_ChecksumType_APP_TABLE    = 5  /**< \brief App checksum */
} CS_ChecksumType_Enum_t;

/* Compatibility macros (e.g. existing table definitions) */
#define CS_CFECORE      CS_ChecksumType_CFECORE
#define CS_OSCORE       CS_ChecksumType_OSCORE
#define CS_EEPROM_TABLE CS_ChecksumType_EEPROM_TABLE
#define CS_MEMORY_TABLE CS_ChecksumType_MEMORY_TABLE
#define CS_TABLES_TABLE CS_ChecksumType_TABLES_TABLE
#define CS_APP_TABLE    CS_ChecksumType_APP_TABLE

#define CS_NUM_TABLES 6 /**< \brief Number of checksum types*/

/**\}*/

/**
 * \name CS Checkum States
 * \{
 */
typedef enum
{
    CS_ChecksumState_EMPTY     = 0x00, /**< \brief Entry unused */
    CS_ChecksumState_ENABLED   = 0x01, /**< \brief Entry or table enabled */
    CS_ChecksumState_DISABLED  = 0x02, /**< \brief Entry or table disabled */
    CS_ChecksumState_UNDEFINED = 0x03  /**< \brief Entry not found state undefined */
} CS_ChecksumState_Enum_t;

/* Compatibility macros (e.g. existing table definitions) */
#define CS_STATE_EMPTY     CS_ChecksumState_EMPTY
#define CS_STATE_ENABLED   CS_ChecksumState_ENABLED
#define CS_STATE_DISABLED  CS_ChecksumState_DISABLED
#define CS_STATE_UNDEFINED CS_ChecksumState_UNDEFINED

/**\}*/

#endif
