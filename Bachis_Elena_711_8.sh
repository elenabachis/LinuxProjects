#!/bin/bash
if [ $# -lt 2 ] 
then
echo "Nicht genug!"
exit 1
fi
cd Labor3
for(( i=1; $i<=$1 ; i++ ))
do
mkdir $i
cd $i
  for(( j=1; $j<=$2; j++ ))
  do
  touch $j
 done
 cd ..
done


