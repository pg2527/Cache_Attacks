#!/bin/bash
j='analysis' 
i=0
mkdir $j-1557
while (true);
do 
   i=$((i+1))
   echo 'wait... We are at '$i' iteration of Template attack'
   ./cache_template_attack  -c 0 -f 1 -t 250 /data/local/tmp/test2/libtest.so > $i-$j-log.csv
   mv $i-$j-log.csv  $j-1557/ 
   echo 'Done!!'
done
