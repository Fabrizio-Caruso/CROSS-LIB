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

@REM 
cd %mypath%\

@REM Working 64 column version without vertical outer wall - SLOW KEYBOARD INPUT with getk() but OK with in_Inkey()
@REM -DDEBUG_CHARACTERS
@REM 
@REM zcc +zx -vn -SO3 -DSPECTRUM_NATIVE_DIRECTIVES -DSPECTRUM_64COL -D__SPECTRUM__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\ZXSpectrum_64col_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\ZXSpectrum_64col_experimental.prg

@REM BROKEN!
@REM Working 32 column version with vertical outer wall - SLOW KEYBOARD INPUT with getk() but OK with in_Inkey()
@REM -DDEBUG_CHARACTERS
@REM 
@REM zcc +zx -DDEBUG_CHARACTERS -vn -DSPECTRUM_NATIVE_DIRECTIVES -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\ZXSpectrum_32col_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\ZXSpectrum_32col_experimental.prg

@REM DEBUGGING
@REM zcc +zx -vn -startup=1 -clib=sdcc_iy experiments\testBUG.c -o %deliverables%\testBUG -create-app

@REM WORKING but with problematic input and not outer wall - FAST KEYBOARD INPUT
@REM optimize with:  -SO3 --max-allocs-per-node200000
@REM -DDEBUG_CHARACTERS
@REM -DDEBUG_SOUNDS
@REM -pragma-define:REGISTER_SP=-1 -pragma-define:CLIB_MALLOC_HEAP_SIZE=0
@REM -DDEBUG_CRASH
@REM zcc +zx -c -startup=1 -clib=sdcc_iy -vn -DSPECTRUM_UDG -DSPECTRUM_32COL -D__SPECTRUM__ -o %deliverables%\input.o %mypath%\input_macros.c
@REM 
zcc +zx -startup=1 -SO3 --max-allocs-per-node200000 -clib=sdcc_iy -vn -DSPECTRUM_UDG -DSPECTRUM_32COL -D__SPECTRUM__ -create-app -o %deliverables%\ZXSpectrum_32col_sdcc_UDG_experimental.prg %mypath%\sleep_macros.c %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\ZXSpectrum_32col_sdcc_UDG_experimental.prg
@REM 
del %deliverables%\ZXSpectrum_32col_sdcc_UDG_experimental_CODE.bin

@REM zcc +zx -startup=1 -clib=new -vn -DSPECTRUM_UDG -DSPECTRUM_32COL -D__SPECTRUM__ -create-app -o %deliverables%\ZXSpectrum_32col_new_UDG_experimental.prg %mypath%\sleep_macros.c %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c


@REM zcc +zx -startup=1 -clib=sdcc_iy -vn -DDEBUG_CRASH -DSPECTRUM_UDG -DSPECTRUM_32COL -D__SPECTRUM__ -create-app -o %deliverables%\ZXSpectrum_CRASH.prg %mypath%\sleep_macros.c %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\ZXSpectrum_CRASH.prg
@REM del %deliverables%\ZXSpectrum_CRASH_CODE.bin

@REM zcc +zx -startup=1 -clib=sdcc_iy -vn -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -create-app -o %deliverables%\ZXSpectrum_32col_sdcc_experimental.prg %mypath%\sleep_macros.c %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\ZXSpectrum_32col_sdcc_experimental.prg
@REM del %deliverables%\ZXSpectrum_32col_sdcc_experimental_CODE.bin

@REM WORKING but with problematic input and not outer wall - FAST KEYBOARD INPUT
@REM optimize with:  -SO3 --max-allocs-per-node200000
@REM 
@REM zcc +zx  -SO3 -startup=5 -clib=sdcc_iy -vn  -DSPECTRUM_64COL -D__SPECTRUM__ -DAMALLOC -create-app -o %deliverables%\ZXSpectrum_64col_sdcc_experimental.prg %mypath%\sleep_macros.c %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\ZXSpectrum_64col_sdcc_experimental.prg
@REM 
@REM del %deliverables%\ZXSpectrum_64col_sdcc_experimental_CODE.bin




