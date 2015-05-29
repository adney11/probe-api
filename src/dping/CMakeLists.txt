include_directories(..)
include_directories(../../deps/jsoncpp/include)
include_directories(../../deps/curlpp/include)
include_directories(../../deps/curl/include)

add_definitions(-DDPING_PRJ)
# add_definitions(-DCURL_STATICLIB)
# add_definitions(-DCURLPP_STATICLIB)

FILE(GLOB CPPSOURCES *.cpp *.h *.ico *.rc)

set(PROJECTNAME "dping")

add_executable(${PROJECTNAME} ${CPPSOURCES})

target_link_libraries(${PROJECTNAME} common libcurl curlpp jsoncpp)
if(WIN32)
  target_link_libraries(${PROJECTNAME} ws2_32)
endif()
