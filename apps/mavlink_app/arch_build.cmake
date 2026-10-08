###########################################################
#
# MAVLINK_APP platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the MAVLINK_APP configuration
set(MAVLINK_APP_PLATFORM_CONFIG_FILE_LIST
  mavlink_app_internal_cfg_values.h
  mavlink_app_platform_cfg.h
  mavlink_app_perfids.h
  mavlink_app_msgids.h
  mavlink_app_msgid_values.h
)

generate_configfile_set(${MAVLINK_APP_PLATFORM_CONFIG_FILE_LIST})

