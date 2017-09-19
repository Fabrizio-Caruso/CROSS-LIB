@REM set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase\minimal"
@REM set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\minimal\experiments"

@REM set cc65path="D:\personal\cc65-snapshot-win32"
@REM set configpath="D:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\minimal\experiments"
@REM set mypath="d:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\minimal"

@REM 
set cc65path="C:\cc65-snapshot-win32"
@REM 
set configpath="C:\Users\Brizio\Retro\ASCII-CHASE\merged\cfg"
@REM 
set mypath="C:\Users\Brizio\Retro\ASCII-CHASE\merged"


set Deliverables=%mypath%\deliverables

mkdir %deliverables% 2> NUL

@REM del %deliverables%\*
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o

cd %cc65path%\bin\



@REM 
cl65.exe  -O -t atarixl -DATARI_MODE1 -DFULL_GAME -DSOUNDS --start-addr 0x4000 %mypath%\atari\atari_sounds.c %mypath%\atari\atari_input.c %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\FULL_atariXL_mode1_sounds.xex

@REM cl65.exe  -O -t atarixl -DFULL_GAME -DSOUNDS %mypath%\atari\atari_sounds.c %mypath%\atari\atari_input.c  %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\FULL_atariXL_mode0_sounds.xex

@REM cl65.exe  -O -t atari -DFULL_GAME -DSOUNDS %mypath%\atari\atari_sounds.c %mypath%\atari\atari_input.c %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\FULL_atari_mode0_sounds.xex

@REM cl65.exe  -O -t atari -DMINIMAL_GAME %mypath%\atari\atari_input.c %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\MINIMAL_atari_mode0.xex
