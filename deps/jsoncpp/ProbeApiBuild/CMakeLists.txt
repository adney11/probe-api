include_directories(../include)

FILE(GLOB CPPSOURCES
	../src/lib_json/*.cpp
	../src/lib_json/*.h
	../src/lib_json/*.inl
	../include/json/*.h
	)

add_library(jsoncpp STATIC ${CPPSOURCES})
