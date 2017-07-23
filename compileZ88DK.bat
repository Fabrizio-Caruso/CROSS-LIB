@REM set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase"
@REM set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\cfg"

@REM 
set cc65path="D:\personal\cc65-snapshot-win32"
@REM 
set configpath="D:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\cfg"
@REM 
set mypath="d:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE"


set Deliverables=%mypath%\deliverables

mkdir %deliverables% 2> NUL

@REM del %deliverables%\*.prg
@REM del %deliverables%\*.bin
@REM del %deliverables%\*.xex
@REM del %deliverables%\*.tap
del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o

cd %cc65path%\bin\

@REM 
cd %mypath%\
@REM zcc +zx -compiler=sdcc -DAMALLOC -D_BUILDING_X -clib=ansi -lmalloc -lm -lndos -create-app  -o %deliverables%\ZXSpectrum.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c

@REM zcc +zx -SO3 -pragma-define:ansicolumns=32  -DSPECTRUM_32COL -DDEBUG_SHOW_INVINCIBLE_GHOST_ISSUE -vn -DAMALLOC -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\ZXSpectrum_ISSUE.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c

@REM zcc +zx -SO3 -pragma-define:ansicolumns=32  -DDEBUG_CHARACTERS -DSPECTRUM_32COL -DDEBUG_SPECTRUM_HACK -DDEBUG_SHOW_INVINCIBLE_GHOST_ISSUE -vn -DAMALLOC -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\ZXSpectrum_FIXED.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c

@REM 
zcc +zx -SO3 -pragma-define:ansicolumns=32 -DDEBUG_SPECTRUM_OUT_OF_MEMORY_HACK -DSPECTRUM_32COL  -vn -DAMALLOC -clib=sdcc_iy -lmalloc -lndos -create-app  -o %deliverables%\ZXSpectrum_32col_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
del %deliverables%\ZXSpectrum_32col_experimental.prg

@REM 
@REM zcc +zx -SO3 -pragma-define:ansicolumns=64 -DDEBUG_SPECTRUM_OUT_OF_MEMORY_HACK -DSPECTRUM_64COL  -vn -DAMALLOC -clib=sdcc_iy -lmalloc -lndos -create-app  -o %deliverables%\ZXSpectrum_64col_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\ZXSpectrum_64col_experimental.prg


@REM zcc +zx -pragma-define:ansicolumns=32  -DSPECTRUM_32COL -DDEBUG_SPECTRUM_HACK -DDEBUG_SHOW_INVINCIBLE_GHOST_ISSUE -v -DAMALLOC -compiler=sdcc  --reserve-regs-iy -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\ZXSpectrum_SDCC.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c

@REM 
@REM zcc +sc3000  -vn -DDEBUG_CHARACTERS -DAMALLOC -lmalloc -lndos -create-app  -o %deliverables%\sc3000.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\sc3000.prg

@REM zcc +cpc -vn -DDEBUG_CHARACTERS -DAMALLOC -lmalloc -lndos -create-app  -o %deliverables%\cpc.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\cpc.prg

@REM zcc +msx  -vn -DDEBUG_CHARACTERS -DAMALLOC -lmalloc -lndos -create-app  -o %deliverables%\msx.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\msx.prg

@REM zcc +msx -create-app -D_BUILDING_X  -DAMALLOC -lmalloc -lndos -o %deliverables%\msx-alt.bin %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c 


@REM zcc +cpc -subtype=wav -DDEBUG_CHARACTERS -DAMALLOC -lmalloc -lndos -zorg=16384 -create-app -make-app -o %deliverables%\cpc-alt.bin %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c 
@REM zcc +cpc -subtype=wav -create-app -DAMALLOC -lmalloc -lndos -o %deliverables%\cpc-alt.bin %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c 

@REM zcc +cpc -lndos -lm -subtype=wav -create-app -o program adv_a.c 

@REM zcc +m5 -SO3 -vn  -DDEBUG_CHARACTERS-DAMALLOC -lmalloc -lndos -create-app  -o %deliverables%\m5.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\m5.prg


@REM SDCC
@REM zcc +sc3000 -SO3 -v -DDEBUG_CHARACTERS -DAMALLOC -compiler=sdcc  --reserve-regs-iy -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\sc3000_SDCC.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\sc3000_SDCC.prg

@REM zcc +msx -SO3 -v -DDEBUG_CHARACTERS -DAMALLOC -compiler=sdcc  --reserve-regs-iy -lmalloc -lndos -create-app  -o %deliverables%\msx_SDCC.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\msx_SDCC.prg

@REM zcc +cpc -v -DDEBUG_CHARACTERS -DAMALLOC -compiler=sdcc  --reserve-regs-iy -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\cpc_SDCC.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\cpc_SDCC.prg

@REM zcc +m5 -SO3 -v -DDEBUG_CHARACTERS -DAMALLOC -compiler=sdcc  --reserve-regs-iy -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\m5_SDCC.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\m5_SDCC.prg





@REM zcc +zx -v -DAMALLOC -compiler=sdcc  --reserve-regs-iy -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\ZXSpectrum_SDCC.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c

@REM zcc +zx -v -pragma-define:ansicolumns=32  -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -compiler=sdcc  --reserve-regs-iy -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\ZXSpectrum_SDCC.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c

