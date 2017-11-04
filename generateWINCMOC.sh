#!/bin/bash

rm -f src/wincmoc_main.c

for file in $(ls src/*.c)
do 
	grep -v extern $file >> src/wincmoc_main.c
done

cat src/wincmoc/wincmoc.h >> src/wincmoc_main.c
