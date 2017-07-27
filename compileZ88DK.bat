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

@REM -DDEBUG_CHARACTERS
@REM 
zcc +zx -vn -SO3 -DSPECTRUM_NATIVE_DIRECTIVES -DSPECTRUM_64COL -D__SPECTRUM__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\ZXSpectrum_64col_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\ZXSpectrum_64col_experimental.prg

@REM 
@REM 
zcc +zx -vn -SO3 -DSPECTRUM_NATIVE_DIRECTIVES -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\ZXSpectrum_32col_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM 
@REM del %deliverables%\ZXSpectrum_32col_experimental.prg


@REM ANSI 
@REM WORKING: Hello world
@REM zcc +zx -startup=5 -clib=sdcc_iy -vn  -DAMALLOC  -create-app -o %deliverables%\hello64.prg hello.c
@REM zcc +zx -startup=1 -clib=sdcc_iy -vn  -DAMALLOC  -create-app -o %deliverables%\hello32.prg hello.c

@REM pragma recognized but NOT the coordinates
@REM zcc +zx -clib=ansi -pragma-define:ansicolumns=32 -lmalloc -lndos -vn -DAMALLOC  -create-app -o %deliverables%\helloANSI32.prg hello.c
@REM zcc +zx -clib=ansi -pragma-define:ansicolumns=64 -lmalloc -lndos -vn -DAMALLOC  -create-app -o %deliverables%\helloANSI64.prg hello.c


@REM zcc +zx -startup=1 -clib=sdcc_iy -vn  -DDEBUG_CHARACTERS  -pragma-define:ansicolumns=32 -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\ZXSpectrum_32col_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\ZXSpectrum_32col_experimental.prg

@REM zcc +msx -vn -DDEBUG_CHARACTERS -D__MSX__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\msx_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\msx_experimental.prg

@REM zcc +cpc -vn  -D__CPC__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\amstrad_cpc_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c
@REM del %deliverables%\amstrad_cpc_experimental.prg

del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
