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
 *   CFS Stored Command (SC) sample RTS table 005
 *
 * The following source code demonstrates how to create a sample
 * Stored Command RTS table using the software defined command structures.
 * It's also possible to create this table via alternative tools
 * (ground system) and or system agnostic data definitions (XTCE/EDS/JSON).
 *
 * This source file creates a sample RTS table that contains only
 * the following commands that are scheduled as follows:
 *
 * SC NOOP command, execution wakeup count relative to start of RTS = 0
 * SC Enable RTS #2 command, execution wakeup count relative to prev cmd = 5
 * SC Start RTS #2 command, execution wakeup count relative to prev cmd = 5
 */

#include "cfe.h"
#include "cfe_tbl_filedef.h"

#include "sc_tbldefs.h"      /* defines SC table headers */
#include "sc_platform_cfg.h" /* defines table buffer size */
#include "sc_msgdefs.h"      /* defines SC command code values */
#include "sc_msgids.h"       /* defines SC packet msg ID's */
#include "sc_msg.h"          /* defines SC message structures */

#include "mavlink_app_msgids.h" /* defines MAVLINK_APP packet msg ID's */
#include "mavlink_app_fcncodes.h" /* defines MAVLINK_APP command code values */

/*
 * Command checksum = 0xFF ^ (XOR of the other command bytes).
 * For an 8-byte command (no payload) the fixed bytes XOR to 0xC0 ^ 0x01 (sequence and
 * length), plus the function code. With MAVLINK_APP_INJECT_GPS_FAILURE_CC = 5:
 * 0xFF ^ 0xC0 ^ 0x01 ^ 0x05 = 0x3A. If the function code changes, recompute.
 */
#ifndef MAVLINK_RESTORE_GPS_FAILURE_CKSUM
#define MAVLINK_RESTORE_GPS_FAILURE_CKSUM \
    (0x38 ^ ((MAVLINK_APP_CMD_MID & 0xFF00) >> 8u) ^ (MAVLINK_APP_CMD_MID & 0x00FF))
#endif



/* Custom table structure, modify as needed to add desired commands */
typedef struct
{
    SC_RtsEntryHeader_t hdr1;
    SC_NoopCmd_t        cmd1;
} SC_RtsStruct005_t;

/* Define the union to size the table correctly */
typedef union
{
    SC_RtsStruct005_t rts;
    uint16            buf[SC_RTS_BUFF_SIZE];
} SC_RtsTable005_t;

/* Helper macro to get size of structure elements */
#define SC_MEMBER_SIZE(member) (sizeof(((SC_RtsStruct005_t *)NULL)->member))

/* Used designated initializers to be verbose, modify as needed/desired */
SC_RtsTable005_t SC_Rts005 = {
    /* 1 */
    .rts.hdr1.WakeupCount = 0,
    .rts.cmd1             = { CFE_MSG_CMD_HDR_INIT(MAVLINK_APP_CMD_MID, SC_MEMBER_SIZE(cmd1), MAVLINK_APP_RESTORE_GPS_FAILURE_CC, MAVLINK_RESTORE_GPS_FAILURE_CKSUM) }
};

/* Macro for table structure */
CFE_TBL_FILEDEF(SC_Rts005, SC.RTS_TBL005, SC Example RTS_TBL005, sc_rts005.tbl)

/************************/
/*  End of File Comment */
/************************/
