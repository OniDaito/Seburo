# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oni/Projects/S9Gear

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oni/Projects/S9Gear/build

# Include any dependencies generated for this target.
include CMakeFiles/s9gear.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/s9gear.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/s9gear.dir/flags.make

CMakeFiles/s9gear.dir/src/shapes.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/shapes.cpp.o: ../src/shapes.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/shapes.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/shapes.cpp.o -c /home/oni/Projects/S9Gear/src/shapes.cpp

CMakeFiles/s9gear.dir/src/shapes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/shapes.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/shapes.cpp > CMakeFiles/s9gear.dir/src/shapes.cpp.i

CMakeFiles/s9gear.dir/src/shapes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/shapes.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/shapes.cpp -o CMakeFiles/s9gear.dir/src/shapes.cpp.s

CMakeFiles/s9gear.dir/src/shapes.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/shapes.cpp.o.requires

CMakeFiles/s9gear.dir/src/shapes.cpp.o.provides: CMakeFiles/s9gear.dir/src/shapes.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/shapes.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/shapes.cpp.o.provides

CMakeFiles/s9gear.dir/src/shapes.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/shapes.cpp.o

CMakeFiles/s9gear.dir/src/asset.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/asset.cpp.o: ../src/asset.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/asset.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/asset.cpp.o -c /home/oni/Projects/S9Gear/src/asset.cpp

CMakeFiles/s9gear.dir/src/asset.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/asset.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/asset.cpp > CMakeFiles/s9gear.dir/src/asset.cpp.i

CMakeFiles/s9gear.dir/src/asset.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/asset.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/asset.cpp -o CMakeFiles/s9gear.dir/src/asset.cpp.s

CMakeFiles/s9gear.dir/src/asset.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/asset.cpp.o.requires

CMakeFiles/s9gear.dir/src/asset.cpp.o.provides: CMakeFiles/s9gear.dir/src/asset.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/asset.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/asset.cpp.o.provides

CMakeFiles/s9gear.dir/src/asset.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/asset.cpp.o

CMakeFiles/s9gear.dir/src/geometry.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/geometry.cpp.o: ../src/geometry.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/geometry.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/geometry.cpp.o -c /home/oni/Projects/S9Gear/src/geometry.cpp

CMakeFiles/s9gear.dir/src/geometry.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/geometry.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/geometry.cpp > CMakeFiles/s9gear.dir/src/geometry.cpp.i

CMakeFiles/s9gear.dir/src/geometry.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/geometry.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/geometry.cpp -o CMakeFiles/s9gear.dir/src/geometry.cpp.s

CMakeFiles/s9gear.dir/src/geometry.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/geometry.cpp.o.requires

CMakeFiles/s9gear.dir/src/geometry.cpp.o.provides: CMakeFiles/s9gear.dir/src/geometry.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/geometry.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/geometry.cpp.o.provides

CMakeFiles/s9gear.dir/src/geometry.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/geometry.cpp.o

CMakeFiles/s9gear.dir/src/tinyxml.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/tinyxml.cpp.o: ../src/tinyxml.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/tinyxml.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/tinyxml.cpp.o -c /home/oni/Projects/S9Gear/src/tinyxml.cpp

CMakeFiles/s9gear.dir/src/tinyxml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/tinyxml.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/tinyxml.cpp > CMakeFiles/s9gear.dir/src/tinyxml.cpp.i

CMakeFiles/s9gear.dir/src/tinyxml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/tinyxml.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/tinyxml.cpp -o CMakeFiles/s9gear.dir/src/tinyxml.cpp.s

CMakeFiles/s9gear.dir/src/tinyxml.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/tinyxml.cpp.o.requires

CMakeFiles/s9gear.dir/src/tinyxml.cpp.o.provides: CMakeFiles/s9gear.dir/src/tinyxml.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/tinyxml.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/tinyxml.cpp.o.provides

CMakeFiles/s9gear.dir/src/tinyxml.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/tinyxml.cpp.o

CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o: ../src/tinyxmlparser.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o -c /home/oni/Projects/S9Gear/src/tinyxmlparser.cpp

CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/tinyxmlparser.cpp > CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.i

CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/tinyxmlparser.cpp -o CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.s

CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o.requires

CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o.provides: CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o.provides

CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o

