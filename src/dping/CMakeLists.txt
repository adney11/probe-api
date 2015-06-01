add_definitions(-DDPING_PRJ)
# add_definitions(-DCURL_STATICLIB)
# add_definitions(-DCURLPP_STATICLIB)

FILE(GLOB CPPSOURCES *.cpp *.h *.ico *.rc)

set(PROJECTNAME "dping")

add_executable(${PROJECTNAME} ${CPPSOURCES})

target_link_libraries(${PROJECTNAME} common curl curlpp jsoncpp)
