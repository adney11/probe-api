include_directories(../../jsoncpp/include)

FILE(GLOB CPPSOURCES
	../../jsoncpp/src/lib_json/*.cpp
	../../jsoncpp/src/lib_json/*.h
	../../jsoncpp/src/lib_json/*.inl
	../../jsoncpp/include/json/*.h
	)

add_library(jsoncpp STATIC ${CPPSOURCES})
