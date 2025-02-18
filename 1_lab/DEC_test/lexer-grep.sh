#!/bin/sh

DIR="folly"

# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP="[0-9]+"

for f in `find $DIR -name "*.cpp"`; do
    echo "*** File $f"
    grep -o -E "$REGEXP" $f
done
