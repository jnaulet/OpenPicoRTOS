#!/bin/sh

# STATIC ANALYSIS
DIR=static_analysis

# Scheduler
make -C $DIR/picoRTOS V=1 || exit 1
make -C $DIR/picoRTOS-SMP V=1 || exit 1

# UNIT TESTS
DIR=unit_tests

# first: no output
make -C $DIR
./$DIR/test_picoRTOS > $DIR.log
./$DIR/test_picoRTOS-SMP > $DIR.log

# second: only show failures
FAIL=$(grep FAIL $DIR.log)
if [ ! -z "$FAIL" ]; then
    cat $DIR.log | grep FAIL
    exit 1
fi

echo ""
echo "*** Test Completed ***"
