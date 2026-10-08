###########################################################
#
# FM App platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the FM configuration
set(FM_PLATFORM_CONFIG_FILE_LIST
    fm_internal_cfg_values.h
    fm_platform_cfg.h
    fm_msgid_values.h
    fm_msgids.h
)

generate_configfile_set(${FM_PLATFORM_CONFIG_FILE_LIST})