#!/bin/bash

RUNFILE="a.out"
LOOPS=100
OUTFILE="$1$2.rst"

if [ ! -f $RUNFILE ]; then
     echo "Compiling EXE..."
     g++ -O0 -lpthread -std=c++11 test.cpp -o $RUNFILE
fi

##rm -f $OUTFILE

for ((jj=0; jj<$LOOPS; jj++));
do
     ## $RUNFILE $1 $2 >> $OUTFILE
     $RUNFILE $1 $2
done
     


















