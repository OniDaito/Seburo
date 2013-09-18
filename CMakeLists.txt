#####################################################################
# Main CMake file for the Seburo Project

cmake_minimum_required (VERSION 2.8) 
project (Seburo) 

set(SEBURO_VERSION_MAJOR "0")
set(SEBURO_VERSION_MINOR "3")
set(SEBURO_VERSION_PATCH "0")
set(SEBURO_VERSION_EXTRA "")
set(SEBURO_VERSION "${SEBURO_VERSION_MAJOR}.${SEBURO_VERSION_MINOR}")
set(SEBURO_VERSION_FULL "${SEBURO_VERSION}.${SEBURO_VERSION_PATCH}${SEBURO_VERSION_EXTRA}")

#####################################################################
# Options

option(SEBURO_BUILD_EXAMPLES "Build the Seburo example programs" ON)

#####################################################################
# Detect OS and Basic Compiler settings
# Assume GCC under Linux, Clang under OSX and CL under Windows32
# No target for Windows 64 as yet. 
# Need to test for 64 bit Linux. OSX is 64 bit non universal by default

if (WIN32)
    set(_SEBURO_WIN32 1)
    message(STATUS "Building SEBURO Shared on a Win32 system") 
    add_definitions(-D_SEBURO_WIN32)
    add_definitions(-D_SEBURO_BUILD_DLL)
    SET(CMAKE_CXX_FLAGS "/EHsc")
    FILE(GLOB OSSources src/win32/*)
    #SET(CMAKE_MODULE_LINKER_FLAGS "/NODEFAULTLIB")

    #CMAKE_EXE_LINKER_FLAGS
    #CMAKE_SHARED_LINKER_FLAGS
    #CMAKE_MODULE_LINKER_FLAGS
 
elseif (UNIX AND APPLE)
    set(_SEBURO_OSX 1)
    add_definitions(-D_SEBURO_OSX)
    message(STATUS "Building SEBURO Shared on Mac OS X")
    FILE(GLOB OSSources src/osx/*)

elseif (UNIX AND NOT APPLE)
    set(_SEBURO_LINUX 1)
    add_definitions(-D_SEBURO_LINUX)
    message(STATUS "Building SEBURO Shared on a Unix-like system")
    FILE(GLOB OSSources src/linux/*)
else()
    message(FATAL_ERROR "No supported platform was detected")
endif()

################################
# DEBUG mode for Compilation
# TODO - Should affect linking also

option(DEBUG "Debug Mode" ON)
if(DEBUG)
  add_definitions(-DDEBUG)  
  set(_SEBURO_DEBUG 1)
  if (_SEBURO_OSX OR _SEBURO_LINUX)
    SET(CMAKE_CXX_FLAGS_DEBUG "-g")
  else()

  endif()

else()
  
endif()

###########

#####################################################################
# S9 Includes

include_directories(include)

#####################################################################
# OpenGL for now

FILE(GLOB GLSources src/gl/*)
FILE(GLOB GLUtils src/gl/utils/*)

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

if (_SEBURO_LINUX)

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
  SEBURO_find_pkgconfig_lib(xxf86vm)
 
  SEBURO_find_library(glfw)
  SEBURO_find_library(AntTweakBar)
  SEBURO_find_library(assimp)

  include_directories(${PROJECT_SOURCE_DIR}/lib/linux
    ${SEBURO_INCLUDES}
  )

elseif(_SEBURO_OSX)

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

  SEBURO_find_library(glfw)
  SEBURO_find_library(AntTweakBar)
  SEBURO_find_library(AssImp)


elseif(_SEBURO_WIN32)
  
  if (DEBUG)
    link_directories(${PROJECT_SOURCE_DIR}/lib/win32/Debug)
    set(LOCAL_LIBRARIES_SEARCH_PATHS
      ${CMAKE_SOURCE_DIR}/lib/win32/Debug
      CACHE STRING "Local Library search paths")
  elseif()
    link_directories(${PROJECT_SOURCE_DIR}/lib/win32/Release)
    set(LOCAL_LIBRARIES_SEARCH_PATHS
      ${CMAKE_SOURCE_DIR}/lib/win32/Release
      CACHE STRING "Local Library search paths")
  endif()

  find_package(OpenGL REQUIRED)

  SEBURO_find_library(glfw) # Built from the github version 3 build
  SEBURO_find_library(AntTweakBar)
  #SEBURO_find_library(AssImp)
  #SEBURO_find_library(glew32)

else()
  message(FATAL_ERROR "No supported platform was detected")
endif()

#####################################################################
# Boost - Regular and installed

# This assumes boost is built for static inclusion as libs for
# MSVC 2010 in release mode
# http://cpprocks.com/using-cmake-to-build-a-cross-platform-project-with-a-boost-dependency/

if(WIN32)
  SET(Boost_USE_STATIC_LIBS   ON)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME ON)
  SET(BOOST_ALL_DYN_LINK  OFF)
  #add_definitions(-DBOOST_ALL_NO_LIB)
  #SET(CMAKE_SHARED_LINKER_FLAGS /NODEFAULTLIB:LIBC;LIBCMT;MSVCRT)
endif()

message("Looking for Installed BOOST Libraries")
find_package(Boost COMPONENTS system filesystem signals iostreams program_options thread REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")


######################################################################
# Glob Source files

FILE(GLOB CommonSources src/*)

# Not sure why I have these?
list(REMOVE_ITEM GLUtils ${PROJECT_SOURCE_DIR}/src/gl/utils/glewinfo.c)
list(REMOVE_ITEM GLUtils ${PROJECT_SOURCE_DIR}/src/gl/utils/visualinfo.c)

#####################################################################
# OpenCV.

option(USE_OPENCV "use opencv" OFF)

if (USE_OPENCV)
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

else()
  list(REMOVE_ITEM CommonSources src/cvprocess.cpp)
endif(USE_OPENCV)

#####################################################################
# OpenNI
# \TODO we are working with OpenNI2 so does this need to change?

option(USE_OPENNI "use openni" ON)
if (USE_OPENNI)
  
  find_package(OpenNI)
  if (OPENNI_FOUND)
    set (HAVE_OPENNI ON)
    add_definitions(-D_SEBURO_OPENNI)
    include_directories(${OPENNI_INCLUDE_DIRS})
  endif(OPENNI_FOUND)

endif(USE_OPENNI)

#####################################################################
# Create Target

# Final removals for Win32 until we add fixes
if (_SEBURO_WIN32)
  #list(REMOVE_ITEM GLSources ${PROJECT_SOURCE_DIR}/src/gl/glew.c)
 

  # Make sure we are using /MT this time around. We may change to /MD
  # but we MUST be consistent

  set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
  foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach()

endif()


add_library(Seburo SHARED
  ${CommonSources}
  ${GLSources}
  ${GLUtils}
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

##########################################################
# Define a bundling Macro for OSX only

if (_SEBURO_OSX)

  macro(SEBURO_bundler)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE app.cpp)
    
    SET(MACOSX_BUNDLE_ICON_FILE ${CMAKE_SOURCE_DIR}/s9.icns) 
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_SOURCE_DIR}/s9.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}")

    set(DIRS ${CMAKE_SOURCE_DIR}/lib/osx
      ${CMAKE_SOURCE_DIR}/data
      ${CMAKE_SOURCE_DIR}/shaders/1.5
    ) 

    set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "uk.co.section9.seburo.${PROJECT_NAME}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION})
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2013. All Rights Reserved.")

    # Setup the Bundle Files

    #add_custom_command (TARGET ${PROJECT_NAME} PRE_BUILD
    #                    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources)

  
    #add_custom_command (TARGET ${PROJECT_NAME} PRE_BUILD
    #                    COMMAND cp -r ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/.)

    add_custom_command (TARGET ${PROJECT_NAME} PRE_BUILD
                        COMMAND cp ${CMAKE_SOURCE_DIR}/lib/osx/* ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/MacOS/.)

    INSTALL(CODE "
      include(BundleUtilities)
      fixup_bundle( ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app \"\"   \"${DIRS}\")
      " COMPONENT Runtime)
  endmacro(SEBURO_bundler)

endif()

#####################################################################
# Post Build

if (_SEBURO_OSX)
  add_custom_command (TARGET Seburo PRE_BUILD
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/../scripts/osx_paths.sh ${CMAKE_CURRENT_BINARY_DIR}/. )

  add_custom_command (TARGET Seburo POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/osx_paths.sh)

endif()


#####################################################################
# Go into examples directory and build these

if (SEBURO_BUILD_EXAMPLES) 
  if (NOT TARGET examples) 
    FILE(GLOB examples examples/*)
    foreach(example ${examples})
      if(IS_DIRECTORY ${example})
        add_subdirectory(${example})
      endif()
    endforeach()
  endif()
endif()
