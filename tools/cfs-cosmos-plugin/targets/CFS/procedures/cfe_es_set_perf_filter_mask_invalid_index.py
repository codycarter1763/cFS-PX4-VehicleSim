#
# Procedure: Verify that CFE_ES rejects an out-of-range performance filter mask index
#
# Assumptions:
#   - Telemetry output has already been enabled by the user
#   - CFE_ES ERROR events are enabled
#   - CFE_EVS is configured for long event messages
#   - No unrelated long events overwrite the expected event during the polling window
#   - The mission supports 128 performance IDs (sixteen 8-bit mask bytes)
#

target = "<%= target_name %>"
invalid_filter_mask_num = 16
expected_event_id = 64
expected_message = (
    "Performance Filter Mask Cmd Error, Index(16) out of range, "
    "valid range is 0 to 15"
)

# Require current ES and EVS housekeeping and the long event-message format.
wait_check_packet(target, "CFE_ES_HK", 1, 20)
wait_check_packet(target, "CFE_EVS_HK", 1, 20)
check(f"{target} CFE_EVS_HK MESSAGE_FORMAT_MODE == 'LONG'")

# Snapshot telemetry so an event from an earlier command cannot satisfy the test.
initial_event_count = tlm(f"{target} CFE_EVS_LONG_EVENT_MSG RECEIVED_COUNT")
if initial_event_count is None:
    initial_event_count = 0

initial_error_count = tlm(f"{target} CFE_ES_HK COMMAND_ERROR_COUNTER")
expected_error_count = (initial_error_count + 1) % 256

# The valid filter-mask byte indices are 0 through 15, so 16 is the first invalid index.
cmd(
    f"{target} CFE_ES_CMD_SET_PERF_FILTER_MASK with "
    f"FILTER_MASK_NUM {invalid_filter_mask_num}, FILTER_MASK 0"
)

# Verify that ES counted the command as an error.
wait_check(f"{target} CFE_ES_HK COMMAND_ERROR_COUNTER == {expected_error_count}", 20)

print(f"Received CFE_ES EID {expected_event_id}: {expected_message}")
