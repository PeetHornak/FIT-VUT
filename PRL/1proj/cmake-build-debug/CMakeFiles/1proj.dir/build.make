# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/peto/Documents/FIT-NO-GIT/PRL/1proj

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/peto/Documents/FIT-NO-GIT/PRL/1proj/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/1proj.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/1proj.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/1proj.dir/flags.make

CMakeFiles/1proj.dir/oets.cpp.o: CMakeFiles/1proj.dir/flags.make
CMakeFiles/1proj.dir/oets.cpp.o: ../oets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/peto/Documents/FIT-NO-GIT/PRL/1proj/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/1proj.dir/oets.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/1proj.dir/oets.cpp.o -c /Users/peto/Documents/FIT-NO-GIT/PRL/1proj/oets.cpp

CMakeFiles/1proj.dir/oets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/1proj.dir/oets.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/peto/Documents/FIT-NO-GIT/PRL/1proj/oets.cpp > CMakeFiles/1proj.dir/oets.cpp.i

CMakeFiles/1proj.dir/oets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/1proj.dir/oets.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/peto/Documents/FIT-NO-GIT/PRL/1proj/oets.cpp -o CMakeFiles/1proj.dir/oets.cpp.s

# Object files for target 1proj
1proj_OBJECTS = \
"CMakeFiles/1proj.dir/oets.cpp.o"

# External object files for target 1proj
1proj_EXTERNAL_OBJECTS =

1proj: CMakeFiles/1proj.dir/oets.cpp.o
1proj: CMakeFiles/1proj.dir/build.make
1proj: CMakeFiles/1proj.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/peto/Documents/FIT-NO-GIT/PRL/1proj/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 1proj"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/1proj.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/1proj.dir/build: 1proj

.PHONY : CMakeFiles/1proj.dir/build

CMakeFiles/1proj.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/1proj.dir/cmake_clean.cmake
.PHONY : CMakeFiles/1proj.dir/clean

CMakeFiles/1proj.dir/depend:
	cd /Users/peto/Documents/FIT-NO-GIT/PRL/1proj/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/peto/Documents/FIT-NO-GIT/PRL/1proj /Users/peto/Documents/FIT-NO-GIT/PRL/1proj /Users/peto/Documents/FIT-NO-GIT/PRL/1proj/cmake-build-debug /Users/peto/Documents/FIT-NO-GIT/PRL/1proj/cmake-build-debug /Users/peto/Documents/FIT-NO-GIT/PRL/1proj/cmake-build-debug/CMakeFiles/1proj.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/1proj.dir/depend
