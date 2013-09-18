#!/bin/bash

# Sets bundle paths for any files in the osx lib dir that may have escaped

 otool -L $1 | grep osx | tr -d ':' | while read -a libs ; do
      echo ${libs[0]}
     #[ "${file}" != "${libs[0]}" ] && install_name_tool -change ${libs[0]} `basename ${libs[0]}` libSeburo.dylib
 done