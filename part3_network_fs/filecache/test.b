#!/bin/bash
for j in {0..4}
do
     for i in {0..9} 
     do
             X=$(($i * 1024 + 512))
             Y=$(($i * 1024 + 1024))
          echo -n $X 'MB ' >> filecache.rst${j}
             ./ftest ../../../JM/csetwo21/testfile_1.txt $X >> filecache.rst${j}
          echo -n $Y 'MB ' >> filecache.rst${j}
             ./ftest ../../../JM/csetwo21/testfile_2.txt $Y >> filecache.rst${j}
     done
done 