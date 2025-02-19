#!/bin/sh

DIR1="folly"
DIR2="./OCT_test"
# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP="0[0-7]+"

for f in `find $DIR2 -name "*.cpp"`; do
    echo "*** File $f"
    grep --color=always -E "$REGEXP" $f
done
