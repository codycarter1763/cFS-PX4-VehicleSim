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

#ifndef _sbn_serial_if_struct_h_
#define _sbn_serial_if_struct_h_

#include "sbn_constants.h"
#include "sbn_interfaces.h"
#include "sbn_serial_platform_cfg.h"
#include "sbn_platform_cfg.h"
#include "cfe.h"

typedef struct
{
    char Filename[SBN_SERIAL_MAX_CHAR_NAME];
    int  FD;
    /* 0 = this peer connects to me, 1 = I connect to this peer */
    uint8 /** flags */
        /** \brief recv the header first */
        ReceivingBody,
        /** \brief am I connected to the serial device? */
        SerialConn, BufNum;
    int RecvSz;

    /** See SBN_SERIAL_CONNTRY_TIME. */
    OS_time_t LastConnTry;
} SBN_SERIAL_Peer_t;

#endif /* _sbn_serial_if_struct_h_ */
