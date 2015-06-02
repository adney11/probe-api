FILE(GLOB CPPSOURCES *.cpp *.h *.ver)

add_library(common STATIC ${CPPSOURCES})

target_link_libraries(common INTERFACE_LINK_LIBRARIES jsoncpp)
target_link_libraries(common INTERFACE_LINK_LIBRARIES curlpp)

target_include_directories(common PUBLIC ..)
