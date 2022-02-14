#!/bin/bash

function nano_timer() {
  echo $(date +%s%N)
}

function timer_now() {
  current_timer=`nano_timer`
  delta_nanos=`expr $current_timer - $1`
  delta_millis=$(echo "$delta_nanos*10^-09" | bc -l)
  echo $(printf %.2f $delta_millis)s
}

if [ ! -d "build/" ]; then
  sh bench/build.sh
else
  pushd build > /dev/null 2>&1
    start_make=`nano_timer`
    cmake .. /dev/null 2>&1
    make > /dev/null 2>&1
    echo make_time: `timer_now $start_make`
  popd > /dev/null 2>&1
fi

start=`nano_timer`

build/bin/ray-tracing $1 $2

echo render_$1: `timer_now $start`
