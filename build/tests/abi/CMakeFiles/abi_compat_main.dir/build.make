# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/OUUDHAOUDHAUGF/json

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/OUUDHAOUDHAUGF/build

# Include any dependencies generated for this target.
include tests/abi/CMakeFiles/abi_compat_main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/abi/CMakeFiles/abi_compat_main.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/abi/CMakeFiles/abi_compat_main.dir/progress.make

# Include the compile flags for this target's objects.
include tests/abi/CMakeFiles/abi_compat_main.dir/flags.make

tests/abi/CMakeFiles/abi_compat_main.dir/main.cpp.o: tests/abi/CMakeFiles/abi_compat_main.dir/flags.make
tests/abi/CMakeFiles/abi_compat_main.dir/main.cpp.o: /root/OUUDHAOUDHAUGF/json/tests/abi/main.cpp
tests/abi/CMakeFiles/abi_compat_main.dir/main.cpp.o: tests/abi/CMakeFiles/abi_compat_main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/OUUDHAOUDHAUGF/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/abi/CMakeFiles/abi_compat_main.dir/main.cpp.o"
	cd /root/OUUDHAOUDHAUGF/build/tests/abi && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/abi/CMakeFiles/abi_compat_main.dir/main.cpp.o -MF CMakeFiles/abi_compat_main.dir/main.cpp.o.d -o CMakeFiles/abi_compat_main.dir/main.cpp.o -c /root/OUUDHAOUDHAUGF/json/tests/abi/main.cpp

tests/abi/CMakeFiles/abi_compat_main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/abi_compat_main.dir/main.cpp.i"
	cd /root/OUUDHAOUDHAUGF/build/tests/abi && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/OUUDHAOUDHAUGF/json/tests/abi/main.cpp > CMakeFiles/abi_compat_main.dir/main.cpp.i

tests/abi/CMakeFiles/abi_compat_main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/abi_compat_main.dir/main.cpp.s"
	cd /root/OUUDHAOUDHAUGF/build/tests/abi && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/OUUDHAOUDHAUGF/json/tests/abi/main.cpp -o CMakeFiles/abi_compat_main.dir/main.cpp.s

# Object files for target abi_compat_main
abi_compat_main_OBJECTS = \
"CMakeFiles/abi_compat_main.dir/main.cpp.o"

# External object files for target abi_compat_main
abi_compat_main_EXTERNAL_OBJECTS =

tests/abi/libabi_compat_main.a: tests/abi/CMakeFiles/abi_compat_main.dir/main.cpp.o
tests/abi/libabi_compat_main.a: tests/abi/CMakeFiles/abi_compat_main.dir/build.make
tests/abi/libabi_compat_main.a: tests/abi/CMakeFiles/abi_compat_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/OUUDHAOUDHAUGF/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libabi_compat_main.a"
	cd /root/OUUDHAOUDHAUGF/build/tests/abi && $(CMAKE_COMMAND) -P CMakeFiles/abi_compat_main.dir/cmake_clean_target.cmake
	cd /root/OUUDHAOUDHAUGF/build/tests/abi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/abi_compat_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/abi/CMakeFiles/abi_compat_main.dir/build: tests/abi/libabi_compat_main.a
.PHONY : tests/abi/CMakeFiles/abi_compat_main.dir/build

tests/abi/CMakeFiles/abi_compat_main.dir/clean:
	cd /root/OUUDHAOUDHAUGF/build/tests/abi && $(CMAKE_COMMAND) -P CMakeFiles/abi_compat_main.dir/cmake_clean.cmake
.PHONY : tests/abi/CMakeFiles/abi_compat_main.dir/clean

tests/abi/CMakeFiles/abi_compat_main.dir/depend:
	cd /root/OUUDHAOUDHAUGF/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/OUUDHAOUDHAUGF/json /root/OUUDHAOUDHAUGF/json/tests/abi /root/OUUDHAOUDHAUGF/build /root/OUUDHAOUDHAUGF/build/tests/abi /root/OUUDHAOUDHAUGF/build/tests/abi/CMakeFiles/abi_compat_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/abi/CMakeFiles/abi_compat_main.dir/depend

