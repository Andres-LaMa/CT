#!/bin/sh

DIR="folly"

# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP="[_a-zA-Z][_a-zA-Z0-9]*"

for f in `find $DIR -name "*.cpp"`; do
    echo "*** File $f"
    grep -o -E "$REGEXP" $f
done
