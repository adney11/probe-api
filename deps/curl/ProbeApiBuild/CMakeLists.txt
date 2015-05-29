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

FILE(GLOB CPPSOURCES
#	../src/*.c
#	../src/*.h
	../lib/*.c
	../lib/*.h
	../lib/vtls/*.c
	../lib/vtls/*.h
	../include/curl/*.h
	)

add_library(libcurl STATIC ${CPPSOURCES})
