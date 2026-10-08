require 'cfs_globals.rb'

#------------------------------------------------------------------------------
# Class to maintain FSW Message Info
#------------------------------------------------------------------------------
class FswMsgInfo
    attr_accessor :fsw_msg_base_stream_id
    attr_accessor :fsw_msg_base_eds_stream_id
    attr_accessor :fsw_msg_packet_names
    def initialize(params)
        @fsw_msg_base_stream_id = params[:base_stream_id]
        @fsw_msg_base_eds_stream_id = params[:base_eds_stream_id]
        @fsw_msg_packet_names = params[:packet_names]
    end
end

#------------------------------------------------------------------------------
# cFS Command / Telemetry Message ID Reference Names
# mapped to Message ID values (per CPU) and pkt names
#------------------------------------------------------------------------------
$CFS_CMD_TLM_LIST = {
    # -------------------------------------------------------------------------
    # Commands
    # -------------------------------------------------------------------------
    "CFE_EVS_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1801,
        packet_names: [
            "CFE_EVS_CMD_NOOP",
            "CFE_EVS_CMD_RESET_COUNTERS",
            "CFE_EVS_CMD_ENABLE_EVENT_TYPE",
            "CFE_EVS_CMD_DISABLE_EVENT_TYPE",
            "CFE_EVS_CMD_SET_EVENT_FORMAT_MODE",
            "CFE_EVS_CMD_ENABLE_APP_EVENT_TYPE",
            "CFE_EVS_CMD_DISABLE_APP_EVENT_TYPE",
            "CFE_EVS_CMD_ENABLE_APP_EVENTS",
            "CFE_EVS_CMD_DISABLE_APP_EVENTS",
            "CFE_EVS_CMD_RESET_APP_COUNTER",
            "CFE_EVS_CMD_SET_FILTER",
            "CFE_EVS_CMD_ENABLE_PORTS",
            "CFE_EVS_CMD_DISABLE_PORTS",
            "CFE_EVS_CMD_RESET_FILTER",
            "CFE_EVS_CMD_RESET_ALL_FILTERS",
            "CFE_EVS_CMD_ADD_EVENT_FILTER",
            "CFE_EVS_CMD_DELETE_EVENT_FILTER",
            "CFE_EVS_CMD_WRITE_APP_DATA_FILE",
            "CFE_EVS_CMD_WRITE_LOG_DATA_FILE",
            "CFE_EVS_CMD_SET_LOG_MODE",
            "CFE_EVS_CMD_CLEAR_LOG",
        ]
    ),
    "CFE_EVS_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1809,
        packet_names: [
            "CFE_EVS_SEND_HK_CMD",
        ]
    ),
    "CFE_SB_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1803,
        packet_names: [
            "CFE_SB_CMD_NOOP",
            "CFE_SB_CMD_RESET_COUNTERS",
            "CFE_SB_CMD_SEND_SB_STATS",
            "CFE_SB_CMD_WRITE_ROUTING_INFO",
            "CFE_SB_CMD_ENABLE_ROUTE",
            "CFE_SB_CMD_DISABLE_ROUTE",
            "CFE_SB_CMD_WRITE_PIPE_INFO",
            "CFE_SB_CMD_WRITE_MAP_INFO",
        ]
    ),
    "CFE_SB_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x180B,
        packet_names: [
            "CFE_SB_SEND_HK_CMD",
        ]
    ),
    "CFE_TBL_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1804,
        packet_names: [
            "CFE_TBL_CMD_NOOP",
            "CFE_TBL_CMD_RESET_COUNTERS",
            "CFE_TBL_CMD_LOAD",
            "CFE_TBL_CMD_DUMP",
            "CFE_TBL_CMD_VALIDATE",
            "CFE_TBL_CMD_ACTIVATE",
            "CFE_TBL_CMD_DUMP_REGISTRY",
            "CFE_TBL_CMD_SEND_REGISTRY",
            "CFE_TBL_CMD_DELETE_CDS",
            "CFE_TBL_CMD_ABORT_LOAD",
        ]
    ),
    "CFE_TBL_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x180C,
        packet_names: [
            "CFE_TBL_SEND_HK_CMD",
        ]
    ),
    "CFE_TIME_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1805,
        packet_names: [
            "CFE_TIME_CMD_NOOP",
            "CFE_TIME_CMD_RESET_COUNTERS",
            "CFE_TIME_CMD_SEND_DIAGNOSTIC",
            "CFE_TIME_CMD_SET_SOURCE",
            "CFE_TIME_CMD_SET_STATE",
            "CFE_TIME_CMD_ADD_DELAY",
            "CFE_TIME_CMD_SUB_DELAY",
            "CFE_TIME_CMD_SET_TIME",
            "CFE_TIME_CMD_SET_MET",
            "CFE_TIME_CMD_SET_STCF",
            "CFE_TIME_CMD_SET_LEAP_SECONDS",
            "CFE_TIME_CMD_ADD_ADJUST",
            "CFE_TIME_CMD_SUB_ADJUST",
            "CFE_TIME_CMD_ADD_ONE_HZ_ADJUSTMENT",
            "CFE_TIME_CMD_SUB_ONE_HZ_ADJUSTMENT",
            "CFE_TIME_CMD_SET_SIGNAL",
        ]
    ),
    "CFE_TIME_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x180D,
        packet_names: [
            "CFE_TIME_SEND_HK_CMD",
        ]
    ),
    "CFE_ES_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1806,
        packet_names: [
            "CFE_ES_CMD_NOOP",
            "CFE_ES_CMD_RESET_COUNTERS",
            "CFE_ES_CMD_RESTART",
            "CFE_ES_CMD_START_APP",
            "CFE_ES_CMD_STOP_APP",
            "CFE_ES_CMD_RESTART_APP",
            "CFE_ES_CMD_RELOAD_APP",
            "CFE_ES_CMD_QUERY_ONE",
            "CFE_ES_CMD_QUERY_ALL",
            "CFE_ES_CMD_CLEAR_SYS_LOG",
            "CFE_ES_CMD_WRITE_SYS_LOG",
            "CFE_ES_CMD_CLEAR_ER_LOG",
            "CFE_ES_CMD_WRITE_ER_LOG",
            "CFE_ES_CMD_START_PERF_DATA",
            "CFE_ES_CMD_STOP_PERF_DATA",
            "CFE_ES_CMD_SET_PERF_FILTER_MASK",
            "CFE_ES_CMD_SET_PERF_TRIGGER_MASK",
            "CFE_ES_CMD_OVER_WRITE_SYS_LOG",
            "CFE_ES_CMD_RESET_PR_COUNT",
            "CFE_ES_CMD_SET_MAX_PR_COUNT",
            "CFE_ES_CMD_DELETE_CDS",
            "CFE_ES_CMD_SEND_MEM_POOL_STATS",
            "CFE_ES_CMD_DUMP_CDS_REGISTRY",
            "CFE_ES_CMD_QUERY_ALL_TASKS",
        ]
    ),
    "CFE_ES_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1808,
        packet_names: [
            "CFE_ES_SEND_HK_CMD",
        ]
    ),
    "CFE_SB_SUB_RPT_CTRL" => FswMsgInfo.new(
        base_stream_id: 0x180E,
        packet_names: [
            "CFE_SB_SUB_RPT_CTRL_ENABLE_SUB_REPORTING",
            "CFE_SB_SUB_RPT_CTRL_DISABLE_SUB_REPORTING",
            "CFE_SB_SUB_RPT_CTRL_SEND_PREV_SUBS",
        ]
    ),
    "TO_LAB_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1880,
        packet_names: [
            "TO_LAB_CMD_NOOP",
            "TO_LAB_CMD_RESET_COUNTERS",
            "TO_LAB_CMD_ADD_PACKET",
            "TO_LAB_CMD_SEND_DATA_TYPES",
            "TO_LAB_CMD_REMOVE_PACKET",
            "TO_LAB_CMD_REMOVE_ALL",
            "TO_LAB_CMD_ENABLE_OUTPUT",
        ]
    ),
    "TO_LAB_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1881,
        packet_names: [
            "TO_LAB_SEND_HK_CMD",
        ]
    ),
    "CF_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18B3,
        packet_names: [
            "CF_CMD_NOOP",
            "CF_CMD_RESET_COUNTERS",
            "CF_CMD_TX_FILE",
            "CF_CMD_PLAYBACK_DIR",
            "CF_CMD_FREEZE",
            "CF_CMD_THAW",
            "CF_CMD_SUSPEND",
            "CF_CMD_RESUME",
            "CF_CMD_CANCEL",
            "CF_CMD_ABANDON",
            "CF_CMD_SET_PARAM",
            "CF_CMD_GET_PARAM",
            "CF_CMD_WRITE_QUEUE",
            "CF_CMD_ENABLE_DEQUEUE",
            "CF_CMD_DISABLE_DEQUEUE",
            "CF_CMD_ENABLE_DIR_POLLING",
            "CF_CMD_DISABLE_DIR_POLLING",
            "CF_CMD_PURGE_QUEUE",
            "CF_CMD_ENABLE_ENGINE",
            "CF_CMD_DISABLE_ENGINE",
        ]
    ),
    "CF_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18B4,
        packet_names: [
            "CF_SEND_HK_CMD",
        ]
    ),
    "SAMPLE_APP_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1882,
        packet_names: [
            "SAMPLE_APP_CMD_NOOP",
            "SAMPLE_APP_CMD_RESET_COUNTERS",
            "SAMPLE_APP_CMD_PROCESS",
            "SAMPLE_APP_CMD_DISPLAY_PARAM",
        ]
    ),
    "SAMPLE_APP_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1883,
        packet_names: [
            "SAMPLE_APP_SEND_HK_CMD",
        ]
    ),
    "SBN_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18FA,
        packet_names: [
            "SBN_CMD_NOOP",
            "SBN_CMD_SEND_HK",
            "SBN_CMD_HK_NET",
            "SBN_CMD_HK_PEER",
            "SBN_CMD_HK_PEERSUBS",
            "SBN_CMD_HK_MYSUBS",
            "SBN_CMD_HK_RESET",
            "SBN_CMD_HK_RESET_PEER",
        ]
    ),
    "CI_LAB_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1884,
        packet_names: [
            "CI_LAB_CMD_NOOP",
            "CI_LAB_CMD_RESET_COUNTERS",
        ]
    ),
    "CI_LAB_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1885,
        packet_names: [
            "CI_LAB_SEND_HK_CMD",
        ]
    ),
    "FM_CMD" => FswMsgInfo.new(
        base_stream_id: 0x188C,
        packet_names: [
            "FM_CMD_NOOP",
            "FM_CMD_RESET_COUNTERS",
            "FM_CMD_COPY_FILE",
            "FM_CMD_MOVE_FILE",
            "FM_CMD_RENAME_FILE",
            "FM_CMD_DELETE_FILE",
            "FM_CMD_DELETE_ALL_FILES",
            "FM_CMD_DECOMPRESS_FILE",
            "FM_CMD_CONCAT_FILES",
            "FM_CMD_GET_FILE_INFO",
            "FM_CMD_GET_OPEN_FILES",
            "FM_CMD_CREATE_DIRECTORY",
            "FM_CMD_DELETE_DIRECTORY",
            "FM_CMD_GET_DIR_LIST_FILE",
            "FM_CMD_GET_DIR_LIST_PKT",
            "FM_CMD_MONITOR_FILESYSTEM_SPACE",
            "FM_CMD_SET_TABLE_STATE",
            "FM_CMD_SET_PERMISSIONS",
        ]
    ),
    "FM_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x188D,
        packet_names: [
            "FM_SEND_HK_CMD",
        ]
    ),
	"MM_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1888,
        packet_names: [
            "MM_CMD_NOOP",
            "MM_CMD_RESET_COUNTERS",
            "MM_CMD_PEEK",
            "MM_CMD_POKE",
            "MM_CMD_LOAD_MEM_WID",
            "MM_CMD_LOAD_MEM_FROM_FILE",
            "MM_CMD_DUMP_MEM_TO_FILE",
            "MM_CMD_DUMP_IN_EVENT",
            "MM_CMD_FILL_MEM",
            "MM_CMD_LOOKUP_SYM",
            "MM_CMD_SYM_TBL_TO_FILE",
            "MM_CMD_EEPROM_WRITE_ENA",
            "MM_CMD_EEPROM_WRITE_DIS",
        ]
    ),
    "MM_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1889,
        packet_names: [
            "MM_SEND_HK_CMD",
        ]
    ),
    "MD_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1890,
        packet_names: [
            "MD_CMD_NOOP",
            "MD_CMD_RESET_COUNTERS",
            "MD_CMD_START_DWELL",
            "MD_CMD_STOP_DWELL",
            "MD_CMD_JAM_DWELL",
            "MD_CMD_SET_SIGNATURE",
        ]
    ),
    "MD_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1891,
        packet_names: [
            "MD_SEND_HK_CMD",
        ]
    ),
    "CS_CMD" => FswMsgInfo.new(
        base_stream_id: 0x189F,
        packet_names: [
            "CS_CMD_NOOP",
            "CS_CMD_RESET_COUNTERS",
            "CS_CMD_ONE_SHOT",
            "CS_CMD_CANCEL_ONE_SHOT",
            "CS_CMD_ENABLE_ALL_CS",
            "CS_CMD_DISABLE_ALL_CS",
            "CS_CMD_ENABLE_CFE_CORE",
            "CS_CMD_DISABLE_CFE_CORE",
            "CS_CMD_REPORT_BASELINE_CFE_CORE",
            "CS_CMD_RECOMPUTE_BASELINE_CFE_CORE",
            "CS_CMD_ENABLE_OS",
            "CS_CMD_DISABLE_OS",
            "CS_CMD_REPORT_BASELINE_OS",
            "CS_CMD_RECOMPUTE_BASELINE_OS",
            "CS_CMD_ENABLE_EEPROM",
            "CS_CMD_DISABLE_EEPROM",
            "CS_CMD_REPORT_BASELINE_ENTRY_ID_EEPROM",
            "CS_CMD_RECOMPUTE_BASELINE_EEPROM",
            "CS_CMD_ENABLE_ENTRY_ID_EEPROM",
            "CS_CMD_DISABLE_ENTRY_ID_EEPROM",
            "CS_CMD_GET_ENTRY_ID_EEPROM",
            "CS_CMD_ENABLE_MEMORY",
            "CS_CMD_DISABLE_MEMORY",
            "CS_CMD_REPORT_BASELINE_ENTRY_ID_MEMORY",
            "CS_CMD_RECOMPUTE_BASELINE_MEMORY",
            "CS_CMD_ENABLE_ENTRY_ID_MEMORY",
            "CS_CMD_DISABLE_ENTRY_ID_MEMORY",
            "CS_CMD_GET_ENTRY_ID_MEMORY",
            "CS_CMD_ENABLE_TABLES",
            "CS_CMD_DISABLE_TABLES",
            "CS_CMD_REPORT_BASELINE_TABLE",
            "CS_CMD_RECOMPUTE_BASELINE_TABLE",
            "CS_CMD_ENABLE_NAME_TABLE",
            "CS_CMD_DISABLE_NAME_TABLE",
            "CS_CMD_ENABLE_APPS",
            "CS_CMD_DISABLE_APPS",
            "CS_CMD_REPORT_BASELINE_APP",
            "CS_CMD_RECOMPUTE_BASELINE_APP",
            "CS_CMD_ENABLE_NAME_APP",
            "CS_CMD_DISABLE_NAME_APP",
        ]
    ),
    "CS_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18A0,
        packet_names: [
            "CS_SEND_HK_CMD",
        ]
    ),
    "HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x189A,
        packet_names: [
            "HK_CMD_NOOP",
            "HK_CMD_RESET_COUNTERS",
        ]
    ),
    "HK_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x189B,
        packet_names: [
            "HK_SEND_HK_CMD",
        ]
    ),
    "LC_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18A4,
        packet_names: [
            "LC_CMD_NOOP",
            "LC_CMD_RESET_COUNTERS",
            "LC_CMD_SET_LC_STATE",
            "LC_CMD_SET_AP_STATE",
            "LC_CMD_SET_AP_PERM_OFF",
            "LC_CMD_RESET_AP_STATS",
            "LC_CMD_RESET_WP_STATS",
        ]
    ),
    "LC_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18A5,
        packet_names: [
            "LC_SEND_HK_CMD",
        ]
    ),
    "HS_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18AE,
        packet_names: [
            "HS_CMD_NOOP",
            "HS_CMD_RESET_COUNTERS",
            "HS_CMD_ENABLE_APP_MON",
            "HS_CMD_DISABLE_APP_MON",
            "HS_CMD_ENABLE_EVENT_MON",
            "HS_CMD_DISABLE_EVENT_MON",
            "HS_CMD_ENABLE_ALIVENESS",
            "HS_CMD_DISABLE_ALIVENESS",
            "HS_CMD_RESET_RESETS_PERFORMED",
            "HS_CMD_SET_MAX_RESETS",
            "HS_CMD_ENABLE_CPU_HOG",
            "HS_CMD_DISABLE_CPU_HOG",
            "HS_CMD_REPORT_DIAG",
			"HS_CMD_SET_UTIL_PARAMS",
			"HS_CMD_SET_UTIL_DIAG",
        ]
    ),
    "HS_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18AF,
        packet_names: [
            "HS_SEND_HK_CMD",
        ]
    ),
    "SC_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18A9,
        packet_names: [
            "SC_CMD_NOOP",
            "SC_CMD_RESET_COUNTERS",
            "SC_CMD_START_ATS",
            "SC_CMD_STOP_ATS",
            "SC_CMD_START_RTS",
            "SC_CMD_STOP_RTS",
            "SC_CMD_DISABLE_RTS",
            "SC_CMD_ENABLE_RTS",
            "SC_CMD_SWITCH_ATS",
            "SC_CMD_JUMP_ATS",
            "SC_CMD_CONTINUE_ATS_ON_FAILURE",
            "SC_CMD_APPEND_ATS",
            "SC_CMD_MANAGE_TABLE",
            "SC_CMD_START_RTS_GRP",
            "SC_CMD_STOP_RTS_GRP",
            "SC_CMD_DISABLE_RTS_GRP",
            "SC_CMD_ENABLE_RTS_GRP",
        ]
    ),
    "SC_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18AA,
        packet_names: [
            "SC_SEND_HK_CMD",
        ]
    ),
    "DS_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18BB,
        packet_names: [
            "DS_CMD_NOOP",
            "DS_CMD_RESET_COUNTERS",
            "DS_CMD_SET_APP_STATE",
            "DS_CMD_SET_FILTER_FILE",
            "DS_CMD_SET_FILTER_TYPE",
            "DS_CMD_SET_FILTER_PARAMS",
            "DS_CMD_SET_DEST_TYPE",
            "DS_CMD_SET_DEST_STATE",
            "DS_CMD_SET_DEST_PATH",
            "DS_CMD_SET_DEST_BASE",
            "DS_CMD_SET_DEST_EXT",
            "DS_CMD_SET_DEST_SIZE",
            "DS_CMD_SET_DEST_AGE",
            "DS_CMD_SET_DEST_COUNT",
            "DS_CMD_CLOSE_FILE",
            "DS_CMD_GET_FILE_INFO",
            "DS_CMD_ADD_MID",
            "DS_CMD_CLOSE_ALL",
            "DS_CMD_REMOVE_MID",
        ]
    ),
    "DS_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x18BC,
        packet_names: [
            "DS_SEND_HK_CMD",
        ]
    ),
    "TA_CMD" => FswMsgInfo.new(
    base_stream_id: 0x1812,
    packet_names: [
            "TA_CMD_NOOP",
            "TA_CMD_RESET_COUNTERS",
            "TA_CMD_QUERY_ALL_TASKS",
            "TA_CMD_SET_TASK_AFFINITY",
            "TA_CMD_GET_TASK_AFFINITY",
        ]
    ),
    "TA_SEND_HK_CMD" => FswMsgInfo.new(
        base_stream_id: 0x1813,
        packet_names: [
            "TA_SEND_HK_CMD",
        ]
    ),

    # -------------------------------------------------------------------------
    # Telemetry
    # -------------------------------------------------------------------------
    "CFE_ES_HK" => FswMsgInfo.new(
        base_stream_id: 0x0800,
        packet_names: [
            "CFE_ES_HK",
        ]
    ),
    "CFE_EVS_HK" => FswMsgInfo.new(
        base_stream_id: 0x0801,
        packet_names: [
            "CFE_EVS_HK",
        ]
    ),
    "CFE_TESTCASE_HK" => FswMsgInfo.new(
        base_stream_id: 0x0802,
        packet_names: [
            "CFE_TESTCASE_HK",
        ]
    ),
    "CFE_SB_HK" => FswMsgInfo.new(
        base_stream_id: 0x0803,
        packet_names: [
            "CFE_SB_HK",
        ]
    ),
    "CFE_TBL_HK" => FswMsgInfo.new(
        base_stream_id: 0x0804,
        packet_names: [
            "CFE_TBL_HK",
        ]
    ),
    "CFE_TIME_HK" => FswMsgInfo.new(
        base_stream_id: 0x0805,
        packet_names: [
            "CFE_TIME_HK",
        ]
    ),
    "CFE_TIME_DIAG" => FswMsgInfo.new(
        base_stream_id: 0x0806,
        packet_names: [
            "CFE_TIME_DIAG",
        ]
    ),
    "CFE_EVS_LONG_EVENT_MSG" => FswMsgInfo.new(
        base_stream_id: 0x0808,
        packet_names: [
            "CFE_EVS_LONG_EVENT_MSG",
        ]
    ),
    "CFE_EVS_SHORT_EVENT_MSG" => FswMsgInfo.new(
        base_stream_id: 0x0809,
        packet_names: [
            "CFE_EVS_SHORT_EVENT_MSG",
        ]
    ),
    "CFE_SB_STATS" => FswMsgInfo.new(
        base_stream_id: 0x080A,
        packet_names: [
            "CFE_SB_STATS",
        ]
    ),
    "CFE_ES_APP" => FswMsgInfo.new(
        base_stream_id: 0x080B,
        packet_names: [
            "CFE_ES_APP",
        ]
    ),
    "CFE_TBL_REG" => FswMsgInfo.new(
        base_stream_id: 0x080C,
        packet_names: [
            "CFE_TBL_REG",
        ]
    ),
    "CFE_SB_ALLSUBS" => FswMsgInfo.new(
        base_stream_id: 0x080D,
        packet_names: [
            "CFE_SB_ALLSUBS",
        ]
    ),
    "CFE_SB_ONESUB" => FswMsgInfo.new(
        base_stream_id: 0x080E,
        packet_names: [
            "CFE_SB_ONESUB",
        ]
    ),
    "CFE_ES_MEMSTATS" => FswMsgInfo.new(
        base_stream_id: 0x0810,
        packet_names: [
            "CFE_ES_MEMSTATS",
        ]
    ),
    "TO_LAB_HK" => FswMsgInfo.new(
        base_stream_id: 0x0880,
        packet_names: [
            "TO_LAB_HK",
        ]
    ),
    "TO_LAB_DATA_TYPES" => FswMsgInfo.new(
        base_stream_id: 0x0881,
        packet_names: [
            "TO_LAB_DATA_TYPES",
        ]
    ),
    "CF_HK" => FswMsgInfo.new(
        base_stream_id: 0x08B0,
        packet_names: [
            "CF_HK",
        ]
    ),
    "CF_EOT" => FswMsgInfo.new(
        base_stream_id: 0x08B3,
        packet_names: [
            "CF_EOT",
        ]
    ),
    "SAMPLE_APP_HK" => FswMsgInfo.new(
        base_stream_id: 0x0883,
        packet_names: [
            "SAMPLE_APP_HK",
        ]
    ),
    "SBN_HK" => FswMsgInfo.new(
        base_stream_id: 0x08FB,
        packet_names: [
            "SBN_HK",
        ]
    ),
    "SBN_HKNET" => FswMsgInfo.new(
        base_stream_id: 0x08FC,
        packet_names: [
            "SBN_HKNET",
        ]
    ),
    "SBN_HKPEER" => FswMsgInfo.new(
        base_stream_id: 0x08FD,
        packet_names: [
            "SBN_HKPEER",
        ]
    ),
    "SBN_HKMYSUBS" => FswMsgInfo.new(
        base_stream_id: 0x08FE,
        packet_names: [
            "SBN_HKMYSUBS",
        ]
    ),
    "SBN_HKPEERSUBS" => FswMsgInfo.new(
        base_stream_id: 0x08FF,
        packet_names: [
            "SBN_HKPEERSUBS",
        ]
    ),
    "CI_LAB_HK" => FswMsgInfo.new(
        base_stream_id: 0x0884,
        packet_names: [
            "CI_LAB_HK",
        ]
    ),
    "FM_HK" => FswMsgInfo.new(
        base_stream_id: 0x088A,
        packet_names: [
            "FM_HK",
        ]
    ),
    "FM_FILE_INFO" => FswMsgInfo.new(
        base_stream_id: 0x088B,
        packet_names: [
            "FM_FILE_INFO",
        ]
    ),
    "FM_DIR_LIST" => FswMsgInfo.new(
        base_stream_id: 0x088C,
        packet_names: [
            "FM_DIR_LIST",
        ]
    ),
    "FM_OPEN_FILES" => FswMsgInfo.new(
        base_stream_id: 0x088D,
        packet_names: [
            "FM_OPEN_FILES",
        ]
    ),
    "FM_MONITOR" => FswMsgInfo.new(
        base_stream_id: 0x088E,
        packet_names: [
            "FM_MONITOR",
        ]
    ),
	"MM_HK" => FswMsgInfo.new(
        base_stream_id: 0x0887,
        packet_names: [
            "MM_HK",
        ]
    ),
	"MD_HK" => FswMsgInfo.new(
        base_stream_id: 0x0890,
        packet_names: [
            "MD_HK",
        ]
    ),
	"MD_DWELL_PKT_1" => FswMsgInfo.new(
        base_stream_id: 0x0891,
        packet_names: [
            "MD_DWELL_PKT_1",
        ]
    ),
	"MD_DWELL_PKT_2" => FswMsgInfo.new(
        base_stream_id: 0x0892,
        packet_names: [
            "MD_DWELL_PKT_2",
        ]
    ),
	"MD_DWELL_PKT_3" => FswMsgInfo.new(
        base_stream_id: 0x0893,
        packet_names: [
            "MD_DWELL_PKT_3",
        ]
    ),
	"MD_DWELL_PKT_4" => FswMsgInfo.new(
        base_stream_id: 0x0894,
        packet_names: [
            "MD_DWELL_PKT_4",
        ]
    ),
    "HK_HK" => FswMsgInfo.new(
        base_stream_id: 0x089B,
        packet_names: [
            "HK_HK",
        ]
    ),
    "HK_HK_COMBINED_PKT1" => FswMsgInfo.new(
        base_stream_id: 0x089C,
        packet_names: [
            "HK_HK_COMBINED_PKT1",
        ]
    ),
    "HK_HK_COMBINED_PKT2" => FswMsgInfo.new(
        base_stream_id: 0x089D,
        packet_names: [
            "HK_HK_COMBINED_PKT2",
        ]
    ),
    "HK_HK_COMBINED_PKT3" => FswMsgInfo.new(
        base_stream_id: 0x089E,
        packet_names: [
            "HK_HK_COMBINED_PKT3",
        ]
    ),
    "HK_HK_COMBINED_PKT4" => FswMsgInfo.new(
        base_stream_id: 0x089F,
        packet_names: [
            "HK_HK_COMBINED_PKT4",
        ]
    ),
    "CS_HK" => FswMsgInfo.new(
        base_stream_id: 0x08A4,
        packet_names: [
            "CS_HK",
        ]
    ),
    "LC_HK" => FswMsgInfo.new(
        base_stream_id: 0x08A7,
        packet_names: [
            "LC_HK",
        ]
    ),
    "SC_HK" => FswMsgInfo.new(
        base_stream_id: 0x08AA,
        packet_names: [
            "SC_HK",
        ]
    ),
	"HS_HK" => FswMsgInfo.new(
        base_stream_id: 0x08AD,
        packet_names: [
            "HS_HK",
        ]
    ),
    "DS_HK" => FswMsgInfo.new(
        base_stream_id: 0x08B8,
        packet_names: [
            "DS_HK",
        ]
    ),
    "DS_DIAG" => FswMsgInfo.new(
        base_stream_id: 0x08B9,
        packet_names: [
            "DS_DIAG",
        ]
    ),
    "DS_COMP" => FswMsgInfo.new(
        base_stream_id: 0x08BA,
        packet_names: [
            "DS_COMP",
        ]
    ),
    "TA_HK" => FswMsgInfo.new(
        base_stream_id: 0x080F,
        packet_names: [
            "TA_HK",
        ]
    ),
}


