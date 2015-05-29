cmake_minimum_required(VERSION 2.6)

project(ProbeApiTools)

add_subdirectory(deps/curl/ProbeApiBuild)
add_subdirectory(deps/curlpp/ProbeApiBuild)
add_subdirectory(deps/jsoncpp/ProbeApiBuild)

add_subdirectory(src/common)

add_subdirectory(src/dping)
add_subdirectory(src/dtraceroute)
