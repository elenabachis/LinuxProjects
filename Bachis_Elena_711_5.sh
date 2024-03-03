#!/bin/bash

if [ $# -lt 2 ]
then
echo 'Nicht genug!'
exit 1
fi

path1=$(realpath "$1")
path2=$(realpath "$2")
mv "$path2" "$path1/$(basename "$path2")"

cd "$path1/$(basename "$path2")"
for file in *; do
if [ -f "$file" ]; then
     mv "$file" "${file%.*}.eins"
fi
done

