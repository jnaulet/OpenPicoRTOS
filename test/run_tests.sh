#!/bin/sh

# UNIT TESTS
DIR=unit_tests

# first: no output
make -C $DIR
./$DIR/picoRTOS > $DIR.log

# second: only show failures
FAIL=$(grep FAIL $DIR.log)
if [ ! -z "$FAIL" ]; then
    cat $DIR.log | grep FAIL
    exit 1
fi

echo ""
echo "*** Test Completed ***"
