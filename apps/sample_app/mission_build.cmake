###########################################################
#
# SAMPLE_APP mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the SAMPLE_APP configuration
set(SAMPLE_APP_MISSION_CONFIG_FILE_LIST
  sample_app_fcncode_values.h
  sample_app_interface_cfg_values.h
  sample_app_mission_cfg.h
  sample_app_perfids.h
  sample_app_msg.h
  sample_app_msgdefs.h
  sample_app_msgstruct.h
  sample_app_tbl.h
  sample_app_tbldefs.h
  sample_app_tblstruct.h
  sample_app_topicid_values.h
)

generate_configfile_set(${SAMPLE_APP_MISSION_CONFIG_FILE_LIST})

