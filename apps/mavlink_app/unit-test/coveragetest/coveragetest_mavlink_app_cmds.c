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

/*
** File: coveragetest_mavlink_app.c
**
** Purpose:
** Coverage Unit Test cases for the MAVLINK Application
**
** Notes:
** This implements various test cases to exercise all code
** paths through all functions defined in the MAVLINK application.
**
** It is primarily focused at providing examples of the various
** stub configurations, hook functions, and wrapper calls that
** are often needed when coercing certain code paths through
** complex functions.
*/

/*
 * Includes
 */

#include "sample_lib.h" /* For SAMPLE_LIB_Function */
#include "mavlink_app_coveragetest_common.h"
#include "mavlink_app.h"
#include "mavlink_app_dispatch.h"
#include "mavlink_app_cmds.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
#include <common/mavlink.h>
#pragma GCC diagnostic pop
/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

void Test_MAVLINK_APP_ReportHousekeeping(void)
{
    /*
     * Test Case For:
     * void MAVLINK_APP_ReportHousekeeping( const CFE_SB_CmdHdr_t *Msg )
     */
    CFE_MSG_Message_t *MsgSend;
    CFE_MSG_Message_t *MsgTimestamp;

    /* Set up to capture send message address */
    UT_SetDataBuffer(UT_KEY(CFE_SB_TransmitMsg), &MsgSend, sizeof(MsgSend), false);

    /* Set up to capture timestamp message address */
    UT_SetDataBuffer(UT_KEY(CFE_SB_TimeStampMsg), &MsgTimestamp, sizeof(MsgTimestamp), false);

    /* Call unit under test, NULL pointer confirms command access is through APIs */
    MAVLINK_APP_SendHkCmd(NULL);

    /* Confirm message sent*/
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 1);
    UtAssert_ADDRESS_EQ(MsgSend, &MAVLINK_APP_Data.HkTlm);

    /* Confirm timestamp msg address */
    UtAssert_STUB_COUNT(CFE_SB_TimeStampMsg, 1);
    UtAssert_ADDRESS_EQ(MsgTimestamp, &MAVLINK_APP_Data.HkTlm);

    /*
     * Confirm that the CFE_TBL_Manage() call was done
     */
    UtAssert_STUB_COUNT(CFE_TBL_Manage, 1);
}