#------------------------------------------------------------------------------
# Convert cpu_num string to an integer (or nil)
#------------------------------------------------------------------------------
def cpu_num_or_nil(string)
    Integer(string || '')
rescue ArgumentError
    nil
end

#------------------------------------------------------------------------------
# Translate cFS packet name to a $CFS_CMD_TLM_LIST key name
# The provided pkt_name should be in the $CFS_CMD_TLM_LIST (above)
#------------------------------------------------------------------------------
def get_cfs_cmd_tlm_list_key_from_pkt_name(pkt_name)
    cfs_msg_id_key_name = ""
    $CFS_CMD_TLM_LIST.each do |pkt_list_key, pkt_info|
        if pkt_info.fsw_msg_packet_names.include? pkt_name
            cfs_msg_id_key_name = pkt_list_key
        end
    end
    return cfs_msg_id_key_name
end

#------------------------------------------------------------------------------
# Given the 2-byte stream id, fill in the cpu number and return the result
# If EDS is not enabled, the base MIDs are used for all MIDs
#
# 1. stream_id_base
#    - base stream id (without 3-bit cpu number set)
#    - should have the mask 0xF8FF
# 2. cpu_num
#    - must be between 0 and $cfs_total_valid_cpus - 1
#    - if a number outside this range is provided,
#      the input stream_id is returned (unmodified)
#
# Example:
#  - `sprintf("0x%04X", get_cfs_msg_id_from_base(0x1801, 0)` should return "0x1801"
#  - `sprintf("0x%04X", get_cfs_msg_id_from_base(0x1801, 1)` should return "0x1901"
#  - `sprintf("0x%04X", get_cfs_msg_id_from_base(0x1801, 7)` should return "0x1F01"
#------------------------------------------------------------------------------
def get_cfs_msg_id_from_base(stream_id_base, cpu_num)
    # The cpu num is a 3-bit field, so the max value is b'111' (7)
    cpu_num_min = 0
    cpu_num_max = $cfs_total_valid_cpus - 1
    # Set the CPU Number bit field
    if ((cpu_num >= cpu_num_min) && (cpu_num <= cpu_num_max))
        stream_id = stream_id_base
        stream_id = stream_id & 0xF8FF # clear the 3-bit cpu-num field
        stream_id = stream_id | (cpu_num << 8)
    else
        # On cpu_num range error, return the base by default
        stream_id = stream_id_base
    end
    return stream_id
