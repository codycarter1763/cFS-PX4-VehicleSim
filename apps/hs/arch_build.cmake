###########################################################
#
# HS App platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the HS configuration
set(HS_PLATFORM_CONFIG_FILE_LIST
  hs_internal_cfg_values.h
  hs_msgid_values.h
  hs_msgids.h
  hs_platform_cfg.h
)

generate_configfile_set(${HS_PLATFORM_CONFIG_FILE_LIST})