# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/peto/Documents/IZG/1project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/peto/Documents/IZG/1project/build

# Include any dependencies generated for this target.
include stack/CMakeFiles/stack.dir/depend.make

# Include the progress variables for this target.
include stack/CMakeFiles/stack.dir/progress.make

# Include the compile flags for this target's objects.
include stack/CMakeFiles/stack.dir/flags.make

stack/CMakeFiles/stack.dir/src/stack/stack.c.o: stack/CMakeFiles/stack.dir/flags.make
stack/CMakeFiles/stack.dir/src/stack/stack.c.o: ../stack/src/stack/stack.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/peto/Documents/IZG/1project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object stack/CMakeFiles/stack.dir/src/stack/stack.c.o"
	cd /Users/peto/Documents/IZG/1project/build/stack && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/stack.dir/src/stack/stack.c.o   -c /Users/peto/Documents/IZG/1project/stack/src/stack/stack.c

stack/CMakeFiles/stack.dir/src/stack/stack.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/stack.dir/src/stack/stack.c.i"
	cd /Users/peto/Documents/IZG/1project/build/stack && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/peto/Documents/IZG/1project/stack/src/stack/stack.c > CMakeFiles/stack.dir/src/stack/stack.c.i

stack/CMakeFiles/stack.dir/src/stack/stack.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/stack.dir/src/stack/stack.c.s"
	cd /Users/peto/Documents/IZG/1project/build/stack && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/peto/Documents/IZG/1project/stack/src/stack/stack.c -o CMakeFiles/stack.dir/src/stack/stack.c.s

# Object files for target stack
stack_OBJECTS = \
"CMakeFiles/stack.dir/src/stack/stack.c.o"

# External object files for target stack
stack_EXTERNAL_OBJECTS =

stack/libstack.a: stack/CMakeFiles/stack.dir/src/stack/stack.c.o
stack/libstack.a: stack/CMakeFiles/stack.dir/build.make
stack/libstack.a: stack/CMakeFiles/stack.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/peto/Documents/IZG/1project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libstack.a"
	cd /Users/peto/Documents/IZG/1project/build/stack && $(CMAKE_COMMAND) -P CMakeFiles/stack.dir/cmake_clean_target.cmake
	cd /Users/peto/Documents/IZG/1project/build/stack && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stack.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
stack/CMakeFiles/stack.dir/build: stack/libstack.a

.PHONY : stack/CMakeFiles/stack.dir/build

stack/CMakeFiles/stack.dir/clean:
	cd /Users/peto/Documents/IZG/1project/build/stack && $(CMAKE_COMMAND) -P CMakeFiles/stack.dir/cmake_clean.cmake
.PHONY : stack/CMakeFiles/stack.dir/clean

stack/CMakeFiles/stack.dir/depend:
	cd /Users/peto/Documents/IZG/1project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/peto/Documents/IZG/1project /Users/peto/Documents/IZG/1project/stack /Users/peto/Documents/IZG/1project/build /Users/peto/Documents/IZG/1project/build/stack /Users/peto/Documents/IZG/1project/build/stack/CMakeFiles/stack.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : stack/CMakeFiles/stack.dir/depend
