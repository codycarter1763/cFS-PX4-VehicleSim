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
 *   Unit specification for the CFS File Manager Application.
 */
#ifndef FM_DISPATCH_H
#define FM_DISPATCH_H

#include "cfe.h"
#include "fm_msg.h"

/**
 *  \brief Verify Command Packet Length Function
 *
 *  \par Description
 *       This function is invoked from each of the command handlers to verify the
 *       length of the command packet.
 *
 *  \par Assumptions, External Events, and Notes:
 *
 *  \param [in]  MsgPtr         Pointer to Message
 *  \param [in]  ExpectedLength Expected packet length (command specific)
 *
 *  \return Boolean valid packet length response
 *  \retval true  Packet length valid
 *  \retval false Packet length invalid
 */
bool FM_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength);

/**
 *  \brief Process Input Command Packets
 *
 *  \par Description
 *
 *       Branch to appropriate input packet handler: HK request or FM commands.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \param [in] BufPtr Pointer to Software Bus message buffer.
 *
 *  \sa #FM_SendHkCmd, #FM_ProcessGroundCommand
 */
void FM_TaskPipe(const CFE_SB_Buffer_t *BufPtr);

/**
 *  \brief Process FM Ground Commands
 *
 *  \par Description
 *
 *       Branch to the command specific handlers for FM ground commands.
 *
 *  \par Assumptions, External Events, and Notes: None
 *
 *  \param [in]  BufPtr Pointer to Software Bus message buffer.
 */
void FM_ProcessGroundCommand(const CFE_SB_Buffer_t *BufPtr);

#endif
