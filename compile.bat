set cc65path="D:\personal\cc65-snapshot-win32"
set mypath="d:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE"

set Deliverables=%mypath%\deliverables

mkdir %deliverables% 2> NUL
del %deliverables%\*

cd %cc65path%\bin\

REM cl65.exe -O -t c64 %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c64.prg

REM cl65.exe -O -t c128 %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c128.prg

REM cl65.exe -O -t pet %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cpet.prg

REM cl65.exe -O -t vic20 --config %cc65path%\cfg\vic20-32k.cfg %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cvic20-24k.prg

cl65.exe -O -t vic20 --config %cc65path%\cfg\vic20-16k.cfg %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cvic20-16k.prg

REM cl65.exe -O -t plus4 %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cplus4.prg

REM cl65.exe -O -t c16 %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\c16-32k.prg

REM cl65.exe -O -t cbm510 %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\cbm510.prg

REM cl65.exe -O -t atari %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atari800.xex

REM apple2 has a "sleep" issue
REM cl65.exe -O -t apple2 %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\apple2.bin

REM NES needs some INPUT
REM cl65.exe -O -t nes %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\nes.bin

REM pce runs out of memory
REM cl65.exe -O -t pce %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\pcengine.bin

REM Atmos has a conio issue
REM cl65.exe -O -t atmos %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %deliverables%\atmos.tap

