#!/bin/bash

## Decided to start thinking about what it would
## take to create a build system for c.

function nano_timer() {
  echo $(date +%s%N)
}

function timer_now() {
  current_timer=`nano_timer`
  delta_nanos=`expr $current_timer - $1`
  delta_millis=$(echo "$delta_nanos*10^-09" | bc -l)
  echo $(printf %.2f $delta_millis)s
}

function timer_result() {
  delta_nanos=`expr $2 - $1`
  delta_millis=$(echo "$delta_nanos*10^-09" | bc -l)
  echo $(printf %.2f $delta_millis)s
}

echo "Creating the project in a build directory. Sit tight."

start=`nano_timer`

if [ -d "build/" ]; then
  rm -rf build
fi
mkdir build
pushd build > /dev/null 2>&1
  start_cmake=`nano_timer`
  cmake .. > /dev/null 2>&1
  echo cmake_time: `timer_now $start_cmake`

  start_make=`nano_timer`
  make > /dev/null 2>&1
  echo make_time: `timer_now $start_make`
popd > /dev/null 2>&1

total_time=`timer_now $start`