void Test_MAVLINK_APP_NoopCmd(void)
{
    /*
     * Test Case For:
     * void MAVLINK_APP_NoopCmd( const MAVLINK_APP_Noop_t *Msg )
     */
    MAVLINK_APP_NoopCmd_t TestMsg;
    UT_CheckEvent_t      EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    /* test dispatch of NOOP */
    UT_CHECKEVENT_SETUP(&EventTest, MAVLINK_APP_NOOP_INF_EID, NULL);

    UtAssert_INT32_EQ(MAVLINK_APP_NoopCmd(&TestMsg), CFE_SUCCESS);

    /*
     * Confirm that the event was generated
     */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

void Test_MAVLINK_APP_ResetCountersCmd(void)
{
    /*
     * Test Case For:
     * void MAVLINK_APP_ResetCounters( const MAVLINK_APP_ResetCounters_t *Msg )
     */
    MAVLINK_APP_ResetCountersCmd_t TestMsg;
    UT_CheckEvent_t               EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, MAVLINK_APP_RESET_INF_EID, "MAVLINK: RESET command");

    UtAssert_INT32_EQ(MAVLINK_APP_ResetCountersCmd(&TestMsg), CFE_SUCCESS);

    /*
     * Confirm that the event was generated
     */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

void Test_MAVLINK_APP_ProcessCmd(void)
{
    /*
     * Test Case For:
     * void  MAVLINK_APP_ProcessCmd( const MAVLINK_APP_ProcessCmd_t *Msg )
     */
    MAVLINK_APP_ProcessCmd_t   TestMsg;
    MAVLINK_APP_ExampleTable_t TestTblData;
    void                     *TblPtr = &TestTblData;

    memset(&TestTblData, 0, sizeof(TestTblData));
    memset(&TestMsg, 0, sizeof(TestMsg));

    /* Provide some table data for the MAVLINK_APP_Process() function to use */
    TestTblData.Int1 = 40;
    TestTblData.Int2 = 50;
    UT_SetDataBuffer(UT_KEY(CFE_TBL_GetAddress), &TblPtr, sizeof(TblPtr), false);
    UtAssert_INT32_EQ(MAVLINK_APP_ProcessCmd(&TestMsg), CFE_SUCCESS);

    /*
     * This only needs to account for the call to CFE_ES_WriteToSysLog() directly
     * invoked by the unit under test.   Note that in this build environment, the
     * MAVLINK_APP_GetCrc() function is a stub.
     */
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 1);

    /*
     * Confirm that the CFE_TBL_GetAddress() call was done
     */
    UtAssert_STUB_COUNT(CFE_TBL_GetAddress, 1);

    /*
     * Confirm that the SAMPLE_LIB_Function() call was done
     * NOTE: This stub is provided by the sample_lib library
     */
    UtAssert_STUB_COUNT(SAMPLE_LIB_Function, 1);

    /*
     * Configure the CFE_TBL_GetAddress function to return an error.
     * Exercise the error return path.
     * Error at this point should add only one additional call to
     * CFE_ES_WriteToSysLog() through the CFE_TBL_GetAddress() error path.
     */
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_TBL_ERR_UNREGISTERED);
    UtAssert_INT32_EQ(MAVLINK_APP_ProcessCmd(&TestMsg), CFE_TBL_ERR_UNREGISTERED);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 2);

    /*
     * Configure CFE_TBL_ReleaseAddress() to return an error, exercising the
     * error return path.
     * Confirm two additional calls to CFE_ES_WriteToSysLog() - one
     * reporting the table values, and one through the CFE_TBL_ReleaseAddress()
     * error path.
     */
    UT_SetDataBuffer(UT_KEY(CFE_TBL_GetAddress), &TblPtr, sizeof(TblPtr), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_GetAddress), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_TBL_ReleaseAddress), CFE_TBL_ERR_NO_ACCESS);
    UtAssert_INT32_EQ(MAVLINK_APP_ProcessCmd(&TestMsg), CFE_TBL_ERR_NO_ACCESS);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 4);
}

void Test_MAVLINK_APP_DisplayParamCmd(void)
{
    /*
     * Test Case For:
     * void  MAVLINK_APP_DisplayParamCmd( const MAVLINK_APP_DisplayParamCmd_t *Msg )
     */
    MAVLINK_APP_DisplayParamCmd_t TestMsg;
    UT_CheckEvent_t              EventTest;

    memset(&TestMsg, 0, sizeof(TestMsg));

    UT_CHECKEVENT_SETUP(&EventTest, MAVLINK_APP_VALUE_INF_EID, "MAVLINK_APP: ValU32=%lu, ValI16=%d, ValStr=%s");
    TestMsg.Payload.ValU32 = 10;
    TestMsg.Payload.ValI16 = -4;
    snprintf(TestMsg.Payload.ValStr, sizeof(TestMsg.Payload.ValStr), "Hello");

    UtAssert_INT32_EQ(MAVLINK_APP_DisplayParamCmd(&TestMsg), CFE_SUCCESS);
    /*
     * Confirm that the event was generated
     */
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}

void Test_MAVLINK_APP_StartMissionCmd(void)
{
    MAVLINK_APP_StartMissionCmd_t TestMsg;
    memset(&TestMsg, 0, sizeof(TestMsg));

    /* nominal: send succeeds */
    UtAssert_INT32_EQ(MAVLINK_APP_StartMissionCmd(&TestMsg), CFE_SUCCESS);

    /* Error socket send fails */
    UT_SetDeferredRetcode(UT_KEY(OS_SocketSendTo), 1, OS_ERROR);
    UtAssert_INT32_EQ(MAVLINK_APP_StartMissionCmd(&TestMsg), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);
}

