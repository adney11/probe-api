add_definitions(-DCURL_STATICLIB)
add_definitions(-DCURLPP_STATICLIB)

FILE(GLOB CPPSOURCES
	../../curlpp/src/curlpp/*.cpp
	../../curlpp/src/curlpp/*.h
	../../curlpp/src/curlpp/*.ins
	../../curlpp/src/curlpp/internal/*.cpp
	../../curlpp/src/utilspp/*.cpp
	../../curlpp/include/curlpp/*.h
	../../curlpp/include/curlpp/*.hpp
	../../curlpp/include/curlpp/*.inl
	../../curlpp/include/curlpp/internal/*.h
	../../curlpp/include/curlpp/internal/*.hpp
	../../curlpp/include/curlpp/internal/*.inl
	)

add_library(curlpp STATIC ${CPPSOURCES})
target_link_libraries(curlpp curl)

target_include_directories(curlpp PUBLIC ../../curlpp/include)
