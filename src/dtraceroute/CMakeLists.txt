
FILE(GLOB CPPSOURCES *.cpp *.h *.ico *.rc)

include_directories(..)
include_directories(../../deps/jsoncpp/include)
include_directories(../../deps/curlpp/include)
include_directories(../../deps/curl/include)

add_definitions(-DDPING_PRJ)
# add_definitions(-DCURL_STATICLIB)
# add_definitions(-DCURLPP_STATICLIB)

set(PROJECTNAME "dtraceroute")
if(WIN32)
	set(PROJECTNAME "dtracert")
endif()

add_executable(${PROJECTNAME} ${CPPSOURCES})
target_link_libraries(${PROJECTNAME} common libcurl curlpp jsoncpp)
if(WIN32)
  target_link_libraries(${PROJECTNAME} ws2_32)
endif()

# http://stackoverflow.com/questions/1618927/cmake-microsoft-visual-studio-and-monolithic-runtimes
if(MSVC)
#We statically link to reduce dependencies
foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
    if(${flag_var} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MDd")
endforeach(flag_var)
endif(MSVC)
