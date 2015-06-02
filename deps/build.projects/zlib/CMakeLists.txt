FILE(GLOB CPPSOURCES
	${DEPS_DIR}/zlib/*.c
	${DEPS_DIR}/zlib/*.h
	)

add_library(zlib STATIC ${CPPSOURCES})

target_include_directories(curl PUBLIC ${DEPS_DIR}/zlib)

if(MSVC)
	target_compile_definitions(zlib PRIVATE -D_CRT_NONSTDC_NO_DEPRECATE)
	target_compile_definitions(zlib PRIVATE -D_CRT_SECURE_NO_DEPRECATE)
	target_compile_definitions(zlib PRIVATE -D_CRT_NONSTDC_NO_WARNINGS)
endif()