end


#------------------------------------------------------------------------------
# Embedded Ruby Utility to output a Message ID based on the following:
# 1. pkt_name
#    - must be a packet name found in a $CFS_CMD_TLM_LIST entry's packet_names array
# 2. cpu_num
#    - must be '1' or '2'
#------------------------------------------------------------------------------
def get_cfs_pkt_msg_id(pkt_name, cpu_num_input)
    pkt_list_key = get_cfs_cmd_tlm_list_key_from_pkt_name(pkt_name)
    cpu_num = cpu_num_or_nil(cpu_num_input)
    msg_id = String.new
    if !$CFS_CMD_TLM_LIST.key?(pkt_list_key)
        msg_id << "pkt_name error: #{pkt_name} not recognized"
    elsif (cpu_num == nil)
        msg_id << "cpu_num_input error: could not convert #{cpu_num_input} to an integer"
    elsif ((cpu_num < 1) && (cpu_num > 2))
        msg_id << "cpu_num_input error: should be 1 or 2. instead got: #{cpu_num}."
    else
        msg_id_with_cpu_num = get_cfs_msg_id_from_base(
                $CFS_CMD_TLM_LIST[pkt_list_key].fsw_msg_base_stream_id,
                cpu_num-1)
        msg_id << sprintf("0x%04X", msg_id_with_cpu_num)
    end
    return msg_id
end

#------------------------------------------------------------------------------
# Tests for definitions in this file
#------------------------------------------------------------------------------
def cfs_cmd_tlm_list_test
    raise "TEST_FAIL" unless (get_cfs_pkt_msg_id("CFE_TBL_CMD_NOOP", "1") == "0x1804")
    raise "TEST_FAIL" unless (get_cfs_pkt_msg_id("CFE_TBL_CMD_NOOP", "2") == "0x1904")
end

cfs_cmd_tlm_list_test()
