#!/bin/bash

# Run this from the build directory.
# 
# sh ../scene/verify.sh
FILES="../scene/*"
for f in $FILES;
do
  if [[ $f == *.scn ]]
  then
    ./bin/ray-tracing $f instant
  fi
done
echo finished

# Git diff will tell you if the images are different.
# Yeah, quick easy hack. Want to verify refactors work.
