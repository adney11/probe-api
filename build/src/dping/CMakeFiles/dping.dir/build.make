# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/adney11/probe-api

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/adney11/probe-api/build

# Include any dependencies generated for this target.
include src/dping/CMakeFiles/dping.dir/depend.make

# Include the progress variables for this target.
include src/dping/CMakeFiles/dping.dir/progress.make

# Include the compile flags for this target's objects.
include src/dping/CMakeFiles/dping.dir/flags.make

src/dping/CMakeFiles/dping.dir/dping.cpp.o: src/dping/CMakeFiles/dping.dir/flags.make
src/dping/CMakeFiles/dping.dir/dping.cpp.o: ../src/dping/dping.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/adney11/probe-api/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/dping/CMakeFiles/dping.dir/dping.cpp.o"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dping.dir/dping.cpp.o -c /home/adney11/probe-api/src/dping/dping.cpp

src/dping/CMakeFiles/dping.dir/dping.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dping.dir/dping.cpp.i"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adney11/probe-api/src/dping/dping.cpp > CMakeFiles/dping.dir/dping.cpp.i

src/dping/CMakeFiles/dping.dir/dping.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dping.dir/dping.cpp.s"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adney11/probe-api/src/dping/dping.cpp -o CMakeFiles/dping.dir/dping.cpp.s

src/dping/CMakeFiles/dping.dir/dping.cpp.o.requires:

.PHONY : src/dping/CMakeFiles/dping.dir/dping.cpp.o.requires

src/dping/CMakeFiles/dping.dir/dping.cpp.o.provides: src/dping/CMakeFiles/dping.dir/dping.cpp.o.requires
	$(MAKE) -f src/dping/CMakeFiles/dping.dir/build.make src/dping/CMakeFiles/dping.dir/dping.cpp.o.provides.build
.PHONY : src/dping/CMakeFiles/dping.dir/dping.cpp.o.provides

src/dping/CMakeFiles/dping.dir/dping.cpp.o.provides.build: src/dping/CMakeFiles/dping.dir/dping.cpp.o


src/dping/CMakeFiles/dping.dir/main.cpp.o: src/dping/CMakeFiles/dping.dir/flags.make
src/dping/CMakeFiles/dping.dir/main.cpp.o: ../src/dping/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/adney11/probe-api/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/dping/CMakeFiles/dping.dir/main.cpp.o"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dping.dir/main.cpp.o -c /home/adney11/probe-api/src/dping/main.cpp

src/dping/CMakeFiles/dping.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dping.dir/main.cpp.i"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adney11/probe-api/src/dping/main.cpp > CMakeFiles/dping.dir/main.cpp.i

src/dping/CMakeFiles/dping.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dping.dir/main.cpp.s"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adney11/probe-api/src/dping/main.cpp -o CMakeFiles/dping.dir/main.cpp.s

src/dping/CMakeFiles/dping.dir/main.cpp.o.requires:

.PHONY : src/dping/CMakeFiles/dping.dir/main.cpp.o.requires

src/dping/CMakeFiles/dping.dir/main.cpp.o.provides: src/dping/CMakeFiles/dping.dir/main.cpp.o.requires
	$(MAKE) -f src/dping/CMakeFiles/dping.dir/build.make src/dping/CMakeFiles/dping.dir/main.cpp.o.provides.build
.PHONY : src/dping/CMakeFiles/dping.dir/main.cpp.o.provides

src/dping/CMakeFiles/dping.dir/main.cpp.o.provides.build: src/dping/CMakeFiles/dping.dir/main.cpp.o


src/dping/CMakeFiles/dping.dir/options.cpp.o: src/dping/CMakeFiles/dping.dir/flags.make
src/dping/CMakeFiles/dping.dir/options.cpp.o: ../src/dping/options.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/adney11/probe-api/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/dping/CMakeFiles/dping.dir/options.cpp.o"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dping.dir/options.cpp.o -c /home/adney11/probe-api/src/dping/options.cpp

src/dping/CMakeFiles/dping.dir/options.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dping.dir/options.cpp.i"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adney11/probe-api/src/dping/options.cpp > CMakeFiles/dping.dir/options.cpp.i

src/dping/CMakeFiles/dping.dir/options.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dping.dir/options.cpp.s"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adney11/probe-api/src/dping/options.cpp -o CMakeFiles/dping.dir/options.cpp.s

src/dping/CMakeFiles/dping.dir/options.cpp.o.requires:

.PHONY : src/dping/CMakeFiles/dping.dir/options.cpp.o.requires

src/dping/CMakeFiles/dping.dir/options.cpp.o.provides: src/dping/CMakeFiles/dping.dir/options.cpp.o.requires
	$(MAKE) -f src/dping/CMakeFiles/dping.dir/build.make src/dping/CMakeFiles/dping.dir/options.cpp.o.provides.build
