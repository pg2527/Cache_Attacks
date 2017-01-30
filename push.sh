#!/bin/bash
i=0
while true;
do
 i=$((i+1))
 adb pull /data/local/tmp/test/$i.csv /home/user/Documents/Thesis/logs/
 echo $i
done
