FILE(GLOB CPPSOURCES *.cpp *.h *.ico *.rc)

set(PROJECTNAME "dping")

add_executable(${PROJECTNAME} ${CPPSOURCES})

target_link_libraries(${PROJECTNAME} common)

set_property(TARGET ${PROJECTNAME} PROPERTY CXX_STANDARD 11)

target_compile_definitions(${PROJECTNAME} PRIVATE -DDPING_PRJ)
