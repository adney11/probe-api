FILE(GLOB CPPSOURCES *.cpp *.h *.ico *.rc)

set(PROJECTNAME "dping")

add_executable(${PROJECTNAME} ${CPPSOURCES})

target_link_libraries(${PROJECTNAME} common)

target_compile_definitions(curlpp PRIVATE -DDPING_PRJ)
