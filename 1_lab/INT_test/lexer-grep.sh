#!/bin/sh

DIR1="folly"
DIR2="./INT_test"
# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP="[0-9]+['0-9]*[uU]?[lL]?[lL]?[zZ]?"

for f in `find $DIR2 -name "*.cpp"`; do
    echo "*** File $f"
    grep --color=always -E "$REGEXP" $f
done
