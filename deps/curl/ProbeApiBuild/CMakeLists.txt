add_definitions(-DBUILDING_LIBCURL)
add_definitions(-DCURL_STATICLIB)

add_definitions(-DUSE_WINDOWS_SSPI)
add_definitions(-DUSE_SCHANNEL)
add_definitions(-DHTTP_ONLY)
add_definitions(-DCURL_DISABLE_CRYPTO_AUTH)

# ====================================================
if(FALSE) # fake a block comment

set(BUILD_CURL_EXE OFF CACHE BOOL "Set to ON to build cURL executable.")
set(BUILD_CURL_TESTS OFF CACHE BOOL "Set to ON to build cURL tests.")
set(CURL_STATICLIB ON CACHE BOOL "Set to ON to build libcurl with static linking.")

set(HTTP_ONLY ON CACHE BOOL "disables all protocols except HTTP (This overrides all CURL_DISABLE_* options)")
set(CURL_DISABLE_CRYPTO_AUTH ON CACHE BOOL "to disable cryptographic authentication")
set(CMAKE_USE_OPENSSL OFF CACHE BOOL "Use OpenSSL code. Experimental")
set(CMAKE_USE_LIBSSH2 OFF CACHE BOOL "Use libSSH2")
set(ENABLE_MANUAL OFF CACHE BOOL "to provide the built-in manual")

# BUILDING_LIBCURL;CURL_STATICLIB;USE_WINDOWS_SSPI;USE_SCHANNEL;HTTP_ONLY;CURL_DISABLE_CRYPTO_AUTH
# BUILDING_LIBCURL;CURL_STATICLIB;USE_WINDOWS_SSPI;USE_SCHANNEL;HTTP_ONLY;CURL_DISABLE_CRYPTO_AUTH;DEBUGBUILD;

endif()

# ====================================================

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

add_library(libcurl STATIC ${CPPSOURCES})

# http://stackoverflow.com/questions/1618927/cmake-microsoft-visual-studio-and-monolithic-runtimes
if(MSVC)
#We statically link to reduce dependancies
foreach(flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
    if(${flag_var} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MDd")
endforeach(flag_var)
endif(MSVC)