CMakeFiles/s9gear.dir/src/camera.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/camera.cpp.o: ../src/camera.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/camera.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/camera.cpp.o -c /home/oni/Projects/S9Gear/src/camera.cpp

CMakeFiles/s9gear.dir/src/camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/camera.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/camera.cpp > CMakeFiles/s9gear.dir/src/camera.cpp.i

CMakeFiles/s9gear.dir/src/camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/camera.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/camera.cpp -o CMakeFiles/s9gear.dir/src/camera.cpp.s

CMakeFiles/s9gear.dir/src/camera.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/camera.cpp.o.requires

CMakeFiles/s9gear.dir/src/camera.cpp.o.provides: CMakeFiles/s9gear.dir/src/camera.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/camera.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/camera.cpp.o.provides

CMakeFiles/s9gear.dir/src/camera.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/camera.cpp.o

CMakeFiles/s9gear.dir/src/tinystr.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/tinystr.cpp.o: ../src/tinystr.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/tinystr.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/tinystr.cpp.o -c /home/oni/Projects/S9Gear/src/tinystr.cpp

CMakeFiles/s9gear.dir/src/tinystr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/tinystr.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/tinystr.cpp > CMakeFiles/s9gear.dir/src/tinystr.cpp.i

CMakeFiles/s9gear.dir/src/tinystr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/tinystr.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/tinystr.cpp -o CMakeFiles/s9gear.dir/src/tinystr.cpp.s

CMakeFiles/s9gear.dir/src/tinystr.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/tinystr.cpp.o.requires

CMakeFiles/s9gear.dir/src/tinystr.cpp.o.provides: CMakeFiles/s9gear.dir/src/tinystr.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/tinystr.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/tinystr.cpp.o.provides

CMakeFiles/s9gear.dir/src/tinystr.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/tinystr.cpp.o

CMakeFiles/s9gear.dir/src/s9xml.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/s9xml.cpp.o: ../src/s9xml.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/s9xml.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/s9xml.cpp.o -c /home/oni/Projects/S9Gear/src/s9xml.cpp

CMakeFiles/s9gear.dir/src/s9xml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/s9xml.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/s9xml.cpp > CMakeFiles/s9gear.dir/src/s9xml.cpp.i

CMakeFiles/s9gear.dir/src/s9xml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/s9xml.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/s9xml.cpp -o CMakeFiles/s9gear.dir/src/s9xml.cpp.s

CMakeFiles/s9gear.dir/src/s9xml.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/s9xml.cpp.o.requires

CMakeFiles/s9gear.dir/src/s9xml.cpp.o.provides: CMakeFiles/s9gear.dir/src/s9xml.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/s9xml.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/s9xml.cpp.o.provides

CMakeFiles/s9gear.dir/src/s9xml.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/s9xml.cpp.o

CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o: ../src/tinyxmlerror.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o -c /home/oni/Projects/S9Gear/src/tinyxmlerror.cpp

CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/tinyxmlerror.cpp > CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.i

CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/tinyxmlerror.cpp -o CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.s

CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o.requires

CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o.provides: CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o.provides

CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o

CMakeFiles/s9gear.dir/src/wingedge.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/wingedge.cpp.o: ../src/wingedge.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/wingedge.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/wingedge.cpp.o -c /home/oni/Projects/S9Gear/src/wingedge.cpp

CMakeFiles/s9gear.dir/src/wingedge.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/wingedge.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/wingedge.cpp > CMakeFiles/s9gear.dir/src/wingedge.cpp.i

CMakeFiles/s9gear.dir/src/wingedge.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/wingedge.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/wingedge.cpp -o CMakeFiles/s9gear.dir/src/wingedge.cpp.s

CMakeFiles/s9gear.dir/src/wingedge.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/wingedge.cpp.o.requires

CMakeFiles/s9gear.dir/src/wingedge.cpp.o.provides: CMakeFiles/s9gear.dir/src/wingedge.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/wingedge.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/wingedge.cpp.o.provides

CMakeFiles/s9gear.dir/src/wingedge.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/wingedge.cpp.o

CMakeFiles/s9gear.dir/src/primitive.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/primitive.cpp.o: ../src/primitive.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/primitive.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/primitive.cpp.o -c /home/oni/Projects/S9Gear/src/primitive.cpp

