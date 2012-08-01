
cmake_minimum_required (VERSION 2.8) 
project (fbo) 

set(SOURCE_FILES 
  app.cpp
)

add_executable (fbo
  ${SOURCE_FILES} 
) 

include_directories(${GEAR_INCLUDES}
  ${INCLUDES_SEARCH_PATHS}
  ${INCLUDES}
)

target_link_libraries(fbo 
  s9gear
)

