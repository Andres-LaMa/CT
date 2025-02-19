#!/bin/sh

DIR1="folly"
DIR2="./STR_test"
# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP='"[^"\$$*(\\.[^"\$$*)*"'

for f in `find $DIR2 -name "*.cpp"`; do
    echo "*** File $f"
    grep --color=always -E "$REGEXP" $f
done
