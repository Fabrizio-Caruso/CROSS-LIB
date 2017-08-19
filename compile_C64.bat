@REM 
set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM 
set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase"
@REM 
set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\cfg"

@REM set cc65path="D:\personal\cc65-snapshot-win32"
@REM set configpath="D:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\cfg"
@REM set mypath="d:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE"


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

@REM cd %mypath%\
@REM zcc +zx -compiler=sdcc -DAMALLOC -D_BUILDING_X -clib=ansi -lmalloc -lm -lndos -create-app  -o %deliverables%\ZXSpectrum.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c

@REM zcc +zx -vn -DAMALLOC -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\ZXSpectrum.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM zcc +zx -v -DAMALLOC -compiler=sdcc -SO3 --max-allocs-per-node200000 --reserve-regs-iy -clib=ansi -lmalloc -lndos -create-app  -o %deliverables%\ZXSpectrum.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c

@REM 
cl65.exe -O -t c64 --config %configpath%\c64_GFXat0xC000.cfg  %mypath%\graphics\graphics.s  %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\c64_redefined_characters.prg
@REM 
cd %mypath%\tools
@REM 
exomizer sfx basic %deliverables%\c64_redefined_characters.prg -o %deliverables%\c64_redefined_chars.prg
@REM 
del %deliverables%\c64_redefined_characters.prg
@REM 
cd %cc65path%\bin\

@REM 
cl65.exe -O -t c128 %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\c128.prg
