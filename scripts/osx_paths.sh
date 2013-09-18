#!/bin/bash

# Sets bundle paths for any files in the osx lib dir that may have escaped

install_name_tool -id  @executable_path/../Resources/libSeburo.dylib libSeburo.dylib
 otool -L libSeburo.dylib | grep osx | tr -d ':' | while read -a libs ; do
      echo ${libs[0]}
     [ "${file}" != "${libs[0]}" ] && install_name_tool -change ${libs[0]} @executable_path/../Resources/`basename ${libs[0]}` libSeburo.dylib
 done