CMakeFiles/s9gear.dir/src/primitive.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/primitive.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/primitive.cpp > CMakeFiles/s9gear.dir/src/primitive.cpp.i

CMakeFiles/s9gear.dir/src/primitive.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/primitive.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/primitive.cpp -o CMakeFiles/s9gear.dir/src/primitive.cpp.s

CMakeFiles/s9gear.dir/src/primitive.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/primitive.cpp.o.requires

CMakeFiles/s9gear.dir/src/primitive.cpp.o.provides: CMakeFiles/s9gear.dir/src/primitive.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/primitive.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/primitive.cpp.o.provides

CMakeFiles/s9gear.dir/src/primitive.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/primitive.cpp.o

CMakeFiles/s9gear.dir/src/gl/video.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/gl/video.cpp.o: ../src/gl/video.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/gl/video.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/gl/video.cpp.o -c /home/oni/Projects/S9Gear/src/gl/video.cpp

CMakeFiles/s9gear.dir/src/gl/video.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/gl/video.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/gl/video.cpp > CMakeFiles/s9gear.dir/src/gl/video.cpp.i

CMakeFiles/s9gear.dir/src/gl/video.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/gl/video.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/gl/video.cpp -o CMakeFiles/s9gear.dir/src/gl/video.cpp.s

CMakeFiles/s9gear.dir/src/gl/video.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/gl/video.cpp.o.requires

CMakeFiles/s9gear.dir/src/gl/video.cpp.o.provides: CMakeFiles/s9gear.dir/src/gl/video.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/gl/video.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/gl/video.cpp.o.provides

CMakeFiles/s9gear.dir/src/gl/video.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/gl/video.cpp.o

CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o: ../src/gl/shapes.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_13)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o -c /home/oni/Projects/S9Gear/src/gl/shapes.cpp

CMakeFiles/s9gear.dir/src/gl/shapes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/gl/shapes.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/gl/shapes.cpp > CMakeFiles/s9gear.dir/src/gl/shapes.cpp.i

CMakeFiles/s9gear.dir/src/gl/shapes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/gl/shapes.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/gl/shapes.cpp -o CMakeFiles/s9gear.dir/src/gl/shapes.cpp.s

CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o.requires

CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o.provides: CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o.provides

CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o

CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o: ../src/gl/glfw_app.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_14)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o -c /home/oni/Projects/S9Gear/src/gl/glfw_app.cpp

CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/gl/glfw_app.cpp > CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.i

CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/gl/glfw_app.cpp -o CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.s

CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o.requires

CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o.provides: CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o.provides

CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o

CMakeFiles/s9gear.dir/src/gl/glew.c.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/gl/glew.c.o: ../src/gl/glew.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_15)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/s9gear.dir/src/gl/glew.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s9gear.dir/src/gl/glew.c.o   -c /home/oni/Projects/S9Gear/src/gl/glew.c

CMakeFiles/s9gear.dir/src/gl/glew.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s9gear.dir/src/gl/glew.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/oni/Projects/S9Gear/src/gl/glew.c > CMakeFiles/s9gear.dir/src/gl/glew.c.i

CMakeFiles/s9gear.dir/src/gl/glew.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s9gear.dir/src/gl/glew.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/oni/Projects/S9Gear/src/gl/glew.c -o CMakeFiles/s9gear.dir/src/gl/glew.c.s

CMakeFiles/s9gear.dir/src/gl/glew.c.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/gl/glew.c.o.requires

CMakeFiles/s9gear.dir/src/gl/glew.c.o.provides: CMakeFiles/s9gear.dir/src/gl/glew.c.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/gl/glew.c.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/gl/glew.c.o.provides

CMakeFiles/s9gear.dir/src/gl/glew.c.o.provides.build: CMakeFiles/s9gear.dir/src/gl/glew.c.o

CMakeFiles/s9gear.dir/src/gl/shader.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/gl/shader.cpp.o: ../src/gl/shader.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_16)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/gl/shader.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/gl/shader.cpp.o -c /home/oni/Projects/S9Gear/src/gl/shader.cpp

CMakeFiles/s9gear.dir/src/gl/shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/gl/shader.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/gl/shader.cpp > CMakeFiles/s9gear.dir/src/gl/shader.cpp.i

