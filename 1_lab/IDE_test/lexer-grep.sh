#!/bin/sh

DIR1="folly"
DIR2="./IDE_test"
# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP="[_a-zA-Z][_a-zA-Z0-9]*"

for f in `find $DIR2 -name "*.cpp"`; do
    echo "*** File $f"
    grep --color=always -E "$REGEXP" $f
done
