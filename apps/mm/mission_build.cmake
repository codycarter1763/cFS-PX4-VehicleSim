###########################################################
#
# MM mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# Add stand alone documentation
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs/dox_src ${MISSION_BINARY_DIR}/docs/mm-usersguide)

# The list of header files that control the MM configuration
set(MM_MISSION_CONFIG_FILE_LIST
    mm_extern_typedefs.h
    mm_filedefs.h
    mm_fcncode_values.h
    mm_interface_cfg_values.h
    mm_mission_cfg.h
    mm_msgdefs.h
    mm_msg.h
    mm_msgstruct.h
    mm_perfids.h
    mm_topicid_values.h
)

generate_configfile_set(${MM_MISSION_CONFIG_FILE_LIST})