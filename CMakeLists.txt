# 3.0 is needed for "line continuation with \":
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(ProbeApiTools)

add_subdirectory(deps/curl/ProbeApiBuild)
add_subdirectory(deps/curlpp/ProbeApiBuild)
add_subdirectory(deps/jsoncpp/ProbeApiBuild)

add_subdirectory(src/common)

add_subdirectory(src/dping)
add_subdirectory(src/dtracert)
