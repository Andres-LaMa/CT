#!/bin/sh

DIR1="folly"
DIR2="./STE_test"
# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP="0[xX][0-9a-fA-F]+"

for f in `find $DIR2 -name "*.cpp"`; do
    echo "*** File $f"
    grep --color=always -E "$REGEXP" $f
done
