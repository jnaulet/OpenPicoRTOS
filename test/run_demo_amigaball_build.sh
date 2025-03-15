#!/bin/sh

DIR=$(pwd)/../demo/amigaball-lcd
cd $DIR

for m in $(find . -maxdepth 1 -name "Makefile.*"); do
    
    # exclude these
    # [ ! -z $(echo $m | grep pthread-linux) ] && continue
    # [ ! -z $(echo $m | grep win32) ] && continue
    [ ! -z $(echo $m | grep common) ] && continue

    echo ""
    echo "*** Building $m ***"
    echo ""
    
    make -f $m config V=1 || exit 1
    make -f $m V=1 || exit 1
done
