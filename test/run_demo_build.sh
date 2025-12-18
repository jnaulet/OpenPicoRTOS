#!/bin/sh

DIR=$(pwd)/../demo

for d in $(find $DIR -mindepth 1 -maxdepth 1 -type d); do

    # exclude these
    # [ ! -z $(echo $d | grep pthread-linux) ] && continue
    # [ ! -z $(echo $d | grep win32) ] && continue
    [ ! -z $(echo $d | grep amigaball-lcd) ] && continue
    # commercial toolchains
    [ ! -z $(echo $d | grep devkit-mpc5748g) ] && continue
    [ ! -z $(echo $d | grep curiosity-2.0-pic32mz-ef) ] && continue
    
    cd $d && make V=1 || exit 1
done
