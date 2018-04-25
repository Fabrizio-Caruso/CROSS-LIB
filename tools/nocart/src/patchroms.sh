#!/bin/sh
rm os.rom
rm basic.rom
rm amsdos.rom
pasmo --bin patchos.asm os.rom
pasmo --bin patchbasic.asm basic.rom
pasmo --bin patchdos.asm amsdos.rom
