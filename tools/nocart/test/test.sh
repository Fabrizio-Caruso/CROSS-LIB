#!/bin/sh
set -eu
pasmo --amsdos test.asm test.bin

cpcxfs -f -nd test.dsk
cpcxfs test.dsk -f -p test.bin test.bin
../nocart test.dsk test.cpr -c 'RUN"test"'

cpcxfs -f "DA4A" test.dsk
cpcxfs test.dsk -f -p test.bin test.bin
../nocart -f "DA4A" test.dsk test.cpr -c 'RUN"test"'



