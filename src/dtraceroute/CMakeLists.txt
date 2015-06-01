set(PROJECTNAME "dtraceroute")
if(WIN32)
	set(PROJECTNAME "dtracert")
endif()
set(TRACEROUTENAME "${PROJECTNAME}")

FILE(GLOB CPPSOURCES *.cpp *.h *.ico *.rc)

add_executable(${PROJECTNAME} ${CPPSOURCES})

target_link_libraries(${PROJECTNAME} common)

target_compile_definitions(curlpp PRIVATE -DDTRACEROUTE_PRJ)
