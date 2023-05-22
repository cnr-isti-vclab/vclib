#!/bin/bash

THIS_PATH="$(dirname "$(realpath "$0")")"
VCLIB_PATH=$THIS_PATH/../../include

x1=1
y1=22

x2=1
y2=22

for file in $(find $VCLIB_PATH -name '*.h' -or -name '*.cpp'); do  
    echo $file
    sed "$x2,\$!d;$y2 q" $THIS_PATH/header.txt | sed -i -e "$x1 r /dev/stdin" -e "$x1,$y1 d" $file
done

VCLIB_PATH=$THIS_PATH/../test

for file in $(find $VCLIB_PATH -name '*.h' -or -name '*.cpp'); do  
    echo $file
    sed "$x2,\$!d;$y2 q" $THIS_PATH/header.txt | sed -i -e "$x1 r /dev/stdin" -e "$x1,$y1 d" $file
done