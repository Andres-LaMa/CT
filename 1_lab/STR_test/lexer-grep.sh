#!/bin/sh

DIR="folly"

# ^ $ * + ? {}
# a
# aa
# aaaaa
REGEXP='"[^"\$$*(\\.[^"\$$*)*"'

for f in `find $DIR -name "*.cpp"`; do
    echo "*** File $f"
    grep -o -E "$REGEXP" $f
done
