###########################################################
#
# CS App platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the app configuration
set(CS_PLATFORM_CONFIG_FILE_LIST
  cs_internal_cfg_values.h
  cs_msgid_values.h
  cs_msgids.h
  cs_platform_cfg.h
)

generate_configfile_set(${CS_PLATFORM_CONFIG_FILE_LIST})
