#!/bin/bash

rm -f src/wincmoc_tmp.c
rm -f src/wincmoc_input.c

rm -f src/wincmoc_graphics.c

rm -f src/wincmoc_tmp.c

rm -f src/wincmoc_main.c

cp src/wincmoc/wincmoc_input.c  src/wincmoc_input.c

for file in $(ls src/*.c)
do 
	grep -v extern $file >> src/wincmoc_tmp.c
done

cp src/wincmoc/wincmoc.h src/wincmoc_main.c
cat src/wincmoc_tmp.c >> src/wincmoc_main.c

rm -rf src/wincmoc_tmp.c

