# Try to find TreeTimer Library
# Once done this will define
# TREETIMER_FOUND - System has libtt.{a,so}
# TREETIMER_INCLUDE_DIRS - The treetimer include directories
# TREETIMER_LIBRARIES - The libraries needed to use treetimer
#
# Variables that can be used by this module.
# They must be defined before calling find_package.
# TREETIMER_ROOT - Root Directory

# TreeTimer Include Directories
if(NOT TREETIMER_INCLUDE_DIRS)
	find_path(TREETIMER_INCLUDE_DIRS tt_interface_c.h
		HINTS ${TREETIMER_ROOT} ENV TREETIMER_ROOT				# TREETIMER_ROOT Path Variable
		PATH_SUFFIXES include/timing_library/interface/
		DOC "Directory where the TreeTimer header files can be found"
	)
	mark_as_advanced(TREETIMER_INCLUDE_DIRS)
	message("Setting TREETIMER_INLUDE_DIRS to " ${TREETIMER_INCLUDE_DIRS})
endif()


# TreeTimer Library Directories
if(NOT TREETIMER_LIBRARIES)
	# Look for static library first?
	set(CMAKE_FIND_LIBRARY_SUFFIXES .a;.so;.dylib)

	find_library(TREETIMER_LIBRARIES
		NAMES tt
		HINTS ${TREETIMER_ROOT} ENV TREETIMER_ROOT				# TREETIMER_ROOT Path Variable
		PATH_SUFFIXES bla
		DOC "Directory where the TreeTimer libraries can be found"
	)

	# Restore library suffix order
	set(CMAKE_FIND_LIBRARY_SUFFIXES .so;.a)
	mark_as_advanced(TREETIMER_LIBRARIES)
endif()

# Handling of find_package arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TreeTimer REQUIRED_VARS TREETIMER_INCLUDE_DIRS TREETIMER_LIBRARIES)
