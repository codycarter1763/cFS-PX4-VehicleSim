###########################################################
#
# TO_LAB platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the TO_LAB configuration
set(TO_LAB_PLATFORM_CONFIG_FILE_LIST
  to_lab_internal_cfg_values.h
  to_lab_platform_cfg.h
  to_lab_perfids.h
  to_lab_msgids.h
  to_lab_msgid_values.h
)

generate_configfile_set(${TO_LAB_PLATFORM_CONFIG_FILE_LIST})

