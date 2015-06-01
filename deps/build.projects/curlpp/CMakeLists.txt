FILE(GLOB CPPSOURCES
	${DEPS_DIR}/curlpp/src/curlpp/*.cpp
	${DEPS_DIR}/curlpp/src/curlpp/*.h
#	${DEPS_DIR}/curlpp/src/curlpp/*.ins
	${DEPS_DIR}/curlpp/src/curlpp/internal/*.cpp
	${DEPS_DIR}/curlpp/src/utilspp/*.cpp
	${DEPS_DIR}/curlpp/include/curlpp/*.h
	${DEPS_DIR}/curlpp/include/curlpp/*.hpp
	${DEPS_DIR}/curlpp/include/curlpp/*.inl
	${DEPS_DIR}/curlpp/include/curlpp/internal/*.h
	${DEPS_DIR}/curlpp/include/curlpp/internal/*.hpp
	${DEPS_DIR}/curlpp/include/curlpp/internal/*.inl
	)

add_library(curlpp STATIC ${CPPSOURCES})
target_link_libraries(curlpp curl)

target_include_directories(curlpp PUBLIC ${DEPS_DIR}/curlpp/include)

target_compile_definitions(curlpp PUBLIC -DCURLPP_STATICLIB)
