cmake_minimum_required(VERSION 2.6)

project(ProbeApiTools)

# =================================================

# http://stackoverflow.com/questions/1618927/cmake-microsoft-visual-studio-and-monolithic-runtimes
if(MSVC)
#We statically link to reduce dependencies
foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
		CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
    if(${flag_var} MATCHES "/MDd")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MDd")
endforeach(flag_var)
endif(MSVC)

if(MSVC)
	set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /O2 /Os /GL /Gy")
	set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /O2 /Os /GL /Gy")
	set(CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL "${CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL} /LTCG")
	set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /OPT:REF /OPT:ICF /LTCG")
endif(MSVC)

# =================================================

add_subdirectory(deps/curl/ProbeApiBuild)
add_subdirectory(deps/curlpp/ProbeApiBuild)
add_subdirectory(deps/jsoncpp/ProbeApiBuild)

add_subdirectory(src/common)

add_subdirectory(src/dping)
add_subdirectory(src/dtraceroute)

# =================================================