static uint8  UT_SentBuf[MAVLINK_MAX_PACKET_LEN];
static size_t UT_SentLen;

static void UT_CaptureSendTo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    const void *Buffer = UT_Hook_GetArgValueByName(Context, "buffer", const void *);
    size_t      BufLen = UT_Hook_GetArgValueByName(Context, "buflen", size_t);
    int32       Status = 0;

    UT_SentLen = (BufLen > sizeof(UT_SentBuf)) ? sizeof(UT_SentBuf) : BufLen;
    memcpy(UT_SentBuf, Buffer, UT_SentLen);

    UT_Stub_GetInt32StatusCode(Context, &Status);
    UT_Stub_SetReturnValue(FuncKey, Status);
}

void Test_MAVLINK_APP_InjectGPSFaultCmd(void)
{
    UT_CheckEvent_t EventTest;
    mavlink_command_long_t Cmd;
    MAVLINK_APP_InjectGPSFailureCmd_t Msg;
    mavlink_message_t MavMsg;
    mavlink_status_t MavStatus;
    bool Decoded = false;
    size_t i;

    memset(&MAVLINK_APP_Data, 0, sizeof(MAVLINK_APP_Data)); 
    memset(&Cmd, 0, sizeof(Cmd));
    memset(&Msg, 0, sizeof(Msg));
    memset(&MavStatus, 0, sizeof(MavStatus));
    UT_SentLen = 0;

    /* Nomimnal: Correct InjectGPSFault Command */
    UT_CHECKEVENT_SETUP(&EventTest, MAVLINK_APP_INJECT_GPS_FAILURE_INF_EID, NULL);
    UT_SetHandlerFunction(UT_KEY(OS_SocketSendTo), UT_CaptureSendTo, NULL);

    UtAssert_INT32_EQ(MAVLINK_APP_InjectGPSFailureCmd(&Msg), CFE_SUCCESS);
    UtAssert_UINT8_EQ(MAVLINK_APP_Data.CommandCounter, 1);
    UtAssert_UINT8_EQ(MAVLINK_APP_Data.CommandErrorCounter, 0);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(OS_SocketSendTo, 1);

    for (i = 0; i < UT_SentLen; i++) {
        if (mavlink_parse_char(MAVLINK_COMM_1, UT_SentBuf[i], &MavMsg, &MavStatus) && MavMsg.msgid == MAVLINK_MSG_ID_COMMAND_LONG) {
            mavlink_msg_command_long_decode(&MavMsg, &Cmd);
            Decoded = true;
            break;
        }
    }
        UtAssert_True(Decoded, "Sent bytes decoded as MAVLink COMMAND_LONG");
        UtAssert_UINT16_EQ(Cmd.command, MAV_CMD_INJECT_FAILURE);
        UtAssert_UINT8_EQ(Cmd.target_system, 1);
        UtAssert_UINT8_EQ(Cmd.target_component, 1);
        UtAssert_True(Cmd.param1 == 4.0f, "param1 is FAIILURE_UNIT_SENSOR_GPS (4), got %f", (double)Cmd.param1);
        UtAssert_True(Cmd.param2 == 1.0f, "param2 is FAILURE_TYPE_OFF (1), got %f", (double)Cmd.param2);

        /* Error: socket send fails */
        UT_CHECKEVENT_SETUP(&EventTest, MAVLINK_APP_INJECT_GPS_FAILURE_ERR_EID, NULL);
        UT_SetDeferredRetcode(UT_KEY(OS_SocketSendTo), 1, OS_ERROR);

        UtAssert_INT32_EQ(MAVLINK_APP_InjectGPSFailureCmd(&Msg), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);
        UtAssert_UINT8_EQ(MAVLINK_APP_Data.CommandCounter, 1);
        UtAssert_UINT8_EQ(MAVLINK_APP_Data.CommandErrorCounter, 1);
        UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    }

