
cmake_minimum_required (VERSION 2.8) 
project (video) 

set(SOURCE_FILES 
  app.cpp
)

add_executable (video
  ${SOURCE_FILES} 
) 

include_directories( ${SEBURO_INCLUDES}
  ${INCLUDES_SEARCH_PATHS}
  ${INCLUDES}
)

target_link_libraries(video 
  Seburo
  ${SEBURO_LIBRARIES}
)

