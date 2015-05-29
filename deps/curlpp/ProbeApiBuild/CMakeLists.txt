
FILE(GLOB CPPSOURCES
	../src/curlpp/*.cpp
	../src/curlpp/*.h
	../src/curlpp/*.ins
	../src/curlpp/internal/*.cpp
	../src/utilspp/*.cpp
	../include/curlpp/*.h
	../include/curlpp/*.hpp
	../include/curlpp/*.inl
	../include/curlpp/internal/*.h
	../include/curlpp/internal/*.hpp
	../include/curlpp/internal/*.inl
	)

include_directories(../include)
include_directories(../../curl/include)

add_definitions(-DCURL_STATICLIB)
add_definitions(-DCURLPP_STATICLIB)

add_library(curlpp STATIC ${CPPSOURCES})

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
