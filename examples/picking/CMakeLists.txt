
cmake_minimum_required (VERSION 2.8) 
project (picking) 

set(SOURCE_FILES 
  app.cpp
)

add_executable (picking
  ${SOURCE_FILES} 
) 

include_directories(${GEAR_INCLUDES}
  ${INCLUDES_SEARCH_PATHS}
  ${INCLUDES}
)

target_link_libraries(picking 
  s9gear
)

