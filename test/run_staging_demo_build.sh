#!/bin/sh

DIR=$(pwd)/../staging/demo

for d in $(find $DIR -mindepth 1 -maxdepth 1 -type d); do

    # exclude these
    # [ ! -z $(echo $d | grep pthread-linux) ] && continue
    # [ ! -z $(echo $d | grep win32) ] && continue
    
    cd $d && make V=1 || exit 1
done
