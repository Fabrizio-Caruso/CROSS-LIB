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


@REM CPC HALF-WORKINg
@REM -DDEBUG_CHARACTERS
@REM 
zcc +vg5k -vn -DDEBUG_CHARACTERS -D__VG5K__ -lndos -create-app -o %deliverables%\vg5k_experimental.prg %mypath%\display_macros.c %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM 
del %deliverables%\vg5k_experimental.prg 


@REM test character drawing
@REM zcc +vg5k -vn -lndos -create-app -o %deliverables%\testVG5K.prg experiments\testVG5K.c 


del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
