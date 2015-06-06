set(PROJECTNAME "dtraceroute")
if(WIN32)
	set(PROJECTNAME "dtracert")
endif()
set(TRACEROUTENAME "${PROJECTNAME}")

FILE(GLOB CPPSOURCES *.cpp *.h *.ico *.rc)

add_executable(${PROJECTNAME} ${CPPSOURCES})

target_link_libraries(${PROJECTNAME} common)

set_property(TARGET ${PROJECTNAME} PROPERTY CXX_STANDARD 11)

target_compile_definitions(${PROJECTNAME} PRIVATE -DDTRACEROUTE_PRJ)

INSTALL(TARGETS ${PROJECTNAME} RUNTIME DESTINATION bin)
