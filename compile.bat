set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase"

cd %cc65path%\bin\

cl65.exe -O -t c64 %mypath%\powerUps.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\display.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input.c %mypath%\main.c  -o %mypath%\c64.prg