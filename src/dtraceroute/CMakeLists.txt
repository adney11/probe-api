add_definitions(-DDTRACEROUTE_PRJ)
# add_definitions(-DCURL_STATICLIB)
# add_definitions(-DCURLPP_STATICLIB)

set(PROJECTNAME "dtraceroute")
if(WIN32)
	set(PROJECTNAME "dtracert")
endif()
set(TRACEROUTENAME "${PROJECTNAME}")

FILE(GLOB CPPSOURCES *.cpp *.h *.ico *.rc)

add_executable(${PROJECTNAME} ${CPPSOURCES})

target_link_libraries(${PROJECTNAME} common curl curlpp jsoncpp)
