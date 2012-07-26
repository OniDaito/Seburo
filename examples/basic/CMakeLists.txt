
cmake_minimum_required (VERSION 2.8) 
project (basic) 

################################
# Set source files

set(SOURCE_FILES 
	app.cpp
)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall)
endif()


################################################################
# Detect Current OS

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
else()
    message(FATAL_ERROR "No supported platform was detected")
endif()


option(USEPCL "usepcl" OFF)

################################################################
# PCL Libs - included in the way mentioned on their FAQ
if (USEPCL)

	find_package(PCL 1.2 REQUIRED)
	include_directories(${PCL_INCLUDE_DIRS})
	link_directories(${PCL_LIBRARY_DIRS})
	add_definitions(${PCL_DEFINITIONS})

endif(USEPCL)

################################################################
# GLIB 

if (_GEAR_X11_GLX)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GLIB_PKG glib-2.0)

	if (GLIB_PKG_FOUND)
		message(Found glib-2.0)
		include_directories(${GLIB_PKG_INCLUDE_DIRS})
	endif(GLIB_PKG_FOUND)
endif(_GEAR_X11_GLX)

################################################################
# GLEW

pkg_check_modules(GLEW_PKG glew)

if (GLEW_PKG)
    message(Found glib-2.0)
	include_directories(${GLEW_PKG_INCLUDE_DIRS})
	
	  set (GLEW_LIBRARIES ${${pkgname}_pkg_LIBRARIES}
                                  ${${pkgname}_pkg_LDFLAGS} ${${pkgname}_pkg_LDFLAGS_OTHER})
	
endif(GLEW_PKG)


######################################
# Libs and includes held inside s9gear

include_directories(${PROJECT_ROOT}/lib)
include_directories(${PROJECT_ROOT}/src)

################################
# DEBUG mode

add_definitions(-DDEBUG)
SET(CMAKE_BUILD_TYPE debug)
SET(CMAKE_CXX_FLAGS_DEBUG "-g")

#SET(CMAKE_CXX_FLAGS_DEBUG "-std=c++0x")

################################
# Setup the exectutable

link_directories(${EXTRA_LIBS})
link_directories(${PROJECT_SOURCE_DIR}/lib)


################################
# OpenGL Packages


gear_find_pkgconfig_lib(gl)
gear_find_pkgconfig_lib(glew)

find_package(OpenGL)
find_package(GLUT)
if(NOT OPENGL_FOUND)
	message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)



################################
# GTK Linux Only - File Dialog stuff

if (_GEAR_X11_GLX)
	gear_find_pkgconfig_lib(gtkmm-3.0)
endif(_GEAR_X11_GLX)

################################################################
# Boost is required for the network, threading and parsing

find_package(Boost COMPONENTS system filesystem signals iostreams program_options thread REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})

################################################################
# Set the search paths for custom OpenCV and SDL headers


set(INCLUDES_SEARCH_PATHS
    /usr/local/include/opencv2
	/usr/local/include
	CACHE STRING "Custom Includes Search Paths")



# We generate a header in bin directory so we need to add that include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


################################################################
# Link Custom OpenCV Libraries

message(STATUS "Looking for libraries in : ${LIBRARIES_SEARCH_PATHS}")
message(STATUS "Looking for includes in  : ${INCLUDES_SEARCH_PATHS}")


option(USEOPENCV "useopencv" OFF)

if (USEOPENCV)
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

gear_find_library(glut)
gear_find_library(assimp)
gear_find_library(glfw)
gear_find_library(AntTweakBar)

if (_GEAR_X11_GLX)
	gear_find_library(Xrandr)
	gear_find_library(rt)
	gear_find_library(X11)
	gear_find_library(Xext)
	gear_find_library(Xxf86vm)

endif(_GEAR_X11_GLX)


if ( APPLE )
    set (CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
    set (CMAKE_FIND_LIBRARY_SUFFIXES ".dylib" ".a" ".lib", ".so")
    #set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m32")
    set (CMAKE_OSX_ARCHITECTURES "i386")
elseif( "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set (CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
    set (CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
endif ()

set (CMAKE_LIBRARY_PATH ${LIBRARIES_SEARCH_PATHS} /usr/lib /usr/local/lib )
set (CMAKE_INCLUDE_PATH ${INCLUDES_SEARCH_PATHS} /usr/include /usr/local/include )

add_executable (basic
	${SOURCE_FILES} 
) 

include_directories(${GEAR_INCLUDES}
	${INCLUDES_SEARCH_PATHS}
	${INCLUDES}
)

target_link_libraries(basic 
	${GEAR_LIBRARIES}
	${OPENGL_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_THREAD_LIBRARY}
	${Boost_IOSTREAMS_LIBRARY}
	${Boost_SIGNALS_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	s9gear
)

get_filename_component(PROJECT_ROOT ${CMAKE_CURRENT_LIST_FILE} PATH)                                                                                            
get_filename_component(PROJECT_NAME ${PROJECT_ROOT} NAME)

project(${PROJECT_NAME})
if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "i386")
endif(APPLE)


