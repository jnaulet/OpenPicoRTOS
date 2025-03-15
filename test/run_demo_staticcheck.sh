#!/bin/sh

DIR=$(pwd)/../demo

for d in $(find $DIR -mindepth 1 -maxdepth 1 -type d); do

    echo ""
    echo "*** Executing staticcheck in $d ***"
    echo ""
    
    # exclude these
    [ ! -z $(echo $d | grep pthread-linux) ] && continue
    [ ! -z $(echo $d | grep win32) ] && continue
    # FIXME: EXTRA_CFLAGS issue
    [ ! -z $(echo $d | grep pic32mz) ] && continue
    
    cd $d && make staticcheck V=1 || exit 1
done
