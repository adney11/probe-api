FILE(GLOB CPPSOURCES
	${DEPS_DIR}/jsoncpp/src/lib_json/*.cpp
	${DEPS_DIR}/jsoncpp/src/lib_json/*.h
	${DEPS_DIR}/jsoncpp/src/lib_json/*.inl
	${DEPS_DIR}/jsoncpp/include/json/*.h
	)

add_library(jsoncpp STATIC ${CPPSOURCES})

set_property(TARGET jsoncpp PROPERTY CXX_STANDARD 11)

target_include_directories(jsoncpp PUBLIC ${DEPS_DIR}/jsoncpp/include)

if(CMAKE_COMPILER_IS_GNUCXX AND WIN32)
	# fix bug in Windows MinGW gcc compiler:
	target_compile_definitions(jsoncpp PRIVATE -D__cplusplus=201102)
endif()
