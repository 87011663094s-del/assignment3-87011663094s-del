#!/bin/sh

if [ $# -ne 2 ]; then
    echo "Error: Two arguments are required: <directory> <search_string>"
    exit 1
fi

filesdir=$1
searchstr=$2

if [ ! -d "$filesdir" ]; then
    echo "Error: Directory $filesdir does not exist."
    exit 1
fi

X=$(find "$filesdir" -type f | wc -l)
Y=$(grep -r "$searchstr" "$filesdir" | wc -l)

echo "The number of matching lines are $Y and number of files are $X"
