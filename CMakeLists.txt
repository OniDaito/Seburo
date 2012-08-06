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
# OpenGL for now

FILE(GLOB GLSources src/gl/*)

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

  link_directories(${PROJECT_SOURCE_DIR}/lib/linux)

  set (CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
  set (CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")

  set(LIBRARIES_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/lib/linux
    /usr/local/lib/
    /usr/local/lib64/
    CACHE STRING "Library search paths")
 
  set(LOCAL_LIBRARIES_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/lib/linux
    CACHE STRING "Local Library search paths")


  # Setup pkgconfig macro for including extra things
  macro(gear_find_pkgconfig_lib pkgname)
    include(FindPkgConfig)
    pkg_check_modules(${pkgname}_pkg REQUIRED ${pkgname})
    if ( DEFINED ${pkgname}_pkg_FOUND )
      set (GEAR_LIBRARIES ${GEAR_LIBRARIES} ${${pkgname}_pkg_LIBRARIES}
                                    ${${pkgname}_pkg_LDFLAGS} ${${pkgname}_pkg_LDFLAGS_OTHER})
      set (GEAR_INCLUDES ${GEAR_INCLUDES} ${${pkgname}_pkg_INCLUDE_DIRS})
      set (GEAR_CXX_FLAGS ${GEAR_CXX_FLAGS} ${${pkgname}_pkg_CFLAGS})
    else()
      message( FATAL_ERROR "Cannot find pkgconfig file for ${pkgconfig}. Check your PKG_CONFIG_PATH environment variable.")
    endif()
  endmacro(gear_find_pkgconfig_lib)

  gear_find_pkgconfig_lib(gtkmm-3.0)
  gear_find_pkgconfig_lib(gl)

  include_directories(${PROJECT_SOURCE_DIR}/lib/linux
    ${GEAR_INCLUDES}
  )

elseif(UNIX AND APPLE)

  link_directories(${PROJECT_SOURCE_DIR}/lib/osx)

  set (CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
  set (CMAKE_FIND_LIBRARY_SUFFIXES ".dylib" ".a" ".lib", ".so")
  #set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m32")
  #set (CMAKE_OSX_ARCHITECTURES "i386")

  FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
  FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime )
  FIND_LIBRARY(COCOA_LIBRARY Cocoa )
  MARK_AS_ADVANCED (COCOA_LIBRARY
                     QUICKTIME_LIBRARY
                     OPENGL_LIBRARY)
  SET(OSX_FRAMEWORKS ${COCOA_LIBRARY} ${QUICKTIME_LIBRARY} ${OPENGL_LIBRARY})

  set(LIBRARIES_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/lib/osx
    /usr/local/lib/
    /usr/local/lib64/
    CACHE STRING "Library search paths")

  set(LOCAL_LIBRARIES_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/lib/osx
    CACHE STRING "Local Library search paths")

else()
  message(FATAL_ERROR "No supported platform was detected")
endif()

#####################################################################
# Boost - Regular and installed

message("Looking for Installed BOOST Libraries")
find_package(Boost COMPONENTS system filesystem signals iostreams program_options thread REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})


#####################################################################
# OpenCV. Turned on by default as its very useful - Version 2+

option(USEOPENCV "useopencv" ON)

if (USEOPENCV)
  add_definitions(-D_GEAR_OPENCV)
  gear_find_library(opencv_calib3d)
  gear_find_library(opencv_contrib)
  gear_find_library(opencv_core)
  gear_find_library(opencv_features2d)
  gear_find_library(opencv_gpu)
  gear_find_library(opencv_highgui)
  gear_find_library(opencv_imgproc)
  gear_find_library(opencv_legacy)
  gear_find_library(opencv_ml)
  gear_find_library(opencv_objdetect)
  gear_find_library(opencv_stitching)
  gear_find_library(opencv_ts)
  gear_find_library(opencv_video)
endif(USEOPENCV)

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
  ${OSX_FRAMEWORKS}
  assimp
  glfw
  AntTweakBar
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_IOSTREAMS_LIBRARY}
  ${Boost_SIGNALS_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

project(${PROJECT_NAME})

################################
# DEBUG mode

#add_definitions(-DDEBUG)
#SET(CMAKE_BUILD_TYPE debug)
#SET(CMAKE_CXX_FLAGS_DEBUG "-g")


#####################################################################
# Go into examples directory and build these

if (NOT TARGET examples)
  add_subdirectory("${CMAKE_SOURCE_DIR}/examples/basic")
  add_subdirectory("${CMAKE_SOURCE_DIR}/examples/model")
  add_subdirectory("${CMAKE_SOURCE_DIR}/examples/fbo")
  add_subdirectory("${CMAKE_SOURCE_DIR}/examples/video")
  add_subdirectory("${CMAKE_SOURCE_DIR}/examples/picking")
endif() 

#####################################################################
# Build Leeds Application

if (NOT TARGET applications AND _GEAR_X11_GLX)
  add_subdirectory("${CMAKE_SOURCE_DIR}/applications/leeds")
endif() 
