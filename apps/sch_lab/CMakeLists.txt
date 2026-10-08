project(CFS_SCH_LAB C)

# Create the app module
add_cfe_app(sch_lab fsw/src/sch_lab_app.c)
add_cfe_tables(sch_lab fsw/tables/sch_lab_table.c)

target_include_directories(sch_lab PUBLIC fsw/inc)
