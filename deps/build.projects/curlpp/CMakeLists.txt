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

set_property(TARGET curlpp PROPERTY CXX_STANDARD 11)

if(WIN32)
	target_link_libraries(curlpp curl)
else()
	#find_package(CURL)
	if(NOT CURL_FOUND)
		message(FATAL_ERROR "Could not find the CURL library and development files.")
	endif()
	target_link_libraries(curlpp ${CURL_LIBRARIES})
	include_directories(${CURL_INCLUDE_DIRS})
	#target_compile_definitions(curlpp PRIVATE -DCURL_STATICLIB)
endif()

target_include_directories(curlpp PUBLIC ${DEPS_DIR}/curlpp/include)
target_include_directories(curlpp PUBLIC ${DEPS_DIR}/build.projects)	# for curlpp/config.h

target_compile_definitions(curlpp PUBLIC -DCURLPP_STATICLIB)
target_compile_definitions(curlpp PUBLIC -DHAVE_CONFIG_H)				# disable usage of config.win32.h which generates lots of warnings in gcc on linux

#find_package(Boost)
if(Boost_FOUND)
	message(STATUS "Using boost ${Boost_VERSION} in curlpp from ${Boost_INCLUDE_DIRS}")
	target_link_libraries(curlpp ${Boost_LIBRARIES})
	target_include_directories(curlpp PUBLIC ${Boost_INCLUDE_DIRS})
	target_compile_definitions(curlpp PRIVATE -DHAVE_BOOST)
endif()