CMakeFiles/s9gear.dir/src/gl/shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/gl/shader.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/gl/shader.cpp -o CMakeFiles/s9gear.dir/src/gl/shader.cpp.s

CMakeFiles/s9gear.dir/src/gl/shader.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/gl/shader.cpp.o.requires

CMakeFiles/s9gear.dir/src/gl/shader.cpp.o.provides: CMakeFiles/s9gear.dir/src/gl/shader.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/gl/shader.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/gl/shader.cpp.o.provides

CMakeFiles/s9gear.dir/src/gl/shader.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/gl/shader.cpp.o

CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o: ../src/gl/fbo.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_17)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o -c /home/oni/Projects/S9Gear/src/gl/fbo.cpp

CMakeFiles/s9gear.dir/src/gl/fbo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/gl/fbo.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/gl/fbo.cpp > CMakeFiles/s9gear.dir/src/gl/fbo.cpp.i

CMakeFiles/s9gear.dir/src/gl/fbo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/gl/fbo.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/gl/fbo.cpp -o CMakeFiles/s9gear.dir/src/gl/fbo.cpp.s

CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o.requires

CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o.provides: CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o.provides

CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o

CMakeFiles/s9gear.dir/src/linux/jpeg.c.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/linux/jpeg.c.o: ../src/linux/jpeg.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_18)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/s9gear.dir/src/linux/jpeg.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s9gear.dir/src/linux/jpeg.c.o   -c /home/oni/Projects/S9Gear/src/linux/jpeg.c

CMakeFiles/s9gear.dir/src/linux/jpeg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s9gear.dir/src/linux/jpeg.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/oni/Projects/S9Gear/src/linux/jpeg.c > CMakeFiles/s9gear.dir/src/linux/jpeg.c.i

CMakeFiles/s9gear.dir/src/linux/jpeg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s9gear.dir/src/linux/jpeg.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/oni/Projects/S9Gear/src/linux/jpeg.c -o CMakeFiles/s9gear.dir/src/linux/jpeg.c.s

CMakeFiles/s9gear.dir/src/linux/jpeg.c.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/linux/jpeg.c.o.requires

CMakeFiles/s9gear.dir/src/linux/jpeg.c.o.provides: CMakeFiles/s9gear.dir/src/linux/jpeg.c.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/linux/jpeg.c.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/linux/jpeg.c.o.provides

CMakeFiles/s9gear.dir/src/linux/jpeg.c.o.provides.build: CMakeFiles/s9gear.dir/src/linux/jpeg.c.o

CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o: ../src/linux/colorspaces.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_19)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o   -c /home/oni/Projects/S9Gear/src/linux/colorspaces.c

CMakeFiles/s9gear.dir/src/linux/colorspaces.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/s9gear.dir/src/linux/colorspaces.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/oni/Projects/S9Gear/src/linux/colorspaces.c > CMakeFiles/s9gear.dir/src/linux/colorspaces.c.i

CMakeFiles/s9gear.dir/src/linux/colorspaces.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/s9gear.dir/src/linux/colorspaces.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/oni/Projects/S9Gear/src/linux/colorspaces.c -o CMakeFiles/s9gear.dir/src/linux/colorspaces.c.s

CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o.requires

CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o.provides: CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o.provides

CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o.provides.build: CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o

CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o: CMakeFiles/s9gear.dir/flags.make
CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o: ../src/linux/uvc_camera.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oni/Projects/S9Gear/build/CMakeFiles $(CMAKE_PROGRESS_20)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o -c /home/oni/Projects/S9Gear/src/linux/uvc_camera.cpp

CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oni/Projects/S9Gear/src/linux/uvc_camera.cpp > CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.i

CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oni/Projects/S9Gear/src/linux/uvc_camera.cpp -o CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.s

CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o.requires:
.PHONY : CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o.requires

CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o.provides: CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o.requires
	$(MAKE) -f CMakeFiles/s9gear.dir/build.make CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o.provides.build
.PHONY : CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o.provides

CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o.provides.build: CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o