.PHONY : src/dping/CMakeFiles/dping.dir/options.cpp.o.provides

src/dping/CMakeFiles/dping.dir/options.cpp.o.provides.build: src/dping/CMakeFiles/dping.dir/options.cpp.o


src/dping/CMakeFiles/dping.dir/stdafx.cpp.o: src/dping/CMakeFiles/dping.dir/flags.make
src/dping/CMakeFiles/dping.dir/stdafx.cpp.o: ../src/dping/stdafx.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/adney11/probe-api/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/dping/CMakeFiles/dping.dir/stdafx.cpp.o"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dping.dir/stdafx.cpp.o -c /home/adney11/probe-api/src/dping/stdafx.cpp

src/dping/CMakeFiles/dping.dir/stdafx.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dping.dir/stdafx.cpp.i"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adney11/probe-api/src/dping/stdafx.cpp > CMakeFiles/dping.dir/stdafx.cpp.i

src/dping/CMakeFiles/dping.dir/stdafx.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dping.dir/stdafx.cpp.s"
	cd /home/adney11/probe-api/build/src/dping && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adney11/probe-api/src/dping/stdafx.cpp -o CMakeFiles/dping.dir/stdafx.cpp.s

src/dping/CMakeFiles/dping.dir/stdafx.cpp.o.requires:

.PHONY : src/dping/CMakeFiles/dping.dir/stdafx.cpp.o.requires

src/dping/CMakeFiles/dping.dir/stdafx.cpp.o.provides: src/dping/CMakeFiles/dping.dir/stdafx.cpp.o.requires
	$(MAKE) -f src/dping/CMakeFiles/dping.dir/build.make src/dping/CMakeFiles/dping.dir/stdafx.cpp.o.provides.build
.PHONY : src/dping/CMakeFiles/dping.dir/stdafx.cpp.o.provides

src/dping/CMakeFiles/dping.dir/stdafx.cpp.o.provides.build: src/dping/CMakeFiles/dping.dir/stdafx.cpp.o


# Object files for target dping
dping_OBJECTS = \
"CMakeFiles/dping.dir/dping.cpp.o" \
"CMakeFiles/dping.dir/main.cpp.o" \
"CMakeFiles/dping.dir/options.cpp.o" \
"CMakeFiles/dping.dir/stdafx.cpp.o"

# External object files for target dping
dping_EXTERNAL_OBJECTS =

../bin/dping: src/dping/CMakeFiles/dping.dir/dping.cpp.o
../bin/dping: src/dping/CMakeFiles/dping.dir/main.cpp.o
../bin/dping: src/dping/CMakeFiles/dping.dir/options.cpp.o
../bin/dping: src/dping/CMakeFiles/dping.dir/stdafx.cpp.o
../bin/dping: src/dping/CMakeFiles/dping.dir/build.make
../bin/dping: src/common/libcommon.a
../bin/dping: deps/build.projects/jsoncpp/libjsoncpp.a
../bin/dping: deps/build.projects/curlpp/libcurlpp.a
../bin/dping: /usr/lib/x86_64-linux-gnu/libcurl.so
../bin/dping: src/dping/CMakeFiles/dping.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/adney11/probe-api/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ../../../bin/dping"
	cd /home/adney11/probe-api/build/src/dping && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dping.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/dping/CMakeFiles/dping.dir/build: ../bin/dping

.PHONY : src/dping/CMakeFiles/dping.dir/build

src/dping/CMakeFiles/dping.dir/requires: src/dping/CMakeFiles/dping.dir/dping.cpp.o.requires
src/dping/CMakeFiles/dping.dir/requires: src/dping/CMakeFiles/dping.dir/main.cpp.o.requires
src/dping/CMakeFiles/dping.dir/requires: src/dping/CMakeFiles/dping.dir/options.cpp.o.requires
src/dping/CMakeFiles/dping.dir/requires: src/dping/CMakeFiles/dping.dir/stdafx.cpp.o.requires

.PHONY : src/dping/CMakeFiles/dping.dir/requires

src/dping/CMakeFiles/dping.dir/clean:
	cd /home/adney11/probe-api/build/src/dping && $(CMAKE_COMMAND) -P CMakeFiles/dping.dir/cmake_clean.cmake
.PHONY : src/dping/CMakeFiles/dping.dir/clean

src/dping/CMakeFiles/dping.dir/depend:
	cd /home/adney11/probe-api/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/adney11/probe-api /home/adney11/probe-api/src/dping /home/adney11/probe-api/build /home/adney11/probe-api/build/src/dping /home/adney11/probe-api/build/src/dping/CMakeFiles/dping.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/dping/CMakeFiles/dping.dir/depend

