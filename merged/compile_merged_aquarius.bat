@REM set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase\merged"
@REM set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\merged\experiments"

@REM set cc65path="D:\personal\cc65-snapshot-win32"
@REM set configpath="D:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\merged\cfg"
@REM set mypath="d:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\merged"

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
@REM %mypath%\aquarius\aquarius_input.c
@REM -clib=ansi
@REM 
@REM 
zcc +aquarius  -vn -D__AQUARIUS__ -lndos -o MINIMAL_aquarius_16k -create-app %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM zcc +aquarius -clib=ansi -vn -D__AQUARIUS__ -lndos -create-app -o MINIMAL_aquarius_16k test_getk.c
@REM 
del %mypath%\MINIMAL_aquarius_16k
@REM 
move %mypath%\MINIMAL_aquarius_16k.caq %deliverables%
@REM 
move %mypath%\_MINIMAL_aquarius_16k.caq %deliverables%

@REM zcc +aquarius -clib=ansi -vn -D__AQUARIUS__ -lndos -o test test_getk.c


@REM 
zcc +aquarius -vn -D__AQUARIUS__ -DFULL_GAME -lndos -o FULL_aquarius_16k -create-app  %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c
@REM 
@REM 
del %mypath%\FULL_aquarius_16k
@REM 
move %mypath%\FULL_aquarius_16k.caq %deliverables%
@REM 
move %mypath%\_FULL_aquarius_16k.caq %deliverables%


del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
