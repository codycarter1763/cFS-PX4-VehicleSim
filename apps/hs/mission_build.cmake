###########################################################
#
# HS App mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# Add stand alone documentation
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs/dox_src ${MISSION_BINARY_DIR}/docs/hs-usersguide)

# The list of header files that control the HS configuration
set(HS_MISSION_CONFIG_FILE_LIST
  hs_fcncode_values.h
  hs_interface_cfg_values.h
  hs_mission_cfg.h
  hs_msgdefs.h
  hs_msg.h
  hs_msgstruct.h
  hs_tbldefs.h
  hs_tbl.h
  hs_tblstruct.h
  hs_topicid_values.h
)

generate_configfile_set(${HS_MISSION_CONFIG_FILE_LIST})