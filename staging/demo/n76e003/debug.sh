#!/bin/sh
mkdir -p debug
# picoRTOS files
make deploy
find picoRTOS -type f -exec mv {} $(pwd)/debug \;
# local files
cp *.[ch] debug
cp include/generated debug -fR
cp ../../../arch/8051/include/macros.S debug/
cp Makefile.debug debug/Makefile
