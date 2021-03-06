
cmake_minimum_required (VERSION 2.8) 
project (texture) 

set(SOURCE_FILES app.cpp)

if (_SEBURO_OSX)
  add_executable(texture MACOSX_BUNDLE app.cpp)
  
  SET(MACOSX_BUNDLE_ICON_FILE ${CMAKE_SOURCE_DIR}/s9.icns) 
  SET_SOURCE_FILES_PROPERTIES(${CMAKE_SOURCE_DIR}/s9.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

  set_target_properties(texture PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "texture")

  set(DIRS ${CMAKE_SOURCE_DIR}/lib/osx
    ${CMAKE_SOURCE_DIR}/data
    ${CMAKE_SOURCE_DIR}/shaders
  ) 

  set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "uk.co.section9.seburo.texture")
  set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME}")
  set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION})
  set(MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION})
  set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2013. All Rights Reserved.")

  # Setup the Bundle Files

  add_custom_command (TARGET texture PRE_BUILD
                      COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/texture.app/Contents/Resources)

  add_custom_command (TARGET texture PRE_BUILD
                      COMMAND cp -r ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/texture.app/Contents/Resources/.)

  add_custom_command (TARGET texture PRE_BUILD
                      COMMAND cp -r ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/texture.app/Contents/Resources/.)

  add_custom_command (TARGET texture PRE_BUILD
                      COMMAND cp ${CMAKE_BINARY_DIR}/../lib/osx/* ${CMAKE_CURRENT_BINARY_DIR}/texture.app/Contents/Resources/.)

  add_custom_command (TARGET texture PRE_BUILD
                      COMMAND cp ${CMAKE_BINARY_DIR}/libSeburo.dylib ${CMAKE_CURRENT_BINARY_DIR}/texture.app/Contents/Resources/.)

  INSTALL(CODE "
    include(BundleUtilities)
    fixup_bundle( ${CMAKE_CURRENT_BINARY_DIR}/texture.app \"\"   \"${DIRS}\")
    " COMPONENT Runtime)


else() 
  add_executable (texture ${SOURCE_FILES} ) 
endif()

include_directories(${SEBURO_INCLUDES}
  ${INCLUDES_SEARCH_PATHS}
  ${INCLUDES}
)

target_link_libraries(texture 
  Seburo
)
