FILE(GLOB CPPSOURCES *.cpp *.h *.ver)

add_library(common STATIC ${CPPSOURCES})

target_link_libraries(common jsoncpp)
target_link_libraries(common curlpp)

target_include_directories(common PUBLIC ..)
