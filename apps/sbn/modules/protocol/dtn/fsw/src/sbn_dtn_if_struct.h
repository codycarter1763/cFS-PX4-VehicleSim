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

#ifndef _sbn_dtn_if_struct_h_
#define _sbn_dtn_if_struct_h_

#include "sbn_constants.h"
#include "sbn_interfaces.h"
#include "sbn_platform_cfg.h"
#include "cfe.h"
#include <bp.h>

typedef struct
{
    char    EIN[32];
    boolean SubsSent;
} SBN_DTN_Peer_t;

typedef struct
{
    char         EIN[32];
    BpSAP        SAP;
    Sdr          SendSDR, RecvSDR;
    ReqAttendant Attendant;
} SBN_DTN_Net_t;

#endif /* _sbn_dtn_if_struct_h_ */