void Test_MAVLINK_APP_RestoreGPSFailureCmd(void)
{
    UT_CheckEvent_t EventTest;
    mavlink_command_long_t Cmd;
    MAVLINK_APP_RestoreGPSFailureCmd_t Msg;
    mavlink_message_t MavMsg;
    mavlink_status_t MavStatus;
    bool Decoded = false;
    size_t i;

    memset(&MAVLINK_APP_Data, 0, sizeof(MAVLINK_APP_Data)); 
    memset(&Cmd, 0, sizeof(Cmd));
    memset(&Msg, 0, sizeof(Msg));
    memset(&MavStatus, 0, sizeof(MavStatus));
    UT_SentLen = 0;

    /* Nomimnal: Correct InjectGPSFault Command */
    UT_CHECKEVENT_SETUP(&EventTest, MAVLINK_APP_RESTORE_GPS_FAILURE_INF_EID, NULL);
    UT_SetHandlerFunction(UT_KEY(OS_SocketSendTo), UT_CaptureSendTo, NULL);

    UtAssert_INT32_EQ(MAVLINK_APP_RestoreGPSFailureCmd(&Msg), CFE_SUCCESS);
    UtAssert_UINT8_EQ(MAVLINK_APP_Data.CommandCounter, 1);
    UtAssert_UINT8_EQ(MAVLINK_APP_Data.CommandErrorCounter, 0);
    UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
    UtAssert_STUB_COUNT(OS_SocketSendTo, 1);

    for (i = 0; i < UT_SentLen; i++) {
        if (mavlink_parse_char(MAVLINK_COMM_1, UT_SentBuf[i], &MavMsg, &MavStatus) && MavMsg.msgid == MAVLINK_MSG_ID_COMMAND_LONG) {
            mavlink_msg_command_long_decode(&MavMsg, &Cmd);
            Decoded = true;
            break;
        }
    }
        UtAssert_True(Decoded, "Sent bytes decoded as MAVLink COMMAND_LONG");
        UtAssert_UINT16_EQ(Cmd.command, MAV_CMD_INJECT_FAILURE);
        UtAssert_UINT8_EQ(Cmd.target_system, 1);
        UtAssert_UINT8_EQ(Cmd.target_component, 1);
        UtAssert_True(Cmd.param1 == 4.0f, "param1 is FAIILURE_UNIT_SENSOR_GPS (4), got %f", (double)Cmd.param1);
        UtAssert_True(Cmd.param2 == 0.0f, "param2 is FAILURE_TYPE_OK (0), got %f", (double)Cmd.param2);

        /* Error: socket send fails */
        UT_CHECKEVENT_SETUP(&EventTest, MAVLINK_APP_RESTORE_GPS_FAILURE_ERR_EID, NULL);
        UT_SetDeferredRetcode(UT_KEY(OS_SocketSendTo), 1, OS_ERROR);

        UtAssert_INT32_EQ(MAVLINK_APP_RestoreGPSFailureCmd(&Msg), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);
        UtAssert_UINT8_EQ(MAVLINK_APP_Data.CommandCounter, 1);
        UtAssert_UINT8_EQ(MAVLINK_APP_Data.CommandErrorCounter, 1);
        UtAssert_UINT32_EQ(EventTest.MatchCount, 1);
}



/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(MAVLINK_APP_ReportHousekeeping);
    ADD_TEST(MAVLINK_APP_NoopCmd);
    ADD_TEST(MAVLINK_APP_ResetCountersCmd);
    ADD_TEST(MAVLINK_APP_ProcessCmd);
    ADD_TEST(MAVLINK_APP_DisplayParamCmd);
    ADD_TEST(MAVLINK_APP_StartMissionCmd);
    ADD_TEST(MAVLINK_APP_InjectGPSFaultCmd);
    ADD_TEST(MAVLINK_APP_RestoreGPSFailureCmd);
}
