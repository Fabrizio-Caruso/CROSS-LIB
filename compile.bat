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


@REM cl65.exe -O -t c64 --config %configpath%\c64_GFXat0xC000.cfg  %mypath%\graphics\graphics.s  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c64_redefined_characters.prg
@REM cd %mypath%\tools
@REM exomizer sfx basic %deliverables%\c64_redefined_characters.prg -o %deliverables%\c64_redefined_chars.prg
@REM del %deliverables%\c64_redefined_characters.prg
@REM cd %cc65path%\bin\

@REM cl65.exe -O -t c128 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c128.prg

@REM cl65.exe -O -t pet %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cpet.prg

@REM cl65.exe -O -t vic20 --config %mypath%\cfg\vic20-24k.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cvic20-24k.prg


@REM 
cl65.exe -O -t c16 --config %configpath%\c16_GFX.cfg  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c16-32k_redefined_chars.prg

@REM cl65.exe -O -t cbm510 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cbm510.prg

@REM cl65.exe -O -t cbm610 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cbm610.prg

@REM cl65.exe -O -t atari -Wl -D__RESERVED_MEMORY__=0x2000 --config %mypath%\cfg\atari_mode12.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atari800.xex

@REM cl65.exe  -O -t atmos --config %mypath%\cfg\atmos_better_tap.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atmos_redefined_chars.tap

@REM cl65.exe  -O -t atarixl --start-addr 0x4000 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atariXL.xex

@REM apple2 has a "sleep" issue
@REM 
@REM cl65.exe -O -t apple2 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\apple2.bin
@REM cd %mypath%\tools
@REM cp MASTER.DSK %deliverables%\apple2.dsk
@REM java -jar ac.jar -cc65 %deliverables%\apple2.dsk aschase B < %deliverables%\apple2.bin
@REM del %deliverables%\MASTER.DSK
@REM del %deliverables%\apple2.bin
@REM cd %cc65path%\bin\

@REM apple2enh has a "sleep" issue
@REM 
@REM cl65.exe -O -t apple2enh %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\apple2enh.bin
@REM cd %mypath%\tools
@REM cp MASTER.DSK %deliverables%\apple2enh.dsk
@REM java -jar ac.jar -cc65 %deliverables%\apple2enh.dsk aschase B < %deliverables%\apple2enh.bin
@REM del %deliverables%\MASTER.DSK
@REM del %deliverables%\apple2enh.bin
@REM cd %cc65path%\bin\



@REM NES has DISPLAY ISSUES
@REM cl65.exe -t nes %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\nes.nes

@REM BROKEN DUE TO CFG
@REM cl65.exe -O -t plus4 --config %configpath%\plus4_GFX.cfg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cplus4_redefined_chars.prg


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
