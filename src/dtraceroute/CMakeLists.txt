include_directories(..)
include_directories(../../deps/jsoncpp/include)
include_directories(../../deps/curlpp/include)
include_directories(../../deps/curl/include)

add_definitions(-DDTRACEROUTE_PRJ)
# add_definitions(-DCURL_STATICLIB)
# add_definitions(-DCURLPP_STATICLIB)

set(PROJECTNAME "dtraceroute")
if(WIN32)
	set(PROJECTNAME "dtracert")
endif()

FILE(GLOB CPPSOURCES *.cpp *.h *.ico *.rc)

add_executable(${PROJECTNAME} ${CPPSOURCES})

target_link_libraries(${PROJECTNAME} common libcurl curlpp jsoncpp)
if(WIN32)
  target_link_libraries(${PROJECTNAME} ws2_32)
endif()
