
FILE(GLOB CPPSOURCES
#	../src/*.c
#	../src/*.h
	../lib/*.c
	../lib/*.h
	../lib/vtls/*.c
	../lib/vtls/*.h
	../include/curl/*.h
	)

include_directories(../include)
include_directories(../lib)

add_definitions(-DBUILDING_LIBCURL)
add_definitions(-DCURL_STATICLIB)

if(WIN32)
	add_definitions(-DUSE_WINDOWS_SSPI)
	add_definitions(-DUSE_SCHANNEL)
else()

endif()

add_definitions(-DHTTP_ONLY)
add_definitions(-DCURL_DISABLE_CRYPTO_AUTH)

add_library(libcurl STATIC ${CPPSOURCES})

# http://stackoverflow.com/questions/1618927/cmake-microsoft-visual-studio-and-monolithic-runtimes
if(MSVC)
#We statically link to reduce dependencies
foreach(flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
    if(${flag_var} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MDd")
endforeach(flag_var)
endif(MSVC)
