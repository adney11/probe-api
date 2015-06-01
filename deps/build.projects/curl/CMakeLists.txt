include_directories(../../curl/include)
include_directories(../../curl/lib)
include_directories(../../zlib)

add_definitions(-DBUILDING_LIBCURL)
add_definitions(-DCURL_STATICLIB)

if(WIN32)
	add_definitions(-DUSE_WINDOWS_SSPI)
	add_definitions(-DUSE_SCHANNEL)
else()
	add_definitions(-DUSE_OPENSSL)
	include_directories(../../openssl/inc32)
endif()

add_definitions(-DHTTP_ONLY)
add_definitions(-DCURL_DISABLE_CRYPTO_AUTH)
add_definitions(-DHAVE_ZLIB_H)
add_definitions(-DHAVE_ZLIB)
add_definitions(-DHAVE_LIBZ)

FILE(GLOB CPPSOURCES
#	../../curl/src/*.c
#	../../curl/src/*.h
	../../curl/lib/*.c
	../../curl/lib/*.h
	../../curl/lib/vtls/*.c
	../../curl/lib/vtls/*.h
	../../curl/include/curl/*.h
	)

add_library(libcurl STATIC ${CPPSOURCES})
