###########################################################
#
# FM app mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# Add stand alone documentation
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs/dox_src ${MISSION_BINARY_DIR}/docs/fm-usersguide)

# The list of header files that control the FM configuration
set(FM_MISSION_CONFIG_FILE_LIST
    fm_fcncode_values.h
    fm_interface_cfg_values.h
    fm_mission_cfg.h
    fm_msg.h
    fm_msgdefs.h
    fm_msgstruct.h
    fm_perfids.h
    fm_tbl.h
    fm_tbldefs.h
    fm_tblstruct.h
    fm_topicid_values.h
    fm_extern_typedefs.h
)

generate_configfile_set(${FM_MISSION_CONFIG_FILE_LIST})