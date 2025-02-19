#!/bin/sh

DIR1="folly"
DIR2="./DIR_test"
# a
# aa
# aaaaa
REGEXP="^\s*#\s*[a-zA-Z_]+"

for f in `find $DIR2 -name "*.cpp"`; do
    echo "*** File $f"
    grep --color=always -E "$REGEXP" $f
done
