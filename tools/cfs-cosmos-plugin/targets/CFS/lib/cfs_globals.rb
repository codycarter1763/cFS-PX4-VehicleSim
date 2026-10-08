# cFS Globals

# -------------------------------------------------------------------------------------------------
# Defines the format of our cFS target names
# -------------------------------------------------------------------------------------------------
def cfs_target_name_from_cpu_num(cpu_num)
  return "CFS-#{cpu_num}"
end

# -------------------------------------------------------------------------------------------------
# Gets the CPU number of the provided cFS target name
# -------------------------------------------------------------------------------------------------
def cfs_cpu_num_from_target_name(target_name)
  return Integer(target_name.delete("CFS\-") || '')
rescue ArgumentError
    nil
end

# -------------------------------------------------------------------------------------------------
# Number of CPU instances of the FSW that can be interfaced with
# -------------------------------------------------------------------------------------------------
$cfs_total_valid_cpus = 2

# -------------------------------------------------------------------------------------------------
# EDS Enabled / Disabled Flag
# Must be boolean (`true` or `false`)
# -------------------------------------------------------------------------------------------------
$cfs_globals_eds_enabled = false

# Set the related global variable, based on the input (called from plugin.txt)
# The user should have entered either "true" or "false"
def cfs_globals_set_eds_enabled(eds_enabled_input_string)
  if eds_enabled_input_string == "true"
    $cfs_globals_eds_enabled = true
  else
    $cfs_globals_eds_enabled = false
  end
end


# -------------------------------------------------------------------------------------------------
# FSW Target Memory Size
# Must be `32` or `64`
# -------------------------------------------------------------------------------------------------
$cfs_globals_mem_addr_size = 64

# Set the related global variable, based on the input (called from plugin.txt)
# The user should have entered either "32" or "64"
# This relates to FSW memory pointer sizes, as defined by: `CFE_ES_MemOffset_t`
def cfs_globals_set_mem_addr_size(cfs_mem_addr_size_input_string)
  if cfs_mem_addr_size_input_string == "64"
    $cfs_globals_mem_addr_size = 64
  else
    $cfs_globals_mem_addr_size = 32
  end
end


# -------------------------------------------------------------------------------------------------
# FSW Target's Processor Endianness
# Must be "LITTLE_ENDIAN" or "BIG_ENDIAN"
# -------------------------------------------------------------------------------------------------
$cfs_globals_endianness = "LITTLE_ENDIAN"

# Set the related global variable, based on the input (called from plugin.txt)
# The user should have entered either "true" or "false"
def cfs_globals_set_endianness(endianness_input_string)
  if endianness_input_string == "BIG_ENDIAN"
    $cfs_globals_endianness = "BIG_ENDIAN"
  else
    $cfs_globals_endianness = "LITTLE_ENDIAN"
  end
end


# -------------------------------------------------------------------------------------------------
# cFE Time Server Configuration
#   Note: This value should correspond with CFE_PLATFORM_TIME_CFG_SERVER
# -------------------------------------------------------------------------------------------------
$cfs_time_server_config = true

# -------------------------------------------------------------------------------------------------
# cFE Time Client Configuration
#   Note: This value should correspond with CFE_PLATFORM_TIME_CFG_CLIENT
# -------------------------------------------------------------------------------------------------
$cfs_time_client_config = false

# -------------------------------------------------------------------------------------------------
# Number of Absolute Time Sequences in SC
#   Note: This value should correspond with SC_NUMBER_OF_ATS
# -------------------------------------------------------------------------------------------------
$sc_num_ats = 2

# -------------------------------------------------------------------------------------------------
# Number of Relative Time Sequences in SC
#   Note: This value should correspond with SC_NUMBER_OF_RTS
# -------------------------------------------------------------------------------------------------
$sc_num_rts = 4

# -------------------------------------------------------------------------------------------------
# Number of RTS status fields in the SC housekeeping telemetry (octets)
# -------------------------------------------------------------------------------------------------
$sc_num_rts_status_fields = (($sc_num_rts + 7) / 8)

# -------------------------------------------------------------------------------------------------
# Number of MD dwell tables defined in the system
#   Note: This value should correspond with MD_NUM_DWELL_TABLES
# -------------------------------------------------------------------------------------------------
$md_num_dwell_tables = 4

# -------------------------------------------------------------------------------------------------
# Whether to move files to the downlink directory after closing
#   Note: This value should correspond with DS_MOVE_FILES
# -------------------------------------------------------------------------------------------------
$ds_move_files = true
