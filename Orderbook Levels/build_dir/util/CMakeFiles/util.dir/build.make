# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/shermanding/QF633/L1_workspace/assignment_4/project/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir

# Include any dependencies generated for this target.
include util/CMakeFiles/util.dir/depend.make

# Include the progress variables for this target.
include util/CMakeFiles/util.dir/progress.make

# Include the compile flags for this target's objects.
include util/CMakeFiles/util.dir/flags.make

util/CMakeFiles/util.dir/incremental_csv_reader.cpp.o: util/CMakeFiles/util.dir/flags.make
util/CMakeFiles/util.dir/incremental_csv_reader.cpp.o: ../util/incremental_csv_reader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object util/CMakeFiles/util.dir/incremental_csv_reader.cpp.o"
	cd /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/util && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/util.dir/incremental_csv_reader.cpp.o -c /home/shermanding/QF633/L1_workspace/assignment_4/project/src/util/incremental_csv_reader.cpp

util/CMakeFiles/util.dir/incremental_csv_reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/util.dir/incremental_csv_reader.cpp.i"
	cd /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/util && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/shermanding/QF633/L1_workspace/assignment_4/project/src/util/incremental_csv_reader.cpp > CMakeFiles/util.dir/incremental_csv_reader.cpp.i

util/CMakeFiles/util.dir/incremental_csv_reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/util.dir/incremental_csv_reader.cpp.s"
	cd /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/util && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/shermanding/QF633/L1_workspace/assignment_4/project/src/util/incremental_csv_reader.cpp -o CMakeFiles/util.dir/incremental_csv_reader.cpp.s

# Object files for target util
util_OBJECTS = \
"CMakeFiles/util.dir/incremental_csv_reader.cpp.o"

# External object files for target util
util_EXTERNAL_OBJECTS =

util/libutil.a: util/CMakeFiles/util.dir/incremental_csv_reader.cpp.o
util/libutil.a: util/CMakeFiles/util.dir/build.make
util/libutil.a: util/CMakeFiles/util.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libutil.a"
	cd /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/util && $(CMAKE_COMMAND) -P CMakeFiles/util.dir/cmake_clean_target.cmake
	cd /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/util && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/util.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
util/CMakeFiles/util.dir/build: util/libutil.a

.PHONY : util/CMakeFiles/util.dir/build

util/CMakeFiles/util.dir/clean:
	cd /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/util && $(CMAKE_COMMAND) -P CMakeFiles/util.dir/cmake_clean.cmake
.PHONY : util/CMakeFiles/util.dir/clean

util/CMakeFiles/util.dir/depend:
	cd /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shermanding/QF633/L1_workspace/assignment_4/project/src /home/shermanding/QF633/L1_workspace/assignment_4/project/src/util /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/util /home/shermanding/QF633/L1_workspace/assignment_4/project/src/build_dir/util/CMakeFiles/util.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : util/CMakeFiles/util.dir/depend

