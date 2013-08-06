#!/bin/bash

if [ $# -ne 1 ]
then
  echo "Usage: `basename $0` {PATH_TO_build_number.txt}"
  exit $E_BADARGS
fi

number=`cat $1/build_number.txt`
let number++
echo "$number" | tee build_number.txt #<-- output and save the number back to file
svnver=$(head -n 1 $1/svnversion.h)
echo $svnver > $1/svnversion.h
echo "#define BUILD_NUMBER ""$number" >> $1/svnversion.h