@REM set cc65path="C:\Retro\DEV\cc65-snapshot-win32"
@REM set mypath="C:\Users\Brizio\Documents\GitHub\PortableChase\minimal"
@REM set configpath="C:\Users\Brizio\Documents\GitHub\PortableChase\minimal\experiments"

@REM set cc65path="D:\personal\cc65-snapshot-win32"
@REM set configpath="D:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\merged\experiments"
@REM set mypath="d:\Userfiles\fcaruso\Documents\GitHub\ASCII-CHASE\merged"

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
cl65.exe -O -t pce %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\MINIMAL_pce.pce
@REM cl65.exe -t nes %mypath%\my_hello.c  -o %deliverables%\my_hello.nes

cl65.exe -O -t pce -DFULL_GAME %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\FULL_pce.pce

@REM vic20-16k works on minimal build!
@REM -O
@REM cl65.exe -O -t vic20 -DVIC_SOUNDS -DVERY_SIMPLE_STRATEGY --config %mypath%\..\cfg\vic20-8k.cfg %mypath%\vic20\vic20_sounds.c %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\MINIMAL_cvic20-8k_sounds.prg


@REM cl65.exe -O -t c16 -DC264_SOUNDS -DVERY_SIMPLE_STRATEGY --config %mypath%\..\cfg\c16-16k.cfg %mypath%\c264\c264_sounds.c %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\MINIMAL_c16-16k_sounds.prg


@REM cl65.exe -O -t vic20 -DVIC_SOUNDS -DFULL_GAME -DVERY_SIMPLE_STRATEGY --config %mypath%\..\cfg\vic20-16k.cfg %mypath%\vic20\vic20_sounds.c %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\FULL_cvic20-8k_sounds.prg


@REM cl65.exe -O -t c16 -DFULL_GAME -DC264_SOUNDS -DVERY_SIMPLE_STRATEGY  %mypath%\c264\c264_sounds.c %mypath%\display_macros.c %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\FULL_c16-16k_sounds.prg


@REM cl65.exe -O -t c64 --config %mypath%\..\cfg\c64_GFXat0xC000.cfg  %mypath%\graphics\graphics.s  %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\c64_redefined_characters.prg
@REM 
@REM cd %mypath%\tools
@REM 
@REM exomizer sfx basic %deliverables%\c64_redefined_characters.prg -o %deliverables%\c64_redefined_chars.prg
@REM 
@REM del %deliverables%\c64_redefined_characters.prg
@REM 
@REM cd %cc65path%\bin\

@REM cl65.exe -O -t c128 %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\c128_sounds.prg



@REM cl65.exe -O -D C128_80COL_VIDEO_MODE -t c128 %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\c128_80col_sounds.prg


@REM cl65.exe  -t pet %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\cpet.prg

@REM cl65.exe -O -t cbm510 %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\cbm510.prg

@REM cl65.exe  -t cbm610 %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\MINIMAL_cbm610.prg

@REM cl65.exe  -t cbm610 -DFULL_GAME %mypath%\display_macros.c  %mypath%\enemy.c %mypath%\invincible_enemy.c %mypath%\level.c %mypath%\character.c %mypath%\text.c %mypath%\missile.c %mypath%\strategy.c %mypath%\input_macros.c %mypath%\main.c  -o %deliverables%\FULL_cbm610.prg
