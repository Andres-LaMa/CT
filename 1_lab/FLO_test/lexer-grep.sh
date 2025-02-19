#!/bin/sh

DIR1="folly"
DIR2="./FLO_test"
# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP="[0-9]*\.[0-9]+([eE][+-]?[0-9]+)?[fFlL]?|[0-9]+[eE][+-]?[0-9]+[fFlL]?"

for f in `find $DIR2 -name "*.cpp"`; do
    echo "*** File $f"
    grep --color=always -E "$REGEXP" $f
done
