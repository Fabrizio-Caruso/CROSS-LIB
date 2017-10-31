#!/bin/bash

rm -f src/cmoc_main.c
for file in $(ls src/*.c)
do 
	grep -v extern $file >> src/cmoc_main.c
done

