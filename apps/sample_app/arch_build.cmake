###########################################################
#
# SAMPLE_APP platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the SAMPLE_APP configuration
set(SAMPLE_APP_PLATFORM_CONFIG_FILE_LIST
  sample_app_internal_cfg_values.h
  sample_app_platform_cfg.h
  sample_app_perfids.h
  sample_app_msgids.h
  sample_app_msgid_values.h
)

generate_configfile_set(${SAMPLE_APP_PLATFORM_CONFIG_FILE_LIST})

