
#####################################################################
# Leeds Application using S9Gear


cmake_minimum_required (VERSION 2.8) 
project (leeds) 

#####################################################################
# Set source files

FILE(GLOB CommonSources src/*)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall)
endif()

#####################################################################
# Add OpenCV to this project

set(INCLUDES_SEARCH_PATHS
    /usr/local/include/opencv2
    /usr/local/include
    CACHE STRING "Custom Includes Search Paths")

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

#####################################################################
# PCL Library


find_package(PCL 1.2 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


#####################################################################
# Add Executable and sources

add_executable (leeds
  ${CommonSources} 
) 


#####################################################################
# Copy the data directory

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

#####################################################################
# Include Directories

include_directories(${PROJECT_SOURCE_DIR}/include
  ${GEAR_INCLUDES}
  ${INCLUDES_SEARCH_PATHS}
  ${INCLUDES}
)

target_link_libraries(leeds 
  s9gear
  ${GEAR_LIBRARIES}
  ${PCL_LIBRARIES}
)
