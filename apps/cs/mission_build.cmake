# App specific mission scope configuration

# Add stand alone documentation
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs/dox_src ${MISSION_BINARY_DIR}/docs/cs-usersguide)

# The list of header files that control the CF configuration
set(CS_MISSION_CONFIG_FILE_LIST
  cs_extern_typedefs.h
  cs_fcncode_values.h
  cs_interface_cfg_values.h
  cs_mission_cfg.h
  cs_msgdefs.h
  cs_msg.h
  cs_msgstruct.h
  cs_tbldefs.h
  cs_tbl.h
  cs_topicid_values.h
)

generate_configfile_set(${CS_MISSION_CONFIG_FILE_LIST})
