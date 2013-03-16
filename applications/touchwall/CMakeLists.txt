
#####################################################################
# Application using Seburo


cmake_minimum_required (VERSION 2.8) 
project (touchwall) 

#####################################################################
# Set source files

FILE(GLOB CommonSources src/*)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall)
endif()


#####################################################################
# Add Executable and sources

add_executable (touchwall
  ${CommonSources} 
) 

#####################################################################
# Copy the data directory

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)


#####################################################################
# Include Directories

include_directories(${PROJECT_SOURCE_DIR}/include
  ${SEBURO_INCLUDES}
  ${INCLUDES_SEARCH_PATHS}
  ${INCLUDES}
)

target_link_libraries(touchwall 
  Seburo
)
