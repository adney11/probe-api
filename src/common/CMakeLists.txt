
FILE(GLOB CPPSOURCES *.cpp *.h *.ver)

add_definitions(-DCURL_STATICLIB)
add_definitions(-DCURLPP_STATICLIB)

include_directories(../../deps/jsoncpp/include)
include_directories(../../deps/curlpp/include)
include_directories(../../deps/curl/include)

add_library(common STATIC ${CPPSOURCES})
# target_link_libraries(common libcurl curlpp jsoncpp)

# http://stackoverflow.com/questions/1618927/cmake-microsoft-visual-studio-and-monolithic-runtimes
if(MSVC)
#We statically link to reduce dependancies
foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
    if(${flag_var} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MDd")
endforeach(flag_var)
endif(MSVC)
