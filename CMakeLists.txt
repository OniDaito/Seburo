#####################################################################
# Main CMake file for the S9Gear Project

cmake_minimum_required (VERSION 2.8) 
project (S9Gear) 

set(GEAR_VERSION_MAJOR "0")
set(GEAR_VERSION_MINOR "2")
set(GEAR_VERSION_PATCH "0")
set(GEAR_VERSION_EXTRA "")
set(GEAR_VERSION "${GEAR_VERSION_MAJOR}.${GEAR_VERSION_MINOR}")
set(GEAR_VERSION_FULL "${GEAR_VERSION}.${GEAR_VERSION_PATCH}${GEAR_VERSION_EXTRA}")


#####################################################################
# Detect OS 

if (WIN32)
    set(_GEAR_WIN32 1)
    message(STATUS "Building Gear for WGL on a Win32 system") 
    add_definitions(-D_GEAR_WIN32)
elseif (UNIX AND APPLE)
    set(_GEAR_OSX 1)
    add_definitions(-D_GEAR_OSX)
    message(STATUS "Building Gear for Cocoa and NSOpenGL on Mac OS X")
    FILE(GLOB OSSources src/osx/*)
elseif (UNIX AND NOT APPLE)
    set(_GEAR_LINUX 1)
    add_definitions(-D_GEAR_LINUX)
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
# Macro to find libraries in the local lib directory then system wide

macro(gear_find_library libname)
  string(TOLOWER ${libname} libnamelow)

  find_library(${libname}_LIB NAMES ${libname} PATHS ${LOCAL_LIBRARIES_SEARCH_PATHS} NO_DEFAULT_PATH)
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
    CACHE STRING "Library search paths")
 
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(LOCAL_LIBRARIES_SEARCH_PATHS
      ${CMAKE_SOURCE_DIR}/lib/linux64
      CACHE STRING "Local Library search paths")
    link_directories(${PROJECT_SOURCE_DIR}/lib/linux64)
  else()
    set(LOCAL_LIBRARIES_SEARCH_PATHS
      ${CMAKE_SOURCE_DIR}/lib/linux32
      CACHE STRING "Local Library search paths")
    link_directories(${PROJECT_SOURCE_DIR}/lib/linux32)
  endif()


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
 
  gear_find_library(glfw3)
  gear_find_library(AntTweakBar)
  gear_find_library(assimp)
  gear_find_pkgconfig_lib(xxf86vm)

  include_directories(${PROJECT_SOURCE_DIR}/lib/linux
    ${GEAR_INCLUDES}
  )

elseif(UNIX AND APPLE)

  link_directories(${PROJECT_SOURCE_DIR}/lib/osx)

  set (CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
  set (CMAKE_FIND_LIBRARY_SUFFIXES ".dylib" ".a" ".lib", ".so")
  #set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m32")
  #set (CMAKE_OSX_ARCHITECTURES "i386")

  find_library(IOKIT_FRAMEWORK IOKit)
  find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
  FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa )
  MARK_AS_ADVANCED (COCOA_LIBRARY
                     OPENGL_LIBRARY)

  SET(OSX_FRAMEWORKS ${COCOA_LIBRARY} 
    ${OPENGL_LIBRARY} 
    ${IOKIT_FRAMEWORK}
    ${CORE_FOUNDATION_FRAMEWORK}
  )
   
  set(LIBRARIES_SEARCH_PATHS
    /usr/local/lib/
    /usr/local/lib64/
    CACHE STRING "Library search paths")

  set(LOCAL_LIBRARIES_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/lib/osx
    CACHE STRING "Local Library search paths")

  gear_find_library(glfw3)
  gear_find_library(AntTweakBar)
  gear_find_library(AssImp)

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
#  gear_find_library(opencv_highgui)
  gear_find_library(opencv_imgproc)
  gear_find_library(opencv_legacy)
  gear_find_library(opencv_ml)
  gear_find_library(opencv_objdetect)
#  gear_find_library(opencv_stitching)
#  gear_find_library(opencv_ts)
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
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_IOSTREAMS_LIBRARY}
  ${Boost_SIGNALS_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

# seperate libs depending on OS

if (UNIX AND APPLE)
  target_link_libraries(s9gear 
    ${OSX_FRAMEWORKS}
  )
endif()

project(${PROJECT_NAME})

################################
# DEBUG mode

option(DEBUG "Debug Mode" ON)
if(DEBUG)
  add_definitions(-DDEBUG)  
  SET(CMAKE_BUILD_TYPE debug)
  SET(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

#####################################################################
# Go into examples directory and build these

if (NOT TARGET examples)
  FILE(GLOB examples examples/*)
  foreach(example ${examples})
    add_subdirectory(${example})
  endforeach()
endif()


#####################################################################
# Build Applications

if (NOT TARGET applications)
  FILE(GLOB applications applications/*)
  foreach(application ${applications})
    add_subdirectory(${application})
  endforeach()
endif()
