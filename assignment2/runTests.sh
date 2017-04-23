#!/bin/sh

COMPILER=./compiler488
PREFIX=test_

FILES=`ls $PREFIX*`

for FILE in $FILES
do
    head -1 $FILE | cut -c 3-
    $COMPILER $FILE
done
