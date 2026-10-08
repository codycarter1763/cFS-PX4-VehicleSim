###########################################################
#
# MAVLINK_APP mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the MAVLINK_APP configuration
set(MAVLINK_APP_MISSION_CONFIG_FILE_LIST
  mavlink_app_fcncode_values.h
  mavlink_app_interface_cfg_values.h
  mavlink_app_mission_cfg.h
  mavlink_app_perfids.h
  mavlink_app_msg.h
  mavlink_app_msgdefs.h
  mavlink_app_msgstruct.h
  mavlink_app_tbl.h
  mavlink_app_tbldefs.h
  mavlink_app_tblstruct.h
  mavlink_app_topicid_values.h
)

generate_configfile_set(${MAVLINK_APP_MISSION_CONFIG_FILE_LIST})

