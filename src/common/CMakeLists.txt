FILE(GLOB CPPSOURCES *.cpp *.h *.ver)

add_library(common STATIC ${CPPSOURCES})

target_link_libraries(common jsoncpp)
target_link_libraries(common curlpp)

target_include_directories(common PUBLIC ..)

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_definitions(common PUBLIC -D_GLIBCXX_USE_NANOSLEEP)
endif()

# target_compile_features(common PUBLIC cxx_nonstatic_member_init)
# target_compile_features(common PUBLIC cxx_auto_type)
set_property(TARGET common PROPERTY CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCXX)
	# enable std::this_thread and std::this_thread::sleep_for:
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -D_GLIBCXX_USE_NANOSLEEP ")
endif()
