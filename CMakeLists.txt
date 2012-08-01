#####################################################################
# Main CMake file for the S9Gear Project

cmake_minimum_required (VERSION 2.8) 
project (S9Gear) 

#####################################################################
# Detect OS 

if (WIN32)
    set(_GEAR_WIN32_WGL 1)
    message(STATUS "Building Gear for WGL on a Win32 system") 
    add_definitions(-D_GEAR_WIN32_WGL)
elseif (UNIX AND APPLE)
    set(_GEAR_COCOA_NSGL 1)
    add_definitions(-D_GEAR_COCOA_NSGL)
    message(STATUS "Building Gear for Cocoa and NSOpenGL on Mac OS X")
elseif (UNIX AND NOT APPLE)
    set(_GEAR_X11_GLX 1)
    add_definitions(-D_GEAR_X11_GLX)
    message(STATUS "Building Gear for X11 and GLX on a Unix-like system")
  FILE(GLOB OSSources src/linux/*)
else()
    message(FATAL_ERROR "No supported platform was detected")
endif()

#####################################################################
# S9 Includes

include_directories(include)

#####################################################################
# Macro to find libraries

macro(gear_find_library libname)
  string(TOLOWER ${libname} libnamelow)

  find_library(${libname}_LIB NAMES ${libname} PATHS ${LIBRARIES_SEARCH_PATHS})
  if (NOT ${libname}_LIB)
    find_library(${libname}_LIB NAMES ${libnamelow} PATHS ${LIBRARIES_SEARCH_PATHS})
  endif()
  
  if (NOT ${libname}_LIB)
    message(FATAL_ERROR "Cannot find '${libname}' library")
  else()
    message(STATUS "Found '${libname}' library in ${${libname}_LIB}")
    set(GEAR_LIBRARIES ${GEAR_LIBRARIES} ${${libname}_LIB})
  endif()
endmacro()

#####################################################################
# Extra Libraries and their paths

if (UNIX AND NOT APPLE)
  set (CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
  set (CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")

  set(LIBRARIES_SEARCH_PATHS 
    /usr/local/lib/
    /usr/local/lib64/
    ${CMAKE_SOURCE_DIR}/lib/linux
    CACHE STRING "Library search paths")

  link_directories(${PROJECT_SOURCE_DIR}/lib/linux)

elseif(UNIX AND APPLE)
  set (CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
  set (CMAKE_FIND_LIBRARY_SUFFIXES ".dylib" ".a" ".lib", ".so")
  #set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m32")
  #set (CMAKE_OSX_ARCHITECTURES "i386")

  set(LIBRARIES_SEARCH_PATHS 
    /usr/local/lib/
    /usr/local/lib64/
    ${CMAKE_SOURCE_DIR}/lib/osx
    CACHE STRING "Library search paths")

  link_directories(${PROJECT_SOURCE_DIR}/lib/osx)

  gear_find_library(assimp)


else()
  message(FATAL_ERROR "No supported platform was detected")
endif()

#####################################################################
# Boost - Regular and installed

find_package(Boost COMPONENTS system filesystem signals iostreams program_options thread REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})

#####################################################################
# Glob Source files and create

FILE(GLOB CommonSources src/*)

add_library(s9gear SHARED
  ${CommonSources}
  ${GLSources}
  ${OSSources}
) 

# Now link the found libraries to the gear library

target_link_libraries(s9gear 
  ${GEAR_LIBRARIES}
  ${OPENGL_LIBRARY}
)

project(${PROJECT_NAME})

#####################################################################
# Go into examples directory and build these

if (NOT TARGET examples)
  add_subdirectory("${CMAKE_SOURCE_DIR}/examples/basic")
  add_subdirectory("${CMAKE_SOURCE_DIR}/examples/model")
  add_subdirectory("${CMAKE_SOURCE_DIR}/examples/fbo")
  add_subdirectory("${CMAKE_SOURCE_DIR}/examples/video")
endif() 