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
CMAKE_SOURCE_DIR = /home/nicolasb/Desktop/EmbPT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nicolasb/Desktop/EmbPT/build

# Include any dependencies generated for this target.
include rtcore/CMakeFiles/rtcore.dir/depend.make

# Include the progress variables for this target.
include rtcore/CMakeFiles/rtcore.dir/progress.make

# Include the compile flags for this target's objects.
include rtcore/CMakeFiles/rtcore.dir/flags.make

rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o: ../rtcore/common/compute_bounds.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/common/compute_bounds.o -c /home/nicolasb/Desktop/EmbPT/rtcore/common/compute_bounds.cpp

rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/common/compute_bounds.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/common/compute_bounds.cpp > CMakeFiles/rtcore.dir/common/compute_bounds.i

rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/common/compute_bounds.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/common/compute_bounds.cpp -o CMakeFiles/rtcore.dir/common/compute_bounds.s

rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o.requires

rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o.provides: rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o.provides

rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o.provides.build: rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o.provides.build

rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o: ../rtcore/common/spatial_binning.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/common/spatial_binning.o -c /home/nicolasb/Desktop/EmbPT/rtcore/common/spatial_binning.cpp

rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/common/spatial_binning.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/common/spatial_binning.cpp > CMakeFiles/rtcore.dir/common/spatial_binning.i

rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/common/spatial_binning.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/common/spatial_binning.cpp -o CMakeFiles/rtcore.dir/common/spatial_binning.s

rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o.requires

rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o.provides: rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o.provides

rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o.provides.build: rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o.provides.build

rtcore/CMakeFiles/rtcore.dir/common/object_binning.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/common/object_binning.o: ../rtcore/common/object_binning.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/common/object_binning.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/common/object_binning.o -c /home/nicolasb/Desktop/EmbPT/rtcore/common/object_binning.cpp

rtcore/CMakeFiles/rtcore.dir/common/object_binning.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/common/object_binning.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/common/object_binning.cpp > CMakeFiles/rtcore.dir/common/object_binning.i

rtcore/CMakeFiles/rtcore.dir/common/object_binning.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/common/object_binning.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/common/object_binning.cpp -o CMakeFiles/rtcore.dir/common/object_binning.s

rtcore/CMakeFiles/rtcore.dir/common/object_binning.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/object_binning.o.requires

rtcore/CMakeFiles/rtcore.dir/common/object_binning.o.provides: rtcore/CMakeFiles/rtcore.dir/common/object_binning.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/common/object_binning.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/object_binning.o.provides

rtcore/CMakeFiles/rtcore.dir/common/object_binning.o.provides.build: rtcore/CMakeFiles/rtcore.dir/common/object_binning.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/object_binning.o.provides.build

rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o: ../rtcore/common/object_binning_parallel.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/common/object_binning_parallel.o -c /home/nicolasb/Desktop/EmbPT/rtcore/common/object_binning_parallel.cpp

rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/common/object_binning_parallel.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/common/object_binning_parallel.cpp > CMakeFiles/rtcore.dir/common/object_binning_parallel.i

rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/common/object_binning_parallel.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/common/object_binning_parallel.cpp -o CMakeFiles/rtcore.dir/common/object_binning_parallel.s

rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o.requires

rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o.provides: rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o.provides

rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o.provides.build: rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o.provides.build

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o: ../rtcore/bvh2/bvh2.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/bvh2/bvh2.o -c /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2.cpp

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/bvh2/bvh2.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2.cpp > CMakeFiles/rtcore.dir/bvh2/bvh2.i

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/bvh2/bvh2.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2.cpp -o CMakeFiles/rtcore.dir/bvh2/bvh2.s

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o.requires

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o.provides: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o.provides

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o.provides.build: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o.provides.build

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o: ../rtcore/bvh2/bvh2_traverser.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o -c /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_traverser.cpp

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_traverser.cpp > CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.i

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_traverser.cpp -o CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.s

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o.requires

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o.provides: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o.provides

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o.provides.build: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o.provides.build

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o: ../rtcore/bvh2/bvh2_builder.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o -c /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_builder.cpp

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/bvh2/bvh2_builder.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_builder.cpp > CMakeFiles/rtcore.dir/bvh2/bvh2_builder.i

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/bvh2/bvh2_builder.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_builder.cpp -o CMakeFiles/rtcore.dir/bvh2/bvh2_builder.s

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o.requires

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o.provides: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o.provides

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o.provides.build: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o.provides.build

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o: ../rtcore/bvh2/bvh2_builder_spatial.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o -c /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_builder_spatial.cpp

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_builder_spatial.cpp > CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.i

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_builder_spatial.cpp -o CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.s

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o.requires

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o.provides: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o.provides

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o.provides.build: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o.provides.build

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o: ../rtcore/bvh2/bvh2_to_bvh4.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o -c /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_to_bvh4.cpp

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_to_bvh4.cpp > CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.i

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/bvh2/bvh2_to_bvh4.cpp -o CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.s

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o.requires

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o.provides: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o.provides

rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o.provides.build: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o.provides.build

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o: ../rtcore/bvh4/bvh4.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/bvh4/bvh4.o -c /home/nicolasb/Desktop/EmbPT/rtcore/bvh4/bvh4.cpp

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/bvh4/bvh4.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/bvh4/bvh4.cpp > CMakeFiles/rtcore.dir/bvh4/bvh4.i

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/bvh4/bvh4.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/bvh4/bvh4.cpp -o CMakeFiles/rtcore.dir/bvh4/bvh4.s

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o.requires

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o.provides: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o.provides

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o.provides.build: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o.provides.build

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o: ../rtcore/bvh4/bvh4_traverser.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o -c /home/nicolasb/Desktop/EmbPT/rtcore/bvh4/bvh4_traverser.cpp

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/bvh4/bvh4_traverser.cpp > CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.i

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/bvh4/bvh4_traverser.cpp -o CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.s

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o.requires

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o.provides: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o.provides

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o.provides.build: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o.provides.build

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o: ../rtcore/bvh4/bvh4_builder.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o -c /home/nicolasb/Desktop/EmbPT/rtcore/bvh4/bvh4_builder.cpp

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/bvh4/bvh4_builder.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/bvh4/bvh4_builder.cpp > CMakeFiles/rtcore.dir/bvh4/bvh4_builder.i

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/bvh4/bvh4_builder.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/bvh4/bvh4_builder.cpp -o CMakeFiles/rtcore.dir/bvh4/bvh4_builder.s

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o.requires

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o.provides: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o.provides

rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o.provides.build: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o.provides.build

rtcore/CMakeFiles/rtcore.dir/rtcore.o: rtcore/CMakeFiles/rtcore.dir/flags.make
rtcore/CMakeFiles/rtcore.dir/rtcore.o: ../rtcore/rtcore.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/nicolasb/Desktop/EmbPT/build/CMakeFiles $(CMAKE_PROGRESS_13)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object rtcore/CMakeFiles/rtcore.dir/rtcore.o"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rtcore.dir/rtcore.o -c /home/nicolasb/Desktop/EmbPT/rtcore/rtcore.cpp

rtcore/CMakeFiles/rtcore.dir/rtcore.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtcore.dir/rtcore.i"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/nicolasb/Desktop/EmbPT/rtcore/rtcore.cpp > CMakeFiles/rtcore.dir/rtcore.i

rtcore/CMakeFiles/rtcore.dir/rtcore.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtcore.dir/rtcore.s"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/nicolasb/Desktop/EmbPT/rtcore/rtcore.cpp -o CMakeFiles/rtcore.dir/rtcore.s

rtcore/CMakeFiles/rtcore.dir/rtcore.o.requires:
.PHONY : rtcore/CMakeFiles/rtcore.dir/rtcore.o.requires

rtcore/CMakeFiles/rtcore.dir/rtcore.o.provides: rtcore/CMakeFiles/rtcore.dir/rtcore.o.requires
	$(MAKE) -f rtcore/CMakeFiles/rtcore.dir/build.make rtcore/CMakeFiles/rtcore.dir/rtcore.o.provides.build
.PHONY : rtcore/CMakeFiles/rtcore.dir/rtcore.o.provides

rtcore/CMakeFiles/rtcore.dir/rtcore.o.provides.build: rtcore/CMakeFiles/rtcore.dir/rtcore.o
.PHONY : rtcore/CMakeFiles/rtcore.dir/rtcore.o.provides.build

# Object files for target rtcore
rtcore_OBJECTS = \
"CMakeFiles/rtcore.dir/common/compute_bounds.o" \
"CMakeFiles/rtcore.dir/common/spatial_binning.o" \
"CMakeFiles/rtcore.dir/common/object_binning.o" \
"CMakeFiles/rtcore.dir/common/object_binning_parallel.o" \
"CMakeFiles/rtcore.dir/bvh2/bvh2.o" \
"CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o" \
"CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o" \
"CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o" \
"CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o" \
"CMakeFiles/rtcore.dir/bvh4/bvh4.o" \
"CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o" \
"CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o" \
"CMakeFiles/rtcore.dir/rtcore.o"

# External object files for target rtcore
rtcore_EXTERNAL_OBJECTS =

librtcore.a: rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/common/object_binning.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/rtcore.o
librtcore.a: rtcore/CMakeFiles/rtcore.dir/build.make
librtcore.a: rtcore/CMakeFiles/rtcore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../librtcore.a"
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && $(CMAKE_COMMAND) -P CMakeFiles/rtcore.dir/cmake_clean_target.cmake
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rtcore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
rtcore/CMakeFiles/rtcore.dir/build: librtcore.a
.PHONY : rtcore/CMakeFiles/rtcore.dir/build

rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/common/compute_bounds.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/common/spatial_binning.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/common/object_binning.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/common/object_binning_parallel.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_traverser.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_builder_spatial.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/bvh2/bvh2_to_bvh4.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_traverser.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/bvh4/bvh4_builder.o.requires
rtcore/CMakeFiles/rtcore.dir/requires: rtcore/CMakeFiles/rtcore.dir/rtcore.o.requires
.PHONY : rtcore/CMakeFiles/rtcore.dir/requires

rtcore/CMakeFiles/rtcore.dir/clean:
	cd /home/nicolasb/Desktop/EmbPT/build/rtcore && $(CMAKE_COMMAND) -P CMakeFiles/rtcore.dir/cmake_clean.cmake
.PHONY : rtcore/CMakeFiles/rtcore.dir/clean

rtcore/CMakeFiles/rtcore.dir/depend:
	cd /home/nicolasb/Desktop/EmbPT/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nicolasb/Desktop/EmbPT /home/nicolasb/Desktop/EmbPT/rtcore /home/nicolasb/Desktop/EmbPT/build /home/nicolasb/Desktop/EmbPT/build/rtcore /home/nicolasb/Desktop/EmbPT/build/rtcore/CMakeFiles/rtcore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : rtcore/CMakeFiles/rtcore.dir/depend
