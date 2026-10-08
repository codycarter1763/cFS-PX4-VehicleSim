###########################################################
#
# SCH_LAB platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the SCH_LAB configuration
set(SCH_LAB_PLATFORM_CONFIG_FILE_LIST
  sch_lab_internal_cfg_values.h
  sch_lab_platform_cfg.h
  sch_lab_perfids.h
)

generate_configfile_set(${SCH_LAB_PLATFORM_CONFIG_FILE_LIST})

