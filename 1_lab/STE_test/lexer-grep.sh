#!/bin/sh

DIR="folly"

# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP="0[xX][0-9a-fA-F]+"

for f in `find $DIR -name "*.cpp"`; do
    echo "*** File $f"
    grep -o -E "$REGEXP" $f
done
