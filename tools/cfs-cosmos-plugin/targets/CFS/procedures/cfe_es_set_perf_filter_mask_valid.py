#
# Procedure: Verify that CFE_ES applies a valid performance filter mask byte
#            and reports every perf-ID bit of it back in housekeeping telemetry.
#
# Assumptions:
#   - Telemetry output has already been enabled by the user
#   - The mission supports 128 performance IDs (sixteen 8-bit mask bytes)
#   - CFE_ES_HK exposes the filter mask as per-perf-ID bit items named
#     PERF_FILTER_MASK_<perf_id> (perf_id = byte_index * 8 + bit)
#

target = "<%= target_name %>"
filter_mask_num = 2   # byte index, valid range 0..15
filter_mask = 0xFF    # 8-bit value applied to perf IDs (num*8)..(num*8+7)

# Require current ES housekeeping before snapshotting the command counter.
wait_check_packet(target, "CFE_ES_HK", 1, 20)

# A valid command must advance the command counter by exactly one (8-bit wrap).
initial_command_count = tlm(f"{target} CFE_ES_HK COMMAND_COUNTER")
if initial_command_count is None:
    initial_command_count = 0
expected_command_count = (initial_command_count + 1) % 256

cmd(
    f"{target} CFE_ES_CMD_SET_PERF_FILTER_MASK with "
    f"FILTER_MASK_NUM {filter_mask_num}, FILTER_MASK {filter_mask}"
)

# Block until the housekeeping packet that reflects this command arrives.
# The counter and the mask are filled from the same HK build, so once the
# counter matches, the latest packet also carries the updated mask.
wait_check(f"{target} CFE_ES_HK COMMAND_COUNTER == {expected_command_count}", 20)

# Each bit of the commanded byte maps to one perf ID: bit b -> perf ID num*8 + b.
for bit in range(8):
    perf_id = filter_mask_num * 8 + bit
    expected_bit = (filter_mask >> bit) & 1
    check(f"{target} CFE_ES_HK PERF_FILTER_MASK_{perf_id} == {expected_bit}")

print(
    f"Verified CFE_ES perf filter byte {filter_mask_num} = "
    f"0x{filter_mask:02X} in CFE_ES_HK (perf IDs "
    f"{filter_mask_num * 8}..{filter_mask_num * 8 + 7})"
)
