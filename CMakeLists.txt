#####################################################################
# Main CMake file for the Seburo Project

cmake_minimum_required (VERSION 2.8) 
project (Seburo) 

set(SEBURO_VERSION_MAJOR "0")
set(SEBURO_VERSION_MINOR "2")
set(SEBURO_VERSION_PATCH "0")
set(SEBURO_VERSION_EXTRA "")
set(SEBURO_VERSION "${SEBURO_VERSION_MAJOR}.${SEBURO_VERSION_MINOR}")
set(SEBURO_VERSION_FULL "${SEBURO_VERSION}.${SEBURO_VERSION_PATCH}${SEBURO_VERSION_EXTRA}")


#####################################################################
# Detect OS 

if (WIN32)
    set(_SEBURO_WIN32 1)
    message(STATUS "Building SEBURO for WGL on a Win32 system") 
    add_definitions(-D_SEBURO_WIN32)
elseif (UNIX AND APPLE)
    set(_SEBURO_OSX 1)
    add_definitions(-D_SEBURO_OSX)
    message(STATUS "Building SEBURO for Cocoa and NSOpenGL on Mac OS X")
    FILE(GLOB OSSources src/osx/*)
elseif (UNIX AND NOT APPLE)
    set(_SEBURO_LINUX 1)
    add_definitions(-D_SEBURO_LINUX)
    message(STATUS "Building SEBURO for X11 and GLX on a Unix-like system")
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

macro(SEBURO_find_library libname)
  string(TOLOWER ${libname} libnamelow)

  find_library(${libname}_LIB NAMES ${libname} PATHS ${LOCAL_LIBRARIES_SEARCH_PATHS} NO_DEFAULT_PATH)
  if (NOT ${libname}_LIB)
    find_library(${libname}_LIB NAMES ${libnamelow} PATHS ${LIBRARIES_SEARCH_PATHS})
  endif()
  
  if (NOT ${libname}_LIB)
    message(FATAL_ERROR "Cannot find '${libname}' library")
  else()
    message(STATUS "Found '${libname}' library in ${${libname}_LIB}")
    set(SEBURO_LIBRARIES ${SEBURO_LIBRARIES} ${${libname}_LIB})
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
  macro(SEBURO_find_pkgconfig_lib pkgname)
    include(FindPkgConfig)
    pkg_check_modules(${pkgname}_pkg REQUIRED ${pkgname})
    if ( DEFINED ${pkgname}_pkg_FOUND )
      set (SEBURO_LIBRARIES ${SEBURO_LIBRARIES} ${${pkgname}_pkg_LIBRARIES}
                                    ${${pkgname}_pkg_LDFLAGS} ${${pkgname}_pkg_LDFLAGS_OTHER})
      set (SEBURO_INCLUDES ${SEBURO_INCLUDES} ${${pkgname}_pkg_INCLUDE_DIRS})
      set (SEBURO_CXX_FLAGS ${SEBURO_CXX_FLAGS} ${${pkgname}_pkg_CFLAGS})
    else()
      message( FATAL_ERROR "Cannot find pkgconfig file for ${pkgconfig}. Check your PKG_CONFIG_PATH environment variable.")
    endif()
  endmacro(SEBURO_find_pkgconfig_lib)

  SEBURO_find_pkgconfig_lib(gtkmm-3.0)
  SEBURO_find_pkgconfig_lib(gl)
 
  SEBURO_find_library(glfw3)
  SEBURO_find_library(AntTweakBar)
  SEBURO_find_library(assimp)
  SEBURO_find_pkgconfig_lib(xxf86vm)

  include_directories(${PROJECT_SOURCE_DIR}/lib/linux
    ${SEBURO_INCLUDES}
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

  SEBURO_find_library(glfw3)
  SEBURO_find_library(AntTweakBar)
  SEBURO_find_library(AssImp)

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
  add_definitions(-D_SEBURO_OPENCV)
  SEBURO_find_library(opencv_calib3d)
  SEBURO_find_library(opencv_contrib)
  SEBURO_find_library(opencv_core)
  SEBURO_find_library(opencv_features2d)
  SEBURO_find_library(opencv_gpu)
#  SEBURO_find_library(opencv_highgui)
  SEBURO_find_library(opencv_imgproc)
  SEBURO_find_library(opencv_legacy)
  SEBURO_find_library(opencv_ml)
  SEBURO_find_library(opencv_objdetect)
#  SEBURO_find_library(opencv_stitching)
#  SEBURO_find_library(opencv_ts)
  SEBURO_find_library(opencv_video)
endif(USEOPENCV)

#####################################################################
# Glob Source files and create

FILE(GLOB CommonSources src/*)

add_library(Seburo SHARED
  ${CommonSources}
  ${GLSources}
  ${OSSources}
) 

# Now link the found libraries to the SEBURO library

target_link_libraries(Seburo 
  ${SEBURO_LIBRARIES}
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
  target_link_libraries(Seburo 
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
# GLFW Windowed mode (in case you want multiple windows in your GLFW3
# Application)

option(MULTIWINDOW "Multiple Window Support" OFF)

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
