#!/bin/bash
j='Xor' 
i=0
z=0
for k in 1..4 ;
 do
  z=$((z+1))
  mkdir $j-$z
  for  p in 1..20 ;
   do 
     i=$((i+1))
     #echo 'wait... We are at '$i' iteration of Template attack'
     echo 'Step '$i''
     cache_template_attack  -c 0 -f 1 -t 250 /home/user/parul/veda\ courseware/LinuxPro/lib/libxyz.so > $i-log.csv
     mv  $i-log.csv  $j-$z/
     # echo 'wait... We are at '$i' iteration of Template attack'
   done
done
