if(WIN32)
	set(USE_WINSSL 1)
endif()
# set(USE_ZLIB 1)

add_definitions(-DBUILDING_LIBCURL)
add_definitions(-DCURL_STATICLIB)

if(USE_WINSSL)
	add_definitions(-DUSE_WINDOWS_SSPI)
	add_definitions(-DUSE_SCHANNEL)
else()
	add_definitions(-DUSE_OPENSSL)
endif()

add_definitions(-DHTTP_ONLY)
add_definitions(-DCURL_DISABLE_CRYPTO_AUTH)
if(USE_ZLIB)
	add_definitions(-DHAVE_ZLIB_H)
	add_definitions(-DHAVE_ZLIB)
	add_definitions(-DHAVE_LIBZ)
endif()

FILE(GLOB CPPSOURCES
#	../../curl/src/*.c
#	../../curl/src/*.h
	../../curl/lib/*.c
	../../curl/lib/*.h
	../../curl/lib/vtls/*.c
	../../curl/lib/vtls/*.h
	../../curl/include/curl/*.h
	)

add_library(curl STATIC ${CPPSOURCES})

if(USE_ZLIB)
	target_link_libraries(curl zlib)
endif()
if(NOT USE_WINSSL)
	target_link_libraries(curl openssl)
	add_dependencies(curl openssl)	# OpenSSL should prepare its include folder during pre-build step
endif()
if(WIN32)
  target_link_libraries(curl ws2_32)
endif()

target_include_directories(curl PUBLIC ../../curl/include)
target_include_directories(curl PRIVATE ../../curl/lib)
