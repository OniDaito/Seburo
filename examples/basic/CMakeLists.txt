
cmake_minimum_required (VERSION 2.8) 
project (basic) 

set(SOURCE_FILES 
	app.cpp
)

if (WIN32)
SET(CMAKE_CXX_FLAGS "/MD /EHsc")
endif()

include_directories(
  ${SEBURO_INCLUDES}
	${INCLUDES_SEARCH_PATHS}
	${INCLUDES}
)

link_libraries( Seburo
${Boost_LIBRARIES} )


add_executable (basic
  ${SOURCE_FILES} 
)
