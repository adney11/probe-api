include_directories(../include)
include_directories(../../curl/include)

add_definitions(-DCURL_STATICLIB)
add_definitions(-DCURLPP_STATICLIB)

FILE(GLOB CPPSOURCES
	../src/curlpp/*.cpp
	../src/curlpp/*.h
	../src/curlpp/*.ins
	../src/curlpp/internal/*.cpp
	../src/utilspp/*.cpp
	../include/curlpp/*.h
	../include/curlpp/*.hpp
	../include/curlpp/*.inl
	../include/curlpp/internal/*.h
	../include/curlpp/internal/*.hpp
	../include/curlpp/internal/*.inl
	)

add_library(curlpp STATIC ${CPPSOURCES})
