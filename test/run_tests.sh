#!/bin/sh

# STATIC ANALYSIS
DIR=static_analysis

# single core
make -C $DIR/picoRTOS || exit 1
make -C $DIR/picoRTOS cppcheck || exit 1

# multi-core
make -C $DIR/picoRTOS-SMP || exit 1
make -C $DIR/picoRTOS-SMP cppcheck || exit 1

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
