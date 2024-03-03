#!/bin/bash

if [[ -z "$1" ]]
then
echo "Nicht Genug!"
exit 1
fi

#Loschen Sie alle Worter mit mindestens einer Ziffer mit sed 

sed -E 's/\<[a-zA-Z]*[0-9]{1,}[a-zA-Z]*\>//g' $1
