#!/bin/bash

WINCMOC_SOURCE=../src/cross_lib/wincmoc
SOURCE=../src/chase

rm -f $SOURCE/wincmoc_tmp.c
rm -f $SOURCE/wincmoc_input.c
rm -f $SOURCE/wincmoc_graphics.c
rm -f $SOURCE/wincmoc_main.c

cp $WINCMOC_SOURCE/wincmoc_input.c  $SOURCE/wincmoc_input.c

for file in $(ls $SOURCE/*.c)
do 
	grep -v extern $file >> $SOURCE/wincmoc_tmp.c
done

cp $WINCMOC_SOURCE/wincmoc.h $SOURCE/wincmoc_main.c
cat $SOURCE/wincmoc_tmp.c >> $SOURCE/wincmoc_main.c

rm -rf $SOURCE/wincmoc_tmp.c

