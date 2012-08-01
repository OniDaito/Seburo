
cmake_minimum_required (VERSION 2.8) 
project (model) 

set(SOURCE_FILES 
  app.cpp
)

add_executable (model
  ${SOURCE_FILES} 
) 

include_directories(${GEAR_INCLUDES}
  ${INCLUDES_SEARCH_PATHS}
  ${INCLUDES}
)

target_link_libraries(model 
  s9gear
)

