
cmake_minimum_required (VERSION 2.8) 
project (openni) 

set(SOURCE_FILES app.cpp)

if (USE_OPENNI)

  get_filename_component(OPENNI_LIBRARY_FILE ${OPENNI_LIBRARY} NAME) 
  get_filename_component(OPENNI_LIBRARY_PATH ${OPENNI_LIBRARY} PATH) 
  get_filename_component(NITE_LIBRARY_FILE ${NITE_LIBRARY} NAME) 
  get_filename_component(NITE_LIBRARY_PATH ${NITE_LIBRARY} PATH) 

  if (_SEBURO_OSX)
    add_executable(openni MACOSX_BUNDLE app.cpp)
    
    SET(MACOSX_BUNDLE_ICON_FILE ${CMAKE_SOURCE_DIR}/s9.icns) 
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_SOURCE_DIR}/s9.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    set_target_properties(openni PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "openni")

    set(DIRS ${CMAKE_SOURCE_DIR}/lib/osx
      ${CMAKE_SOURCE_DIR}/data
      ${CMAKE_SOURCE_DIR}/shaders
    ) 

    set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "uk.co.section9.seburo.openni")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VTK_VERSION})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${VTK_VERSION})
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2013. All Rights Reserved.")

    # Setup the Bundle Files

    add_custom_command (TARGET openni PRE_BUILD
                        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources)

    add_custom_command (TARGET openni PRE_BUILD
                        COMMAND cp -r ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/.)

    add_custom_command (TARGET openni PRE_BUILD
                        COMMAND cp -r ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/.)

    add_custom_command (TARGET openni PRE_BUILD
                        COMMAND cp ${CMAKE_BINARY_DIR}/../lib/osx/* ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/.)

    add_custom_command (TARGET openni PRE_BUILD
                        COMMAND cp ${CMAKE_BINARY_DIR}/libSeburo.dylib ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/.)


    # OpenNI and Nite Libraries
    # We need to copy the libraries from the absolute path and extract the filenames
    # then when they are copied, we must change their IDs and the app reference path
    # We must also copy the drivers files to their correct location
    # We must change the NITE reference to the moved OpenNI


    add_custom_command (TARGET openni PRE_BUILD
                      COMMAND cp ${OPENNI_LIBRARY} ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/.)

    add_custom_command (TARGET openni PRE_BUILD
                      COMMAND cp ${NITE_LIBRARY} ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/.)
  

    # TODO - replace the ini file to actually move the nite2 dir into Resources
    add_custom_command (TARGET openni POST_BUILD
                        COMMAND cp -r ${NITE_LIBRARY_PATH}/NiTE2 ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/MacOS/.)

    add_custom_command (TARGET openni POST_BUILD
                        COMMAND cp -r ${NITE_LIBRARY_PATH}/NiTE2 ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/MacOS/.)


    add_custom_command (TARGET openni PRE_BUILD
                      COMMAND cp -r ${OPENNI_LIBRARY_PATH}/OpenNI2 ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/.)
  
    add_custom_command (TARGET openni POST_BUILD
                      COMMAND install_name_tool -change  ${OPENNI_LIBRARY_FILE} @executable_path/../Resources/${OPENNI_LIBRARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/MacOS/openni)
  
    add_custom_command (TARGET openni POST_BUILD
                      COMMAND install_name_tool -change ${NITE_LIBRARY_FILE} @executable_path/../Resources/${NITE_LIBRARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/MacOS/openni)
  
    add_custom_command (TARGET openni POST_BUILD
                      COMMAND install_name_tool -id  @executable_path/../Resources/${OPENNI_LIBRARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/${OPENNI_LIBRARY_FILE})
    
    add_custom_command (TARGET openni POST_BUILD
                      COMMAND install_name_tool -id  @executable_path/../Resources/${NITE_LIBRARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/${NITE_LIBRARY_FILE})
  
    add_custom_command (TARGET openni POST_BUILD
                      COMMAND install_name_tool -change ${OPENNI_LIBRARY_FILE} @executable_path/../Resources/${OPENNI_LIBRARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/openni.app/Contents/Resources/${NITE_LIBRARY_FILE})
  

    INSTALL(CODE "
      include(BundleUtilities)
      fixup_bundle( ${CMAKE_CURRENT_BINARY_DIR}/openni.app \"\"   \"${DIRS}\")
      " COMPONENT Runtime)

  elseif(_SEBURO_LINUX)
 
    add_executable (openni ${SOURCE_FILES} ) 

    target_link_libraries(openni 
      Seburo
    )

    # Package up for linux.

    # Now Copy NiTE2 and OpenNI data files
    add_custom_command (TARGET openni POST_BUILD
      COMMAND cp ARGS ${NITE_LIBRARY_PATH}/NiTE.ini ${CMAKE_CURRENT_BINARY_DIR} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Copy NiTE.ini"
      COMMAND cp ARGS -r  ${NITE_LIBRARY_PATH}/NiTE2 ${CMAKE_CURRENT_BINARY_DIR} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Copy NiTE2 Data"
    )


  
  endif()

  include_directories(${SEBURO_INCLUDES}
    ${INCLUDES_SEARCH_PATHS}
    ${INCLUDES}
  )

  

   

endif()
