#!/bin/bash

n=3
i=1
while (( $i <= $n ))
do
    python3 data.py
    time python3 golden.py
    time ./test
    diff out out1 > /dev/null
    if [ $? != 0 ]
    then
        echo "$i" + "failed"
        break
    fi
    let "i++"
    echo "$i"
done