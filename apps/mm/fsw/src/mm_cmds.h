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

#ifndef MM_CMDS_H
#define MM_CMDS_H

/* ======== */
/* Includes */
/* ======== */

#include "cfe.h"
#include "mm_msgstruct.h"

/* =================== */
/* Function Prototypes */
/* =================== */

/**
 * \brief Process housekeeping request
 *
 * \par Description
 *      Processes an on-board housekeeping request message.
 *
 * \par Assumptions, External Events, and Notes:
 *      This command does not affect the command execution counter
 *
 * \param[in] Msg Pointer to Send HK command struct
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_SendHkCmd(const MM_SendHkCmd_t *Msg);

/**
 * \brief Process noop command
 *
 * \par Description
 *      Processes a noop ground command.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Noop command struct
 *
 * \sa #MM_NOOP_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_NoopCmd(const MM_NoopCmd_t *Msg);

/**
 * \brief Process reset counters command
 *
 * \par Description
 *      Processes a reset counters ground command which will reset
 *      the memory manager commmand error and command execution counters
 *      to zero.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Reset Counters command struct
 *
 * \sa #MM_RESET_COUNTERS_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_ResetCountersCmd(const MM_ResetCountersCmd_t *Msg);

/**
 * \brief Process lookup symbol command
 *
 * \par Description
 *      Processes a lookup symbol ground command which takes a
 *      symbol name and tries to resolve it to an address using the
 *      #OS_SymbolLookup OSAL function.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Symbol Table Lookup command struct
 *
 * \sa #MM_LOOKUP_SYM_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_LookupSymCmd(const MM_LookupSymCmd_t *Msg);

/**
 * \brief Dump symbol table to file command
 *
 * \par Description
 *      Processes a dump symbol table to file ground command which calls
 *      the #OS_SymbolTableDump OSAL function using the specified dump
 *      file name.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Save Symbol Table To File command struct
 *
 * \sa #MM_SYM_TBL_TO_FILE_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_SymTblToFileCmd(const MM_SymTblToFileCmd_t *Msg);

/**
 * \brief Write-enable EEPROM command
 *
 * \par Description
 *      Processes an EEPROM write enable ground command which calls
 *      the #CFE_PSP_EepromWriteEnable cFE function using the specified
 *      bank number.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to EEPROM Write Enable command struct
 *
 * \sa #MM_EEPROM_WRITE_ENA_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_EepromWriteEnaCmd(const MM_EepromWriteEnaCmd_t *Msg);

/**
 * \brief Write-disable EEPROM command
 *
 * \par Description
 *      Processes an EEPROM write disable ground command which calls
 *      the #CFE_PSP_EepromWriteDisable cFE function using the specified
 *      bank number.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to EEPROM Write Disable command struct
 *
 * \sa #MM_EEPROM_WRITE_DIS_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_EepromWriteDisCmd(const MM_EepromWriteDisCmd_t *Msg);

/**
 * \brief Process memory poke command
 *
 * \par Description
 *      Processes the memory poke command that will load a memory
 *      location with data specified in the command message.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Memory Poke command struct
 *
 * \sa #MM_POKE_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_PokeCmd(const MM_PokeCmd_t *Msg);

/**
 * \brief Process load memory with interrupts disabled command
 *
 * \par Description
 *      Processes the load memory with interrupts disabled command
 *      that will load up to #MM_INTERFACE_MAX_UNINTERRUPTIBLE_DATA bytes into
 *      ram with interrupts disabled during the actual load
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Memory Load With Interrupts Disabled command struct
 *
 * \sa #MM_LOAD_MEM_WID_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_LoadMemWIDCmd(const MM_LoadMemWIDCmd_t *Msg);

/**
 * \brief Process memory load from file command
 *
 * \par Description
 *      Processes the memory load from file command that will read a
 *      file and store the data in the command specified address range
 *      of memory.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Memory Load From File command struct
 *
 * \sa #MM_LOAD_MEM_FROM_FILE_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_LoadMemFromFileCmd(const MM_LoadMemFromFileCmd_t *Msg);

/**
 * \brief Process memory fill command
 *
 * \par Description
 *      Processes the memory fill command that will load an address
 *      range of memory with the command specified fill pattern
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Memory Fill command struct
 *
 * \sa #MM_FILL_MEM_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_FillMemCmd(const MM_FillMemCmd_t *Msg);

/**
 * \brief Process memory peek command
 *
 * \par Description
 *      Processes the memory peek command that will read a memory
 *      location and report the data in an event message.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Memory Peek command struct
 *
 * \sa #MM_PEEK_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_PeekCmd(const MM_PeekCmd_t *Msg);

/**
 * \brief Process memory dump to file command
 *
 * \par Description
 *      Processes the memory dump to file command that will read a
 *      address range of memory and store the data in a command
 *      specified file.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Memory Dump To File command struct
 *
 * \sa #MM_DUMP_MEM_TO_FILE_CC
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_DumpMemToFileCmd(const MM_DumpMemToFileCmd_t *Msg);

/**
 * \brief Process memory dump in event command
 *
 * \par Description
 *      Processes the memory dump in event command that will read
 *      up to #MM_INTERNAL_MAX_DUMP_INEVENT_BYTES from memory and report
 *      the data in an event message.
 *
 * \par Assumptions, External Events, and Notes:
 *      None
 *
 * \param[in] Msg Pointer to Dump Memory In Event Message command struct
 *
 * \sa #MM_DUMP_IN_EVENT_CC, #MM_INTERNAL_MAX_DUMP_INEVENT_BYTES
 *
 * \return Exuection status
 * \retval CFE_SUCCESS: Command was successfully handled
 */
CFE_Status_t MM_DumpInEventCmd(const MM_DumpInEventCmd_t *Msg);

#endif /* MM_CMDS_H */