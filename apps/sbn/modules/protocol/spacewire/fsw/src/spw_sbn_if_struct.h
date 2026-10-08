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

#ifndef _spw_sbn_if_struct_h_
#define _spw_sbn_if_struct_h_

/* Since the target implementation of the SpaceWire driver only operates point-to-point, this struct is a bit sparse
** The only necessary data is the SpaceWire device class and device name
*/
typedef struct
{
    char DevClass[SBN_SPW_MAX_CHAR_NAME];    /* e.g. 'spw' from /sys/class */
    char DevInstance[SBN_SPW_MAX_CHAR_NAME]; /* e.g. 'spw0' from /dev */
} SPW_SBNEntry_t;

typedef struct
{
    SPW_SBNEntry_t *spwEntry;
} SPW_SBNHostData_t;

typedef struct
{
    SPW_SBNEntry_t *spwEntry;
} SPW_SBNPeerData_t;

#endif /* _spw_sbn_if_struct_h_ */