@REM ANSI TESTzcc +cpc -clib=ansi -lndos -create-app -o %deliverables%\cpc_ansitest %mypath%\experiments\ansitest.c
@REM cd %mypath%\tools\2cdt
@REM 2cdt.exe -n -r ansitest %deliverables%\cpc_ansitest.cpc  %deliverables%\ansitest.cdt



@REM HALF-WORKING CPC
@REM zcc +cpc -vn -DDEBUG_CHARACTERS -D__CPC__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\cpc.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\cpc.prg 
@REM del %deliverables%\cpc.cpc
@REM cd %mypath%\tools\2cdt
@REM 2cdt.exe -n -r cross_chase %deliverables%\cpc.cpc  %deliverables%\cpc.cdt

@REM IT STARTS!
@REM zcc +vg5k -vn -DDEBUG_CHARACTERS -D__CPC__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\vf5k.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c

@REM zcc +mz -vn -DDEBUG_CHARACTERS -D__CPC__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\vf5k.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c




@REM ----------------------------------------------------------------------------------------------------------------------------------------------------
@REM NON_WORKING




@REM zcc +msx -vn  -DSPECTRUM_NATIVE_DIRECTIVES -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\msx_32col_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\msx_32col_experimental.prg

@REM 
@REM zcc +cpc -vn  -subtype=wav -DDEBUG_CHARACTERS -DSPECTRUM_NATIVE_DIRECTIVES -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\cpc_32col_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\cpc_32col_experimental.prg




@REM 
@REM not working because of missing dependencies
@REM zcc +cpc -startup=1 -clib=sdcc_iy -vn  -DDEBUG_CHARACTERS  -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -create-app -o %deliverables%\cpc_32col_sdcc_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM zcc +msx -startup=1 -clib=sdcc_iy -vn  -DDEBUG_CHARACTERS  -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -create-app -o %deliverables%\msx_32col_sdcc_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM zcc +sms -startup=1 -clib=sdcc_iy -vn  -DDEBUG_CHARACTERS  -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -create-app -o %deliverables%\sms_32col_sdcc_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM zcc +z80 -startup=1 -clib=sdcc_iy -vn  -DDEBUG_CHARACTERS  -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -create-app -o %deliverables%\z80_32col_sdcc_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM zcc +cpm -startup=1 -clib=sdcc_iy -vn  -DDEBUG_CHARACTERS  -DSPECTRUM_32COL -D__SPECTRUM__ -DAMALLOC -create-app -o %deliverables%\cpm_32col_sdcc_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c



@REM zcc +msx -vn -DDEBUG_CHARACTERS -D__MSX__ -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\msx.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\msx.prg

@REM zcc +mz -clib=ansi  -lndos -create-app -o %deliverables\mz_ansitest ansitest.c
@REM zcc +zx -clib=ansi -lndos -create-app -o %deliverables\zx_ansitest ansitest.c

@REM WORKING: Hello world
@REM zcc +zx -startup=5 -clib=sdcc_iy -vn  -DAMALLOC  -create-app -o %deliverables%\hello64.prg my_hello.c
@REM zcc +zx -startup=1 -clib=sdcc_iy -vn  -DAMALLOC  -create-app -o %deliverables%\hello32.prg my_hello.c

@REM pragma recognized but NOT the coordinates because the code should use real VT100/ANSI directives
@REM zcc +zx -clib=ansi -pragma-define:ansicolumns=32 -lmalloc -lndos -vn -DAMALLOC  -create-app -o %deliverables%\helloANSI32.prg hello.c
@REM zcc +zx -clib=ansi -pragma-define:ansicolumns=64 -lmalloc -lndos -vn -DAMALLOC  -create-app -o %deliverables%\helloANSI64.prg hello.c

REM zcc +msx -vn -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\msxhello.prg my_hello.c
REM zcc +cpc -subtype=wav -vn -DAMALLOC -lmalloc -lndos -create-app -o %deliverables%\cpchello.prg cpc_hello.c



del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
