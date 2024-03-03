#!/bin/bash

if [ $# -eq 0 ]
then
echo 'Nicht genug!'
exit 1
fi


for dir in $@; do
	cd $dir
	grep -r -U -l "" *  #ich suche rekursiv und betrachte alle Dateien aus dem Verzeichnis und aus allen Unterverzeichnissen als binar
	cd
done
