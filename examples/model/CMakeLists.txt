
cmake_minimum_required (VERSION 2.8) 
project (model) 

set(SOURCE_FILES app.cpp)

if (APPLE)
  add_executable(model MACOSX_BUNDLE app.cpp)
  
  SET(MACOSX_BUNDLE_ICON_FILE s9.icns) 
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_SOURCE_DIR}/s9.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

  set_target_properties(model PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "model")

  INSTALL(CODE "
    include(BundleUtilities)
    fixup_bundle( ${CMAKE_CURRENT_BINARY_DIR}/model.app \"\"   \"${CMAKE_SOURCE_DIR}/lib/osx\")
    " COMPONENT Runtime)

else() 
  add_executable (model ${SOURCE_FILES} ) 
endif()

include_directories(${GEAR_INCLUDES}
  ${INCLUDES_SEARCH_PATHS}
  ${INCLUDES}
)

target_link_libraries(model 
  s9gear
)
