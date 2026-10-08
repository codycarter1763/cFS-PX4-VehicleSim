project(CFS_CS C)

set(APP_SRC_FILES
  fsw/src/cs_cmds.c
  fsw/src/cs_table_processing.c
  fsw/src/cs_app.c
  fsw/src/cs_utils.c
  fsw/src/cs_table_cmds.c
  fsw/src/cs_app_cmds.c
  fsw/src/cs_memory_cmds.c
  fsw/src/cs_init.c
  fsw/src/cs_eeprom_cmds.c
  fsw/src/cs_compute.c
)

if (CFE_EDS_ENABLED)
  list(APPEND APP_SRC_FILES fsw/src/cs_eds_dispatch.c)
else()
  list(APPEND APP_SRC_FILES fsw/src/cs_dispatch.c)
endif()

# Create the app module
add_cfe_app(cs ${APP_SRC_FILES})

# This permits direct access to public headers in the fsw/inc directory
target_include_directories(cs PUBLIC fsw/inc)

set(APP_TABLE_FILES
  fsw/tables/cs_apptbl.c
  fsw/tables/cs_eepromtbl.c
  fsw/tables/cs_memorytbl.c
  fsw/tables/cs_tablestbl.c
)

add_cfe_tables(cs ${APP_TABLE_FILES})

# If UT is enabled, then add the tests from the subdirectory
# Note that this is an app, and therefore does not provide
# stub functions, as other entities would not typically make
# direct function calls into this application.
if(ENABLE_UNIT_TESTS)
  add_subdirectory(unit-test)
endif()
