@REM 
set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM 
set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase\minimal"
@REM 
set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\minimal\experiments"

@REM set cc65path="D:\personal\cc65-snapshot-win32"
@REM set configpath="D:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\minimal\cfg"
@REM set mypath="d:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\minimal"


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
zcc +cpc -O3 -vn -clib=ansi -D__CPC__ -lndos -create-app -o %deliverables%\MINIMAL_cpc_experimental.prg %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c

cd %mypath%\..\tools\2cdt
2cdt.exe -n -r cross_chase %deliverables%\MINIMAL_cpc_experimental.cpc  %deliverables%\MINIMAL_cpc_experimental.cdt

@REM 
del %deliverables%\MINIMAL_cpc_experimental.cpc
del %deliverables%\MINIMAL_cpc_experimental.prg
@REM del %deliverables%\cpc.cpc


del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
