project(CFS_TO_LAB C)

set(TO_TABLE_COMPILE_DEFS)

set(APP_SRC_FILES
    fsw/src/to_lab_app.c
    fsw/src/to_lab_cmds.c
)

if (CFE_EDS_ENABLED)
  list(APPEND APP_SRC_FILES
    fsw/src/to_lab_eds_dispatch.c
    fsw/src/to_lab_eds_encode.c
  )
else()
  list(APPEND APP_SRC_FILES
    fsw/src/to_lab_dispatch.c
    fsw/src/to_lab_passthru_encode.c
  )
endif()

# Create the app module
add_cfe_app(to_lab ${APP_SRC_FILES})

add_cfe_tables(to_lab fsw/tables/to_lab_sub.c)

target_include_directories(to_lab PUBLIC fsw/inc)

target_include_directories(cfe-platform-table-build INTERFACE
    ${MISSION_SOURCE_DIR}/apps/mavlink_app/fsw/inc
)