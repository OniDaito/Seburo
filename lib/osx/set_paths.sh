#!/bin/bash

# Script to change the install paths for libraries in the OSX Directory

in="install_name_tool -change "

libs=(*)
num_libs=${#libs[*]}

for lib in *.dylib ; do 
        i=0
        echo ""
        echo "${lib}"
        echo "-------------------------------------------------"
        install_name_tool -id "@executable_path/../../../../../../lib/osx/${lib}" "${lib}" 
        lines=`otool -L  ${lib}`
        for lib_name in ${lines} ; do 
                for local_lib in ${libs[@]}; do
                        in=`echo ${lib_name} | grep -i ${local_lib}`
                        if [ "$in" != "" ] ; then
                                echo "${lib_name} -  ${local_lib}"
                                install_name_tool -change "${lib_name}" "@executable_path/../../../../../../lib/osx/${local_lib}" ${lib}
                                echo  install_name_tool -change "${lib_name}" "@executable_path/../../../../../../lib/osx/${local_lib}" ${lib}
                        fi      
                done

                #echo ${lib_name}
                #echo "."
                #match=$((echo "${lines}" grep -i ${libs[$i]}`))
                #echo $lines
                #echo ${libs[$i]}
                #i=$(($i+1))
        done
done