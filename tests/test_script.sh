##!/usr/bin/bash

n=0
for run in {1,2,3,4,5,6,7,8,9,10};
do
  echo $n
  ./test
  echo $n
done


