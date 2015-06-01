add_definitions(-DCURL_STATICLIB)
add_definitions(-DCURLPP_STATICLIB)

FILE(GLOB CPPSOURCES *.cpp *.h *.ver)

add_library(common STATIC ${CPPSOURCES})

target_link_libraries(common jsoncpp)
target_link_libraries(common curlpp)
target_link_libraries(common curl)

target_include_directories(common PUBLIC ..)
