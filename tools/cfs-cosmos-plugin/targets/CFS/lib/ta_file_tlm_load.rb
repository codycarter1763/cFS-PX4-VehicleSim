# Set this to match your OSAL configuration!
os_max_cpus = 64

# Calculate dynamic byte widths
mask_bytes = (os_max_cpus + 7) / 8
# Calculate compiler padding before the 32-bit Type enum (Struct offset is 20 + 2*mask_bytes)
pre_enum_offset = 20 + (2 * mask_bytes)
padding_bytes = (4 - (pre_enum_offset % 4)) % 4

struct_bytes = 20 + (2 * mask_bytes) + padding_bytes + 4 + 4

raw = File.binread(get_target_file('CFS-1/lib/ta_dump.bin').path)[64..-1]

data = {
  'CORES_CONFIGURED' => raw[0,2].unpack1('v'),
  'TA_CORES_MAX'     => raw[2,2].unpack1('v')
}

type_map = {0 => 'OS_DEFAULT', 1 => 'MASK', 2 => 'ADDED'}
stat_map = {0 => 'UNKNOWN', 1 => 'OK', 2 => 'ERROR'}

# Dynamic unpack string: String(20), Mask(X), OS_Mask(X), Padding(X), Type(32), Status(32)
# 'a' is string, 'a*' grabs bytes, 'x' skips padding, 'V' is 32-bit unsigned LE
unpack_fmt = "Z20 a#{mask_bytes} a#{mask_bytes} x#{padding_bytes} V V"

num_tasks = (raw.length - 4) / struct_bytes

raw[4..-1].unpack(unpack_fmt * num_tasks).each_slice(5).with_index do |t, i|
  break if i >= 64

  # Convert byte arrays into little-endian integers, FORCED to Integer (.to_i)
  mask_val  = (t[1].unpack1('Q<') || t[1].unpack1('V') || t[1].unpack1('C')).to_i
  os_mask_val = (t[2].unpack1('Q<') || t[2].unpack1('V') || t[2].unpack1('C')).to_i

  type_str = type_map[t[3]] || 'OS_DEFAULT'
  stat_str = stat_map[t[4]] || 'UNKNOWN'

  data.merge!("TASK_NAME_#{i}"   => t[0].strip,
              "TA_MASK_#{i}"     => mask_val,
              "TA_FROM_OS_#{i}"  => os_mask_val,
              "TA_TYPE_#{i}"     => type_str,
              "TA_STATUS_#{i}"   => stat_str)
end

inject_tlm('CFS-1', 'TA_AFFINITY_FILE', data)
