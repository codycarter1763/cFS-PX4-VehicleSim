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
 *   Unit specification for the Core Flight System (CFS)
 *   Checksum (CS) Application.
 */
#ifndef CS_DISPATCH_H
#define CS_DISPATCH_H

#include "cfe_sb_api_typedefs.h"

/**************************************************************************
**
** Include section
**
**************************************************************************/

/**
 * \brief Process a command pipe message
 *
 *  \par Description
 *       Processes a single software bus command pipe message. Checks
 *       the message and command IDs and calls the appropriate routine
 *       to handle the command.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param [in]   BufPtr   A #CFE_SB_Buffer_t* pointer that
 *                         references the software bus message.  The
 *                         calling function verifies that BufPtr is
 *                         non-null.
 *
 * \return Execution status, see \ref CFEReturnCodes
 * \retval #CFE_SUCCESS \copybrief CFE_SUCCESS
 */
CFE_Status_t CS_AppPipe(const CFE_SB_Buffer_t *BufPtr);

/**
 * \brief Command packet processor
 *
 * \par Description
 *      Processes all CS commands
 *
 * \param [in] BufPtr A CFE_SB_Buffer_t* pointer that
 *                    references the software bus pointer. The
 *                    BufPtr is verified non-null in CS_AppMain.
 */
void CS_ProcessCmd(const CFE_SB_Buffer_t *BufPtr);

#endif
