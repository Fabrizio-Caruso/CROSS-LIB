@REM set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase\merged"
@REM set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\merged\cfg"

@REM set cc65path="D:\personal\cc65-snapshot-win32"
@REM set configpath="D:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\cfg"
@REM set mypath="d:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE"

@REM 
set cc65path="C:\cc65-snapshot-win32"
@REM 
set configpath="C:\Users\Brizio\Retro\ASCII-CHASE\merged\cfg"
@REM 
set mypath="C:\Users\Brizio\Retro\ASCII-CHASE\merged"


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


@REM WORKING but with problematic input and not outer wall - FAST KEYBOARD INPUT
@REM optimize with:  -SO3 --max-allocs-per-node200000
@REM -DDEBUG_CHARACTERS
@REM -DDEBUG_SOUNDS
@REM -pragma-define:REGISTER_SP=-1 -pragma-define:CLIB_MALLOC_HEAP_SIZE=0
@REM -DDEBUG_CRASH
@REM 
@REM 
zcc +zx -startup=1 -SO3 --max-allocs-per-node200000 -clib=sdcc_iy -vn -DFULL_GAME -DREDEFINED_CHARS -DSOUNDS -DSPECTRUM_32COL -D__SPECTRUM__ -create-app -o %deliverables%\FULL_zx_spectrum_redefined_chars.prg %mypath%\sleep_macros.c %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM 
@REM 
@REM 
del %deliverables%\FULL_zx_spectrum_redefined_chars_CODE.bin
@REM 
del %deliverables%\FULL_zx_spectrum_redefined_chars_BANK_07.bin


@REM 
zcc +zx -startup=1 -SO3 --max-allocs-per-node200000 -clib=sdcc_iy -vn -DREDEFINED_CHARS -DSOUNDS -DSPECTRUM_32COL -D__SPECTRUM__ -create-app -o %deliverables%\MINIMAL_zx_spectrum_redefined_chars.prg %mypath%\sleep_macros.c %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM 
@REM 
@REM 
del %deliverables%\MINIMAL_zx_spectrum_redefined_chars_CODE.bin
@REM 
del %deliverables%\MINIMAL_zx_spectrum_redefined_chars_BANK_07.bin


@REM 
zcc +zx -startup=1 -SO3 --max-allocs-per-node200000 -clib=sdcc_iy -vn -DSOUNDS -DSPECTRUM_32COL -D__SPECTRUM__ -create-app -o %deliverables%\MINIMAL_zx_spectrum.prg %mypath%\sleep_macros.c %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM 
@REM 
@REM 
del %deliverables%\MINIMAL_zx_spectrum_CODE.bin
@REM 
del %deliverables%\MINIMAL_zx_spectrum_BANK_07.bin


del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