# Object files for target s9gear
s9gear_OBJECTS = \
"CMakeFiles/s9gear.dir/src/shapes.cpp.o" \
"CMakeFiles/s9gear.dir/src/asset.cpp.o" \
"CMakeFiles/s9gear.dir/src/geometry.cpp.o" \
"CMakeFiles/s9gear.dir/src/tinyxml.cpp.o" \
"CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o" \
"CMakeFiles/s9gear.dir/src/camera.cpp.o" \
"CMakeFiles/s9gear.dir/src/tinystr.cpp.o" \
"CMakeFiles/s9gear.dir/src/s9xml.cpp.o" \
"CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o" \
"CMakeFiles/s9gear.dir/src/wingedge.cpp.o" \
"CMakeFiles/s9gear.dir/src/primitive.cpp.o" \
"CMakeFiles/s9gear.dir/src/gl/video.cpp.o" \
"CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o" \
"CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o" \
"CMakeFiles/s9gear.dir/src/gl/glew.c.o" \
"CMakeFiles/s9gear.dir/src/gl/shader.cpp.o" \
"CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o" \
"CMakeFiles/s9gear.dir/src/linux/jpeg.c.o" \
"CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o" \
"CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o"

# External object files for target s9gear
s9gear_EXTERNAL_OBJECTS =

libs9gear.so: CMakeFiles/s9gear.dir/src/shapes.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/asset.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/geometry.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/tinyxml.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/camera.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/tinystr.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/s9xml.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/wingedge.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/primitive.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/gl/video.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/gl/glew.c.o
libs9gear.so: CMakeFiles/s9gear.dir/src/gl/shader.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o
libs9gear.so: CMakeFiles/s9gear.dir/src/linux/jpeg.c.o
libs9gear.so: CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o
libs9gear.so: CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o
libs9gear.so: /usr/local/lib/libopencv_calib3d.so
libs9gear.so: /usr/local/lib/libopencv_contrib.so
libs9gear.so: /usr/local/lib/libopencv_core.so
libs9gear.so: /usr/local/lib/libopencv_features2d.so
libs9gear.so: /usr/local/lib/libopencv_gpu.so
libs9gear.so: /usr/local/lib/libopencv_highgui.so
libs9gear.so: /usr/local/lib/libopencv_imgproc.so
libs9gear.so: /usr/local/lib/libopencv_legacy.so
libs9gear.so: /usr/local/lib/libopencv_ml.so
libs9gear.so: /usr/local/lib/libopencv_objdetect.so
libs9gear.so: /usr/local/lib/libopencv_stitching.so
libs9gear.so: /usr/local/lib/libopencv_ts.so
libs9gear.so: /usr/local/lib/libopencv_video.so
libs9gear.so: /usr/lib/libboost_filesystem-mt.so
libs9gear.so: /usr/lib/libboost_system-mt.so
libs9gear.so: /usr/lib/libboost_thread-mt.so
libs9gear.so: /usr/lib/libboost_iostreams-mt.so
libs9gear.so: /usr/lib/libboost_signals-mt.so
libs9gear.so: /usr/lib/libboost_program_options-mt.so
libs9gear.so: CMakeFiles/s9gear.dir/build.make
libs9gear.so: CMakeFiles/s9gear.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library libs9gear.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/s9gear.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/s9gear.dir/build: libs9gear.so
.PHONY : CMakeFiles/s9gear.dir/build

CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/shapes.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/asset.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/geometry.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/tinyxml.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/tinyxmlparser.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/camera.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/tinystr.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/s9xml.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/tinyxmlerror.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/wingedge.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/primitive.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/gl/video.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/gl/shapes.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/gl/glfw_app.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/gl/glew.c.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/gl/shader.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/gl/fbo.cpp.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/linux/jpeg.c.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/linux/colorspaces.c.o.requires
CMakeFiles/s9gear.dir/requires: CMakeFiles/s9gear.dir/src/linux/uvc_camera.cpp.o.requires
.PHONY : CMakeFiles/s9gear.dir/requires

CMakeFiles/s9gear.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/s9gear.dir/cmake_clean.cmake
.PHONY : CMakeFiles/s9gear.dir/clean

CMakeFiles/s9gear.dir/depend:
	cd /home/oni/Projects/S9Gear/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oni/Projects/S9Gear /home/oni/Projects/S9Gear /home/oni/Projects/S9Gear/build /home/oni/Projects/S9Gear/build /home/oni/Projects/S9Gear/build/CMakeFiles/s9gear.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/s9gear.dir/depend

