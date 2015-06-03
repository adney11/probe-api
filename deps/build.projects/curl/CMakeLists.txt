if(WIN32 OR CYGWIN)
#	set(USE_WINSSL 1)
endif()
set(USE_ZLIB 1)

FILE(GLOB CPPSOURCES
#	${DEPS_DIR}/curl/src/*.c
#	${DEPS_DIR}/curl/src/*.h
	${DEPS_DIR}/curl/lib/*.c
	${DEPS_DIR}/curl/lib/*.h
	${DEPS_DIR}/curl/lib/vtls/*.c
	${DEPS_DIR}/curl/lib/vtls/*.h
	${DEPS_DIR}/curl/include/curl/*.h
	)

add_library(curl STATIC ${CPPSOURCES})

if(USE_ZLIB)
	target_link_libraries(curl PRIVATE zlib)
endif()
if(NOT USE_WINSSL)
	target_link_libraries(curl PRIVATE openssl)
endif()
if(WIN32 OR CYGWIN)
  target_link_libraries(curl PRIVATE ws2_32)
endif()

target_include_directories(curl PUBLIC ${DEPS_DIR}/curl/include)
target_include_directories(curl PRIVATE ${DEPS_DIR}/curl/lib)
target_include_directories(curl PRIVATE ${DEPS_DIR}/build.projects)
target_include_directories(curl PUBLIC ${DEPS_DIR}/build.projects/curl)

target_compile_definitions(curl PUBLIC -DCURL_STATICLIB)
target_compile_definitions(curl PRIVATE -DBUILDING_LIBCURL)

if(USE_WINSSL)
	target_compile_definitions(curl PRIVATE -DUSE_WINDOWS_SSPI)
	target_compile_definitions(curl PRIVATE -DUSE_SCHANNEL)
else()
	target_compile_definitions(curl PRIVATE -DUSE_OPENSSL)
endif()

target_compile_definitions(curl PRIVATE -DHTTP_ONLY)
target_compile_definitions(curl PRIVATE -DCURL_DISABLE_CRYPTO_AUTH)
if(USE_ZLIB)
	target_compile_definitions(curl PRIVATE -DHAVE_ZLIB_H)
	target_compile_definitions(curl PRIVATE -DHAVE_ZLIB)
	target_compile_definitions(curl PRIVATE -DHAVE_LIBZ)
endif()
