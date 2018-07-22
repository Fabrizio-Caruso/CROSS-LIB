@REM set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase\merged"
@REM set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\merged\experiments"

@REM set cc65path="D:\personal\cc65-snapshot-win32"
@REM set configpath="D:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\minimal\cfg"
@REM set mypath="d:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\minimal"

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


@REM CPC HALF-WORKINg
@REM -DDEBUG_CHARACTERS
@REM -O3
@REM zcc +msx -O3 -create-app -vn -DMSX_MODE1 -D__MSX__ -lndos -create-app -o %deliverables%\MINIMAL_msx_16k_experimental.prg %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM del %deliverables%\MINIMAL_msx_16k_experimental.prg 

@REM 
zcc +msx -O3 -create-app -vn -D__MSX__ -lndos -create-app -o %deliverables%\MINIMAL_msx_16k_experimental_MODE0.prg %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM 
@REM 
del %deliverables%\MINIMAL_msx_16k_experimental_MODE0.prg 


@REM 
@REM -O3 -DDEBUG_CHARS
@REM 
zcc +msx -O3 -DSOUNDS -DREDEFINED_CHARS -create-app -vn -DMSX_MODE1 -DFULL_GAME -D__MSX__ -lndos -create-app -o %deliverables%\FULL_msx_16k_experimental.prg %mypath%\msx\msx_sounds.c %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM 
@REM 
@REM 
del %deliverables%\FULL_msx_16k_experimental.prg 

del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
