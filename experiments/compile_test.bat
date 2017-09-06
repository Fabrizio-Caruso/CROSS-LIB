@REM set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase\minimal"
@REM set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\minimal\experiments"

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
@REM 
zcc +cpc -vn -clib=ansi -D__CPC__ -lndos -create-app -o %deliverables%\test.prg %mypath%\test.c 
@REM 
cd %mypath%\..\tools\2cdt
@REM 
2cdt.exe -n -r cross_chase %deliverables%\test.cpc  %deliverables%\test.cdt

@REM 
@REM 
del %deliverables%\test.cpc
@REM 
del %deliverables%\test.prg
@REM 


@REM -O3
@REM zcc +cpc -vn -DFULL_GAME -clib=ansi -D__CPC__ -lndos -create-app -o %deliverables%\FULL_cpc_experimental.prg %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c

@REM cd %mypath%\..\tools\2cdt
@REM 2cdt.exe -n -r cross_chase %deliverables%\FULL_cpc_experimental.cpc  %deliverables%\FULL_cpc_experimental.cdt

@REM del %deliverables%\FULL_cpc_experimental.cpc
@REM del %deliverables%\FULL_cpc_experimental.prg
@REM 

del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
