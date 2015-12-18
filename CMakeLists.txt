#
# Copyright Regents of the University of Minnesota, 2014.  
# This software is released under the following license: http://opensource.org/licenses/GPL-2.0
# Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
#
# Code author(s):
# 		Dan Orban (dtorban)
#
 
cmake_minimum_required (VERSION 2.8.2) 
project (FiniteElement)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/../install" CACHE PATH "default install path" FORCE )
endif()

#This way will auto detect all source files in the directory,
#while this will work, the preferred technique is to list out
#sourcefiles individually.  This will allow cmake to automatically
#rerun if a source file is added, which it will not do if you don't
#list the explicit source files.
#aux_source_directory(${CMAKE_SOURCE_DIR} SOURCEFILES})
#This is how you specific source file individually
set (SOURCEFILES 
)
set (HEADERFILES 
)
source_group("Header Files" FILES ${HEADERFILES})

############################################################
#Start of library detection
#Any libraries outside of your source should have a corresponding
#find module in the $G/src/cmake/Modules directory and the 
#find_package method should be used here.
############################################################

find_package(CUDA)
if (CUDA_FOUND)
        message("CUDA found")
        set(USE_CUDA ON)
        add_definitions(-DUSE_CUDA)
endif()

############################################################
#End external library searching
############################################################

set (CMAKE_VERBOSE_MAKEFILE TRUE)

# Used for compiling in subdirectories of the project
include_directories (
  .
  ..
  ../../../eigen3
  /usr/include/eigen3
)

############################################################
#OS Specific flags should go here
############################################################

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # Linux specific code
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 

	set(Lib_Config "none")

	find_package(Threads)
	find_package(X11)
	set(LIBS_ALL ${LIBS_ALL} ${CMAKE_THREAD_LIBS_INIT} rt Xrandr Xxf86vm Xi m dl ${X11_LIBRARIES})
	set(All_LIB ${LIBS_ALL})
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  # Windows specific code

endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if (MSVC)
	# Windows specific code
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

endif (MSVC)


############################################################
# Add POSTFIX info
############################################################

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
set(CMAKE_MINSIZEREL_POSTFIX "s")

#set the build postfix extension according to the current configuration
if (CMAKE_BUILD_TYPE MATCHES "Release")
  set(CMAKE_BUILD_POSTFIX "${CMAKE_RELEASE_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
  set(CMAKE_BUILD_POSTFIX "${CMAKE_MINSIZEREL_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
  set(CMAKE_BUILD_POSTFIX "${CMAKE_RELWITHDEBINFO_POSTFIX}")
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CMAKE_BUILD_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
else()
  set(CMAKE_BUILD_POSTFIX "")
endif()

############################################################
#Finally add all the targets here
############################################################

#You can also add subprojects that are in subdirectories
#These subdirectories must contain their own CMakeLists.txt

add_subdirectory(deps)
add_subdirectory(integration)
add_subdirectory(fea)
add_subdirectory(gpu)
add_subdirectory(test)
add_subdirectory(apps)
#add_subdirectory(gpu)
#add_subdirectory(vis)
