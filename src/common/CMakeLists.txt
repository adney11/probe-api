include_directories(../../deps/jsoncpp/include)
include_directories(../../deps/curlpp/include)
include_directories(../../deps/curl/include)

add_definitions(-DCURL_STATICLIB)
add_definitions(-DCURLPP_STATICLIB)

FILE(GLOB CPPSOURCES *.cpp *.h *.ver)

add_library(common STATIC ${CPPSOURCES})
# target_link_libraries(common libcurl curlpp jsoncpp)
