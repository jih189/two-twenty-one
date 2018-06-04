#!/bin/bash
for i in {0..4} 
do
        X=$(($i * 1024 + 512))
        Y=$(($i * 1024 + 1024))
	echo -n $X 'MB '
        ./ftest testfile8G_1.txt $X
	echo -n $Y 'MB '
        ./ftest testfile8G_2.txt $Y
done
