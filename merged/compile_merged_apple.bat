@REM set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase"
@REM set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\cfg"

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


@REM apple2 has a "sleep" issue [patched]
@REM 
@REM 
cl65.exe -O -t apple2 -DFULL_GAME %mypath%\sleep_macros.c %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\apple2.bin
@REM 
cd %mypath%\..\tools
@REM 
copy MASTER_BOOT_ASCHASE.DSK %deliverables%\apple2.dsk
@REM 
java -jar ac.jar -cc65 %deliverables%\apple2.dsk aschase B < %deliverables%\apple2.bin
@REM 
del %deliverables%\apple2.bin
@REM 
cd %cc65path%\bin\

@REM apple2enh has a "sleep" issue
@REM 
@REM 
cl65.exe -O -t apple2enh -DFULL_GAME %mypath%\sleep_macros.c %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\apple2enh.bin
@REM 
cd %mypath%\..\tools
@REM 
copy MASTER_BOOT_ASCHASE.DSK %deliverables%\apple2enh.dsk
@REM 
java -jar ac.jar -cc65 %deliverables%\apple2enh.dsk aschase B < %deliverables%\apple2enh.bin
@REM 
del %deliverables%\apple2enh.bin
@REM 
cd %cc65path%\bin\




del %mypath%\*.o
del %mypath%\atmos\*.o
del %mypath%\c64\*.o
del %mypath%\patch\*.o
