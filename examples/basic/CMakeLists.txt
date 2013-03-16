
cmake_minimum_required (VERSION 2.8) 
project (basic) 

set(SOURCE_FILES 
	app.cpp
)

add_executable (basic
	${SOURCE_FILES} 
) 

include_directories(
  ${SEBURO_INCLUDES}
	${INCLUDES_SEARCH_PATHS}
	${INCLUDES}
)


target_link_libraries( basic
  Seburo 
)