@REM -SO3 --max-allocs-per-node200000

@REM cl65.exe -O -t c64 --config %configpath%\c64_GFXat0xC000.cfg  %mypath%\graphics\graphics.s  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c64_redefined_characters.prg
@REM cd %mypath%\tools
@REM exomizer sfx basic %deliverables%\c64_redefined_characters.prg -o %deliverables%\c64_redefined_chars.prg
@REM del %deliverables%\c64_redefined_characters.prg
@REM cd %cc65path%\bin\

@REM cl65.exe -O -t c128 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c128.prg

@REM cl65.exe -O -t pet %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cpet.prg

@REM cl65.exe -O -t vic20 --config %mypath%\cfg\vic20-24k.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cvic20-24k.prg

@REM cl65.exe -O -t c16 --config %configpath%\c16_GFX.cfg  %mypath%\c264\c264_sounds.c %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c16-32k_redefined_chars.prg

@REM cl65.exe -O -t c16 --config %configpath%\c16_GFX.cfg  %mypath%\c264\c264_sounds.c %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c16-32k_redefined_characters.prg
@REM cd %mypath%\tools
@REM @REM 
@REM exomizer sfx basic %deliverables%\c16-32k_redefined_characters.prg -o %deliverables%\c16-32k_redefined_chars.prg
@REM @REM 
@REM del %deliverables%\c16-32k_redefined_chars.prg
@REM @REM 
@REM cd %cc65path%\bin\


@REM cl65.exe -O -t cbm510 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cbm510.prg

@REM 
@REM cl65.exe  -O -t atmos --config %mypath%\cfg\atmos_better_tap.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atmos_redefined_chars.tap

@REM cl65.exe  -O -t atarixl --start-addr 0x4000 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atariXL_mode1.xex

@REM apple2 has a "sleep" issue [patched]
@REM 
@REM cl65.exe -O -t apple2 %mypath%\sleep_macros.c %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\apple2.bin
@REM cd %mypath%\tools
@REM cp MASTER_BOOT_ASCHASE.DSK %deliverables%\apple2.dsk
@REM java -jar ac.jar -cc65 %deliverables%\apple2.dsk aschase B < %deliverables%\apple2.bin
@REM del %deliverables%\apple2.bin
@REM cd %cc65path%\bin\

@REM apple2enh has a "sleep" issue
@REM 
@REM cl65.exe -O -t apple2enh %mypath%\sleep_macros.c %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\apple2enh.bin
@REM cd %mypath%\tools
@REM cp MASTER_BOOT_ASCHASE.DSK %deliverables%\apple2enh.dsk
@REM java -jar ac.jar -cc65 %deliverables%\apple2enh.dsk aschase B < %deliverables%\apple2enh.bin
@REM del %deliverables%\apple2enh.bin
@REM cd %cc65path%\bin\





@REM Which emulator to use?
@REM cl65.exe -O -t cbm610 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cbm610.prg

@REM NES has DISPLAY ISSUES
@REM cl65.exe -t nes %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\nes.nes

@REM BROKEN DUE TO CFG
@REM cl65.exe -O -t plus4 --config %configpath%\plus4_GFX.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cplus4_redefined_chars.prg

@REM Memory issue
@REM cl65.exe -O -t atari -Wl -D__RESERVED_MEMORY__=0x2000 --config %mypath%\cfg\atari_mode12.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atari800.xex


@REM TO EXPERIMENT
@REM cl65.exe -O -t c64 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c64.prg


@REM cl65.exe  -lib %cc65path%\target\atarixl\drv\tgi\atrx7.tgi -O -t atarixl %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atariXL_HACK.xex

@REM TARGETS MEMORY LIMITATIONS

@REM vic20-16k has reached its memory limitations
@REM cl65.exe -O -t vic20 --config %mypath%\cfg\vic20-16k.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cvic20-16k.prg

@REM Atari 5200: Out of memory
@REM cl65.exe -O -t atari5200 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atari5200.a52

@REM Atari 2600: Out of memory
@REM cl65.exe -O -t atari2600 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atari2600.a26

@REM vic20-16k has reached its memory limitations
@REM cl65.exe -O -t vic20 --config %mypath%\cfg\vic20-16k.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cvic20-16k.prg

@REM pce runs out of memory
@REM cl65.exe -O -t pce %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\pcengine.bin



@REM TARGETS WITH OTHER ISSUES

@REM cl65.exe -t nes %mypath%\experiments\hello.c -o %mypath%\deliverables\hello.nes

@REM BBC fails looking for bbc.lib
@REM cl65.exe -O -t bbc  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\bbc.bin



@REM BBC fails looking for bbc.lib
@REM cl65.exe -O -t lynx  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\lynx.bin

@REM BBC fails looking for bbc.lib
@REM cl65.exe -O -t supervision  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\supervision.bin

@REM BBC fails looking for bbc.lib
@REM cl65.exe -O -t apple2enh  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\apple2enh.bin

@REM BBC fails looking for bbc.lib
@REM cl65.exe -O -t atarixl  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atarixl.bin

@REM BBC fails looking for bbc.lib
@REM cl65.exe -O -t cbm610  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cbm610.bin




del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
