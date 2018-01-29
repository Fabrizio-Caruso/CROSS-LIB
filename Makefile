# Makefile for CROSS-CHASE 
 

ifneq ($(COMSPEC),)
DO_WIN:=1
endif
ifneq ($(ComSpec),)
DO_WIN:=1
endif 

ifeq ($(DO_WIN),1)
EXEEXT = .exe
endif

SOURCE_PATH := src

CC65_PATH ?= /cygdrive/c/cc65-snapshot-win32/bin/
#CC65_PATH ?= /home/fcaruso/cc65/bin/
Z88DK_PATH ?= /cygdrive/c/z88dk/bin/
Z88DK_PATH_20171210 ?= /cygdrive/c/z88dk_20171210/bin/
Z88DK_INCLUDE ?= /cygdrive/c/z88dk/include
BUILD_PATH ?= build
MYCC65 ?= cl65$(EXEEXT)
MYZ88DK ?= zcc$(EXEEXT)
MYZ88DKASM ?= z80asm$(EXEEXT)
TOOLS_PATH ?= ./tools

COCO_OPTS_TINY  ?= -O0 -D__CMOC__ -DASM_KEY_DETECT -DCMOC_RAND_FIX -DTINY_GAME -DALT_PRINT

COCO_OPTS_LIGHT ?= -O0 -D__CMOC__ -DASM_KEY_DETECT -DCMOC_RAND_FIX -DALT_PRINT

COCO_OPTS       ?= -O0 -D__CMOC__ -DASM_KEY_DETECT -DCMOC_RAND_FIX -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL -DALT_PRINT


# -DNO_SLEEP

# ------------------------------------------------------------------------------------------
#CC65
# 
atari_color: 
	$(CC65_PATH)$(MYCC65) -O -t atari \
	-DREDEFINED_CHARS -DFULL_GAME -DATARI_MODE1 -DSOUNDS -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(SOURCE_PATH)/../cfg/atari_mode1_redefined_chars.cfg \
	$(SOURCE_PATH)/atari/disable_setcursor.s \
	$(SOURCE_PATH)/atari/atari_sounds.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/atari/atari_mode1_redefined_chars_graphics.c  $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_atari_color.xex

atari_no_color: 
	$(CC65_PATH)$(MYCC65) -O -t atari \
	-DFULL_GAME -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	$(SOURCE_PATH)/atari/atari_sounds.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_atari_no_color.xex

atari_no_color_16k: 
	$(CC65_PATH)$(MYCC65) -O -t atari \
	-DSOUNDS $(SOURCE_PATH)/atari/atari_sounds.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  -o \
	$(BUILD_PATH)/LIGHT_atari_no_color_16k.xex

atari5200: 
	$(CC65_PATH)$(MYCC65) -O -t atari5200 \
	--config $(SOURCE_PATH)/../cfg/atari5200_less_stack.cfg \
	-DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_atari5200.rom
	
atmos: 
	$(CC65_PATH)$(MYCC65)  -O  -DSOUNDS -DREDEFINED_CHARS -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-t atmos \
	--config $(SOURCE_PATH)/../cfg/atmos_better_tap.cfg $(SOURCE_PATH)/atmos/atmos_redefined_characters.c \
	$(SOURCE_PATH)/atmos/atmos_input.c  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_atmos_and_oric1_48k.tap

oric1_16k: 
	$(CC65_PATH)$(MYCC65)  -O -D__ORIC1__ -DSOUNDS -DREDEFINED_CHARS \
	-t atmos --config $(SOURCE_PATH)/../cfg/atmos_better_tap.cfg $(SOURCE_PATH)/atmos/atmos_redefined_characters.c \
	$(SOURCE_PATH)/atmos/atmos_input.c  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/item.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/LIGHT_oric1_16k.tap

vic20_unexpanded: 
	$(CC65_PATH)$(MYCC65) -O -Cl -t vic20 \
	-DVIC20_UNEXPANDED \
	-DALT_PRINT -DNO_SLEEP -DLESS_TEXT -DNO_SET_SCREEN_COLORS \
	-DTINY_GAME -DVIC20_UNEXPANDED -DNO_RANDOM_LEVEL -DNO_INITIAL_SCREEN -DNO_MESSAGE \
	--config $(SOURCE_PATH)/../cfg/vic20_unexpanded.cfg  \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/vic20/vic20_unexpanded.c \
	$(SOURCE_PATH)/enemy.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/TINY_vic20_unexpanded.prg	
	
vic20_exp_3k:
	$(CC65_PATH)$(MYCC65) -O  -t vic20 \
	-DALT_PRINT -DREDEFINED_CHARS -DNO_SLEEP -DLESS_TEXT -DNO_SET_SCREEN_COLORS -DTINY_GAME -DSOUNDS \
	--config $(SOURCE_PATH)/../cfg/vic20-3k_GFX.cfg \
	$(SOURCE_PATH)/vic20/vic20_sounds_3k.c \
	$(SOURCE_PATH)/vic20/udc_3k.s  \
	$(SOURCE_PATH)/vic20/vic20_graphics_3k.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  \
	-o $(BUILD_PATH)/TINY_vic20_exp_3k.prg

vic20_exp_8k: 
	$(CC65_PATH)$(MYCC65) -O  -t vic20  -DSOUNDS -DREDEFINED_CHARS --config $(SOURCE_PATH)/../cfg/vic20-8k_GFX.cfg \
	$(SOURCE_PATH)/vic20/udc.s $(SOURCE_PATH)/vic20/vic20_graphics.c   \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  $(SOURCE_PATH)/invincible_enemy.c  \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/input_macros.c  $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/vic20/vic20_sounds.c $(SOURCE_PATH)/missile.c \
	--code-name CODE2 \
	$(SOURCE_PATH)/display_macros.c  \
	$(SOURCE_PATH)/item.c $(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/LIGHT_vic20_exp_8k.prg

vic20_exp_16k: 
	$(CC65_PATH)$(MYCC65) -O -t vic20 -DREDEFINED_CHARS -DFULL_GAME -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	--config $(SOURCE_PATH)/../cfg/vic20-16k_GFX.cfg $(SOURCE_PATH)/vic20/udc.s \
	$(SOURCE_PATH)/vic20/vic20_graphics.c $(SOURCE_PATH)/vic20/vic20_sounds.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_vic20_exp_16k.prg

	
c64: 
	$(CC65_PATH)$(MYCC65) -O -t c64 \
	-DFULL_GAME -DREDEFINED_CHARS -DSOUNDS -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(SOURCE_PATH)/../cfg/c64_GFXat0xC000.cfg  $(SOURCE_PATH)/c64/graphics.s \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/c64/c64_redefined_characters.c $(SOURCE_PATH)/display_macros.c  \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_c64.prg
	$(TOOLS_PATH)/exomizer sfx basic $(BUILD_PATH)/FULL_c64.prg -o $(BUILD_PATH)/FULL_c64_exomized.prg
	rm $(BUILD_PATH)/FULL_c64.prg

c128_40col: 
	$(CC65_PATH)$(MYCC65) -O -t c128 \
	-DFULL_GAME -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_c128_40col.prg

c128_80col: 
	$(CC65_PATH)$(MYCC65) -O -t c128  \
	-D C128_80COL_VIDEO_MODE -DFULL_GAME -DSOUNDS -DBETWEEN_LEVEL -DEND_SCREEN \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c  \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_c128_80col.prg

# -DNO_SLEEP -DLESS_TEXT -DNO_RANDOM_LEVEL
# -DSOUNDS $(SOURCE_PATH)/c264/c264_sounds.c
# -Cl 
c16_16k: 
	$(CC65_PATH)$(MYCC65) -O -t c16 --config $(SOURCE_PATH)/../cfg/c16-16k.cfg \
	-DREDEFINED_CHARS  -DSOUNDS \
	$(SOURCE_PATH)/c264/c264_sounds.c  $(SOURCE_PATH)/c264/c264_graphics.c  \
	$(SOURCE_PATH)/item.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/LIGHT_c16_16k.prg
	
c16_32k: 
	$(CC65_PATH)$(MYCC65) -O -t c16 --config $(SOURCE_PATH)/../cfg/c16-32k.cfg \
	-DREDEFINED_CHARS -DFULL_GAME -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	$(SOURCE_PATH)/c264/c264_graphics.c $(SOURCE_PATH)/c264/c264_sounds.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  \
	-o $(BUILD_PATH)/FULL_c16_32k.prg

pet_8k: 
	$(CC65_PATH)$(MYCC65) -O -t pet \
	-DTINY_GAME -DNO_SLEEP -DLESS_TEXT \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/TINY_pet_8k.prg
	
pet_16k: 
	$(CC65_PATH)$(MYCC65) -O -t pet \
	-DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_pet_16k.prg

cbm610: 
	$(CC65_PATH)$(MYCC65) -O -t cbm610 -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_cbm610.prg
	
cbm510: 
	$(CC65_PATH)$(MYCC65) -O -t cbm510 -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_cbm510.prg

nes: 
	$(CC65_PATH)$(MYCC65) -O -t nes -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_nes.nes
	
apple2:	 
	$(CC65_PATH)$(MYCC65) -O -t apple2 -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/sleep_macros.c \
	-o $(BUILD_PATH)/apple2.bin
	cp $(SOURCE_PATH)/../tools/MASTER_BOOT_ASCHASE.DSK $(BUILD_PATH)/FULL_apple2.dsk
	java -jar $(SOURCE_PATH)/../tools/ac.jar -cc65 $(BUILD_PATH)/FULL_apple2.dsk aschase B < $(BUILD_PATH)/apple2.bin
	rm $(BUILD_PATH)/apple2.bin

apple2enh: 
	$(CC65_PATH)$(MYCC65) -O -t apple2enh -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/sleep_macros.c \
	-o $(BUILD_PATH)/apple2enh.bin
	cp $(SOURCE_PATH)/../tools/MASTER_BOOT_ASCHASE.DSK $(BUILD_PATH)/FULL_apple2enh.dsk
	java -jar $(SOURCE_PATH)/../tools/ac.jar -cc65 $(BUILD_PATH)/FULL_apple2enh.dsk aschase B < $(BUILD_PATH)/apple2enh.bin
	rm $(BUILD_PATH)/apple2enh.bin

osic1p_32k: 
	$(CC65_PATH)$(MYCC65) --start-addr 0x200 -Wl -D,__HIMEM__=0x8000 -O -t osic1p -DFULL_GAME \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/sleep_macros.c \
	-o $(BUILD_PATH)/FULL_osic1p_32k.lod
	$(TOOLS_PATH)/srec_cat $(BUILD_PATH)/FULL_osic1p_32k.lod -binary -offset 0x200 -o $(BUILD_PATH)/FULL_osic1p_32k.c1p -Ohio_Scientific -execution-start-address=0x200	
	rm $(BUILD_PATH)/FULL_osic1p_32k.lod
	mv $(BUILD_PATH)/FULL_osic1p_32k.c1p $(BUILD_PATH)/FULL_osic1p_32k.lod

# -Wl -D,__HIMEM__=0x8000	
# -DNO_INITIAL_SCREEN
osic1p_8k: 
	$(CC65_PATH)$(MYCC65) -Cl --start-addr 0x200 -Wl -D,__HIMEM__=0x2000 -O --config $(SOURCE_PATH)/../cfg/osic1p_less_stack.cfg -t osic1p -DROUND_ENEMIES -DNO_SLEEP  -DNO_RANDOM_LEVEL -DLESS_TEXT -DNO_SET_SCREEN_COLOR -DTINY_GAME $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_osic1p_8k.lod
	$(TOOLS_PATH)/srec_cat $(BUILD_PATH)/TINY_osic1p_8k.lod -binary -offset 0x200 -o $(BUILD_PATH)/TINY_osic1p_8k.c1p -Ohio_Scientific -execution-start-address=0x200	
	rm $(BUILD_PATH)/TINY_osic1p_8k.lod
	mv $(BUILD_PATH)/TINY_osic1p_8k.c1p $(BUILD_PATH)/TINY_osic1p_8k.lod
	
		
gamate: 
	$(CC65_PATH)$(MYCC65) -O -t gamate -Cl  --config $(SOURCE_PATH)/../cfg/gamate_reduced_stack.cfg -DFULL_GAME \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/gamate/gamate_graphics.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_gamate.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/FULL_gamate.bin

creativision_8k: 
	$(CC65_PATH)$(MYCC65) -O -t creativision --config $(SOURCE_PATH)/../cfg/creativision-8k.cfg -DTINY_GAME \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_creativision_8k.bin

	
# ------------------------------------------------------------------------------------------
#Z88DK

aquarius_exp_4k:
	$(Z88DK_PATH)$(MYZ88DK) +aquarius -compiler=sdcc -SO3 --max-allocs-per-node200000 -vn \
	-DALT_PRINT -D__AQUARIUS__ -DTINY_GAME -DNO_SLEEP -DLESS_TEXT -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL \
	-pragma-include:$(SOURCE_PATH)/../cfg/zpragma_clib.inc \
	-lndos -o TINY_aquarius_exp_4k -create-app \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/aquarius/aquarius_graphics.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(SOURCE_PATH)/../TINY_aquarius_exp_4k
	mv $(SOURCE_PATH)/../TINY_aquarius_exp_4k.caq $(BUILD_PATH)
	mv $(SOURCE_PATH)/../_TINY_aquarius_exp_4k.caq $(BUILD_PATH)


aquarius_exp_16k: 
	$(Z88DK_PATH)$(MYZ88DK) +aquarius -clib=ansi -vn \
	-DSOUNDS -D__AQUARIUS__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-o FULL_aquarius_exp_16k -create-app \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(SOURCE_PATH)/../FULL_aquarius_exp_16k
	mv $(SOURCE_PATH)/../FULL_aquarius_exp_16k.caq $(BUILD_PATH)
	mv $(SOURCE_PATH)/../_FULL_aquarius_exp_16k.caq $(BUILD_PATH)
	
vz200_16k: 	
	$(Z88DK_PATH)$(MYZ88DK) +vz -O3 -vn \
	-DSOUNDS -D__VZ__ -clib=ansi \
	-lndos \
	-create-app -o $(BUILD_PATH)/LIGHT_vz200_16k.vz \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_vz200_16k.cas
	
vz200_32k: 
	$(Z88DK_PATH)$(MYZ88DK) +vz -O3 -vn \
	-DSOUNDS -D__VZ__ -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-clib=ansi -lndos -create-app -o  $(BUILD_PATH)/FULL_vz200_32k.vz \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_vz200_32k.cas
	
# TODO: Adapt code to work with -compiler=sdcc
# -SO3 --max-allocs-per-node200000
#  -DNO_RANDOM_LEVEL
# $(SOURCE_PATH)/sleep_macros.c
# -DLESS_TEXT -DNO_SLEEP
vg5k: 
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-O3 -zorg=18941 -vn \
	-D__VG5K__ -DSOUNDS  \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_vg5k.prg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/sleep_macros.c \
	$(SOURCE_PATH)/vg5k/vg5k_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_vg5k.prg
	
vg5k_exp_16k:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-O3 -zorg=18941 \
	-DSOUNDS -vn -DFULL_GAME -D__VG5K__ -DBETWEEN_LEVEL -DEND_SCREEN \
	-lndos -create-app -o $(BUILD_PATH)/FULL_vg5k_exp_16k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/sleep_macros.c  \
	$(SOURCE_PATH)/vg5k/vg5k_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_vg5k_exp_16k.prg
	
ace_exp_16k:
	$(Z88DK_PATH)$(MYZ88DK) +ace -O3 \
	-D__ACE__ -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-clib=ansi -o full -Cz--audio -create-app \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	cp full.wav $(BUILD_PATH)/FULL_ace_exp_16k.wav
	rm full.wav
	rm full.tap
	rm full

	
zx80_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 -O3 -vn \
	-D__ZX80__ -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx80_16k.prg \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_zx80_16k.prg

# zx81_16k_no_text:
	# $(Z88DK_PATH)$(MYZ88DK) +zx81 -O3 -vn -D__ZX81__ -DLESS_TEXT -DNO_SLEEP -DFULL_GAME -lndos -create-app -o  $(BUILD_PATH)/FULL_zx81_16k_no_text.prg $(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	# rm $(BUILD_PATH)/FULL_zx81_16k_no_text.prg

# zx81_16k:
	# $(Z88DK_PATH)$(MYZ88DK) +zx81 -O3 -vn -D__ZX81__ -DFULL_GAME -lndos -create-app -o  $(BUILD_PATH)/FULL_zx81_16k.prg $(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	# rm $(BUILD_PATH)/FULL_zx81_16k.prg
	
# -compiler=sdcc	 -DLESS_TEXT -DNO_SLEEP -DBETWEEN_LEVEL
zx81_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -O3 -vn -D__ZX81__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o  $(BUILD_PATH)/FULL_zx81_16k.prg \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_zx81_16k.prg
	
lambda_16k:
	$(Z88DK_PATH)$(MYZ88DK) +lambda -O3 \
	-vn -D__LAMBDA__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o  $(BUILD_PATH)/FULL_lambda_16k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_lambda_16k.prg		
	
cpc:
	$(Z88DK_PATH)$(MYZ88DKASM) -v \
	-x$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lib \
	@$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lst	
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 \
	-DREDEFINED_CHARS -DSOUNDS -DFULL_GAME -clib=ansi -D__CPC__ -DCPCRSLIB -DBETWEEN_LEVEL -DEND_SCREEN \
	-l$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib -lndos \
	-create-app -o $(BUILD_PATH)/FULL_cpc.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/sleep_macros.c \
	$(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc.cpc  $(BUILD_PATH)/FULL_cpc.cdt
	rm $(BUILD_PATH)/FULL_cpc.cpc 
	rm $(BUILD_PATH)/FULL_cpc.prg

msx_color_16k:
	$(Z88DK_PATH)$(MYZ88DK) +msx -O3 -zorg=49200 \
	-DSOUNDS -DREDEFINED_CHARS -create-app -vn -DMSX_MODE1 -D__MSX__ -lndos \
	-create-app -o $(BUILD_PATH)/LIGHT_msx_color_16k.prg \
	$(SOURCE_PATH)/msx/msx_graphics.c $(SOURCE_PATH)/psg/psg_sounds.c \
	$(SOURCE_PATH)/item.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \	
	rm $(BUILD_PATH)/LIGHT_msx_color_16k.prg 	
	
msx_color_32k:
	$(Z88DK_PATH)$(MYZ88DK) +msx -O3 \
	-DSOUNDS -DREDEFINED_CHARS \
	-create-app -vn -DMSX_MODE1 -DFULL_GAME -D__MSX__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_msx_color_32k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/msx/msx_graphics.c $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_msx_color_32k.prg 

msx_color_32k_rom:
	$(Z88DK_PATH)$(MYZ88DK) +msx -O3 \
	-DSOUNDS -DREDEFINED_CHARS -vn -DMSX_MODE1 -DFULL_GAME -D__MSX__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -subtype=rom \
	-o $(BUILD_PATH)/FULL_msx_color_32k.rom \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/msx/msx_graphics.c $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_msx_color_32k_BSS.bin
	rm $(BUILD_PATH)/FULL_msx_color_32k_DATA.bin
	
svi_318_mode0:
	$(Z88DK_PATH)$(MYZ88DK) +svi -O3 -zorg=49200 -vn -lndos \
	-D__SVI__ -DMSX_MODE0 \
	-create-app -o  $(BUILD_PATH)/LIGHT_svi_318_mode0 \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/svi/svi_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_SVI_318_mode0

# too big for a 16k machine ?
# -DSOUNDS $(SOURCE_PATH)/psg/psg_sounds.c
# 
svi_318_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +svi -SO3 --max-allocs-per-node200000 -zorg=49152 \
	-clib=ansi -compiler=sdcc -pragma-define:ansicolumns=32 -vn -lndos \
	-DTINY_GAME \
	-D__SVI__ -create-app -o $(BUILD_PATH)/TINY_svi_318 \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	#rm $(BUILD_PATH)/TINY_svi_318

svi_318:
	$(Z88DK_PATH)$(MYZ88DK) +svi -SO3 --max-allocs-per-node200000 -zorg=49152 \
	-clib=ansi -compiler=sdcc \
	-pragma-define:ansicolumns=32 \
	-pragma-include:$(SOURCE_PATH)/../cfg/zpragma_clib.inc \
	-vn -lndos \
	-DLESS_TEXT -DNO_SLEEP -DNO_RANDOM_LEVEL \
	-D__SVI__ -create-app -o $(BUILD_PATH)/LIGHT_svi_318 \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	#rm $(BUILD_PATH)/LIGHT_svi_318	
	

sharp_mz:
	$(Z88DK_PATH)$(MYZ88DK) +mz -O3 \
	-D__MZ__ -clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DFULL_GAME -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o $(BUILD_PATH)/FULL_sharp_mz.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_sharp_mz.prg
	mv $(BUILD_PATH)/FULL_sharp_mz.mzt $(BUILD_PATH)/FULL_sharp_mz.mzf
	
microbee_16k:
	$(Z88DK_PATH)$(MYZ88DK) +bee -O3 \
	-D__BEE__ -clib=ansi -vn -DSOUNDS -DCLIB_ANSI \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_microbee.prg  \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_microbee.prg
	
microbee:
	$(Z88DK_PATH)$(MYZ88DK) +bee -O3 \
	-D__BEE__ -clib=ansi -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o $(BUILD_PATH)/FULL_microbee.prg  \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_microbee.prg

# import as data into ram at 32768 - call 32768
samcoupe:
	$(Z88DK_PATH)$(MYZ88DK) +sam -O3 \
	-D__SAM__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	-DFULL_GAME -DSOUNDS -DCLIB_ANSI -o $(BUILD_PATH)/FULL_samcoupe.bin -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	
mtx:
	$(Z88DK_PATH)$(MYZ88DK) +mtx -startup=2 -O3 \
	-D__MTX__ -clib=ansi -pragma-define:ansicolumns=32 -create-app -o FULL.bin -vn \
	-DFULL_GAME -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm FULL.bin
	rm FULL.wav
	mv FULL $(BUILD_PATH)/FULL_mtx.mtx
	
abc80_16k:
	$(Z88DK_PATH)$(MYZ88DK) +abc80 -lm -subtype=hex -zorg=49200 -O3 \
	-D__ABC80__ -clib=ansi -vn -DSOUNDS -DCLIB_ANSI -lndos -create-app -o a \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm a
	mv a.ihx $(BUILD_PATH)/LIGHT_abc.ihx 
	
abc80_32k:
	$(Z88DK_PATH)$(MYZ88DK) +abc80 -lm -subtype=hex -zorg=49200 -O3 \
	-D__ABC80__ -clib=ansi -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o a \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm a
	mv a.ihx $(BUILD_PATH)/FULL_abc.ihx 

p2000_16k:
	$(Z88DK_PATH)$(MYZ88DK) +p2000 -O3 -clib=ansi -D__P2000__ -vn \
	-DSOUNDS -DCLIB_ANSI \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_p2000.prg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_p2000.prg
	
p2000_32k:
	$(Z88DK_PATH)$(MYZ88DK) +p2000 -O3 -clib=ansi -D__P2000__ -vn \
	-DFULL_GAME -DSOUNDS -DCLIB_ANSI -DBETWEEN_LEVEL -DEND_SCREEN \
	-lndos -create-app -o $(BUILD_PATH)/FULL_p2000.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_p2000.prg	
	
# KEYBOARD INPUT PROBLEM
# -DFULL_GAME -DSOUNDS
z9001_16k:
	$(Z88DK_PATH)$(MYZ88DK) +z9001 -O3 -clib=ansi \
	-D__Z9001__ -vn  -DCLIB_ANSI \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_z9001.z80 \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	rm $(BUILD_PATH)/LIGHT_z9001.z80
	
z9001_32k:
	$(Z88DK_PATH)$(MYZ88DK) +z9001 -O3 -clib=ansi \
	-D__Z9001__ -vn -DFULL_GAME -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o $(BUILD_PATH)/FULL_z9001.z80 \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	rm $(BUILD_PATH)/FULL_z9001.z80	

mc1000_16k:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 -O3 \
	-pragma-define:ansicolumns=32 -subtype=gaming -clib=ansi -D__MC1000__ -DSOUNDS -vn \
	-DCLIB_ANSI \
	-lndos -create-app -Cz--audio \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv a.wav $(BUILD_PATH)/LIGHT_mc1000_16k.wav
	rm a.bin
	rm a.cas

mc1000_48k:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 -O3 \
	-subtype=gaming -pragma-define:ansicolumns=32 \
	-DFULL_GAME  \
	-clib=ansi \
	-D__MC1000__ -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	-vn -DCLIB_ANSI -lndos -create-app -Cz--audio \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv a.wav $(BUILD_PATH)/FULL_mc1000_48k.wav
	rm a.bin
	rm a.cas		


gal_22k:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-pragma-need=ansiterminal \
	-D__GAL__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-vn -lndos -create-app -o  $(BUILD_PATH)/FULL_galaksija.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_galaksija.prg	
	rm $(BUILD_PATH)/FULL_galaksija.wav
	
# Warning at file 'c:/z88dk/\lib\pc6001_crt0.asm' line 112: integer '66384' out of range
# Warning at file 'stdio/ansi/pc6001/f_ansi_char.asm' line 46: integer '66657' out of range
pc6001_32k:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 -O3 -Cz--audio -clib=ansi -subtype=32k \
	-D__PC6001__ -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o $(BUILD_PATH)/FULL_pc6001_32k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_pc6001_32k.prg
	rm $(BUILD_PATH)/FULL_pc6001_32k.wav
	mv $(BUILD_PATH)/FULL_pc6001_32k.cas $(BUILD_PATH)/FULL_pc6001_32k.cp6

	
# -DLESS_TEXT -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL 
# -SO3 --max-allocs-per-node200000
spectrum_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx --opt-code-size  -startup=1 -zorg=24055 -SO3 --max-allocs-per-node200000 \
	-pragma-include:$(SOURCE_PATH)/../cfg/zpragma.inc -clib=sdcc_iy \
	-DNO_SLEEP -DLESS_TEXT -DTINY_GAME -vn  -D__SPECTRUM__ \
	-create-app -o $(BUILD_PATH)/TINY_spectrum_16k.prg \
	$(SOURCE_PATH)/spectrum/spectrum_graphics.c $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_spectrum_16k_CODE.bin 
	rm $(BUILD_PATH)/TINY_spectrum_16k_UNASSIGNED.bin
	
# -pragma-redirect:ansifont=_font_8x8_zx_system -pragma-define:ansifont_is_packed=0
spectrum_48k:
	$(Z88DK_PATH)$(MYZ88DK) +zx -O3 -clib=ansi -vn  \
	-pragma-redirect:ansifont=_udg -pragma-define:ansifont_is_packed=0 -pragma-define:ansicolumns=32 \
	-DFULL_GAME -DREDEFINED_CHARS -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL -D__SPECTRUM__ \
	-lndos -create-app \
	-o $(BUILD_PATH)/FULL_spectrum_48k.prg \
	$(SOURCE_PATH)/sleep_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/spectrum/udg.asm $(SOURCE_PATH)/spectrum/spectrum_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_spectrum_48k.prg
	rm $(BUILD_PATH)/FULL_spectrum_48k_BANK_7.bin	


pc6001_16k:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 -O3 -Cz--audio -clib=ansi -subtype=32k \
	-D__PC6001__ -vn -DCLIB_ANSI \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_pc6001.prg  \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/sleep_macros.c  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_pc6001.prg
	
	
# kbhit KO
# Everything displayed on the same line
nascom_32k:
	$(Z88DK_PATH)$(MYZ88DK) +nascom -O3 -clib=ansi -vn -lndos \
	-D__NASCOM__ -DCLIB_ANSI -D__NASCOM__ -DSOUNDS -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o $(BUILD_PATH)/FULL_nascom_32k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_nascom_32k.prg
	
	
# -DSOUNDS
# -pragma-define:ansicolumns=32  -Cz-audio 
nascom_16k:
	$(Z88DK_PATH)$(MYZ88DK) +nascom -O3 -clib=ansi -vn -lndos \
	-D__NASCOM__ -DCLIB_ANSI -D__NASCOM__ -DSOUNDS  \
	-create-app -o $(BUILD_PATH)/LIGHT_nascom_16k.prg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	rm $(BUILD_PATH)/LIGHT_nascom_16k.prg
	
	
	
# ------------------------------------

coco:
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/display_macros.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/cmoc/cmoc_graphics.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/enemy.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/level.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/character.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/text.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/strategy.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/input_macros.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/item.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/missile.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/invincible_enemy.c	
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/cmoc/cmoc_input.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/horizontal_missile.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/rocket.c 
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/end_screen.c	
	cmoc -o $(BUILD_PATH)/Full_CoCoDragon.bin \
	$(COCO_OPTS) \
	$(SOURCE_PATH)/main.c \
	display_macros.o cmoc_graphics.o \
	enemy.o \
	level.o character.o text.o \
	strategy.o \
	input_macros.o cmoc_input.o \
	item.o missile.o invincible_enemy.o \
	rocket.o horizontal_missile.o end_screen.o
		


.PHONY: mtx vic20exp_8k vic20exp_16k  atari_color atari_no_color atari_no_color_16k atari5200 atmos c128_40col c128_80col c16_16k c16_32k c64 pet cbm510 cbm610 nes apple2 apple2enh

# KO: 
# OK: 25
# ------------
cc65_targets: \
	vic20_unexpanded vic20_exp_3k vic20_exp_8k vic20_exp_16k \
	atari_color atari_no_color atari_no_color_16k \
	atmos oric1_16k \
	c16_16k c16_32k \
	pet_8k pet_16k cbm510 cbm610 \
	apple2 apple2enh \
	c64 c128_40col c128_80col \
	atari5200 creativision_8k nes gamate \
	osic1p_8k osic1p_32k


# KO:      \
# OK: 35
z88dk_targets: \
	ace_exp_16k  cpc vg5k vg5k_exp_16k svi_318_color svi_318_mode0 sharp_mz \
	samcoupe mtx abc80_16k abc80_32k p2000_16k p2000_32k \
	msx_color_16k msx_color_32k_rom msx_color_32k spectrum_16k spectrum_48k \
	zx81_16k aquarius_exp_4k aquarius_exp_16k vz200_16k \
	z9001_16k z9001_32k mc1000_16k mc1000_48k pc6001_16k pc6001_32k nascom_16k \
	lambda_16k nascom_32k zx80_16k vz200_32k microbee gal_22k

cmoc_targets: \
	coco
	
all: cc65_targets z88dk_targets cmoc_targets

clean:
	rm -rf $(BUILD_PATH)/*
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(SOURCE_PATH)/apple2/*.o
	rm -rf $(SOURCE_PATH)/aquarius/*.o
	rm -rf $(SOURCE_PATH)/atari/*.o
	rm -rf $(SOURCE_PATH)/atmos/*.o
	rm -rf $(SOURCE_PATH)/c64/*.o
	rm -rf $(SOURCE_PATH)/c264/*.o
	rm -rf $(SOURCE_PATH)/c128/*.o
	rm -rf $(SOURCE_PATH)/vic20/*.o
	rm -rf $(SOURCE_PATH)/msx/*.o
	rm -rf $(SOURCE_PATH)/cpc/*.o
	rm -rf $(SOURCE_PATH)/svi/*.o
	rm -rf $(SOURCE_PATH)/vg5k/*.o
	rm -rf $(SOURCE_PATH)/spectrum/*.o
	rm -rf $(SOURCE_PATH)/graphics/*.o
	rm -rf $(SOURCE_PATH)/patch/*.o


help:
	cat BUILD.txt
	cat TARGETS.txt

list:
	cat TARGETS.txt
	

	
	
	
	
	
	

####################################################################################################################
	
# DEBUG


vic20_targets: \
	vic20_unexpanded vic20_exp_3k vic20_exp_8k vic20_exp_16k


# -DNO_MESSAGE \
# 	-compiler=sdcc \
#	-SO3 --max-allocs-per-node200000 \
# -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL
# -DALT_PRINT
# -pragma-include:$(SOURCE_PATH)/../cfg/zpragma.inc

gal_6k: 
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-O3 \
	-pragma-need=ansiterminal \
	-DTINY_GAME -DNO_SLEEP -DLESS_TEXT -D__GAL__ -DNO_RANDOM_LEVEL -DALT_PRINT -DNO_MESSAGE \
	-vn -lndos -create-app -Cz--audio -o  $(BUILD_PATH)/TINY_galaksija_6k.prg \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_galaksija_6k.prg
	#rm $(BUILD_PATH)/TINY_galaksija_6k.wav
	



# It hangs if compiled with sdcc.
# syntax error if compiled with sccz80
# $(SOURCE_PATH)/svi/svi_graphics.c	
svi_328:
	$(Z88DK_PATH)$(MYZ88DK) +svi -O3 \
	-clib=ansi -pragma-define:ansicolumns=32 -vn -lndos \
	-DSOUNDS -DFULL_GAME -D__SVI__ -DBETWEEN_LEVEL -DEND_SCREEN \
	-create-app -o $(BUILD_PATH)/FULL_svi_328 \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_svi_328



error_cc65:
	$(CC65_PATH)$(MYCC65) -t vic20 $(SOURCE_PATH)/../experiments/error.c -o $(BUILD_PATH)/error_cc65.prg

error_z88dk:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k $(SOURCE_PATH)/../experiments/error.c -o $(BUILD_PATH)/error_z88dk.prg

error_cmoc:
	cmoc -o $(BUILD_PATH)/error_cmoc.bin $(SOURCE_PATH)/../experiments/error.c 
#	rm -f cmoc.exe.stackdump

# CMOC

# coco_mono: 
	# cmoc \
	# -D __CMOC__ -DASM_KEY_DETECT -DTINY_GAME -DNO_SLEEP	-DLESS_TEXT \
	# $(SOURCE_PATH)/cmoc/cmoc_graphics.c \
	# $(SOURCE_PATH)/cmoc/cmoc_input.c \
	# $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	# $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	# $(SOURCE_PATH)/main.c	

coco_tiny:
	cmoc \
	$(COCO_OPTS_TINY) \
	-c $(SOURCE_PATH)/display_macros.c
	cmoc \
	$(COCO_OPTS_TINY) \
	-c $(SOURCE_PATH)/cmoc/cmoc_graphics.c
	cmoc \
	$(COCO_OPTS_TINY) \
	-c $(SOURCE_PATH)/enemy.c
	cmoc \
	$(COCO_OPTS_TINY) \
	-c $(SOURCE_PATH)/level.c
	cmoc \
	$(COCO_OPTS_TINY) \
	-c $(SOURCE_PATH)/character.c
	cmoc \
	$(COCO_OPTS_TINY) \
	-c $(SOURCE_PATH)/text.c
	cmoc \
	$(COCO_OPTS_TINY) \
	-c $(SOURCE_PATH)/strategy.c
	cmoc \
	$(COCO_OPTS_TINY) \
	-c $(SOURCE_PATH)/input_macros.c
	cmoc \
	$(COCO_OPTS_TINY) \
	-c $(SOURCE_PATH)/cmoc/cmoc_input.c
	cmoc -o $(BUILD_PATH)/coco_tiny.bin \
	$(COCO_OPTS_TINY) \
	$(SOURCE_PATH)/main.c \
	display_macros.o cmoc_graphics.o \
	enemy.o \
	level.o character.o text.o \
	strategy.o \
	input_macros.o cmoc_input.o
	
coco_light:
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/display_macros.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/cmoc/cmoc_graphics.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/enemy.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/level.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/character.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/text.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/strategy.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/input_macros.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/item.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/missile.c
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/invincible_enemy.c	
	cmoc \
	$(COCO_OPTS_LIGHT) \
	-c $(SOURCE_PATH)/cmoc/cmoc_input.c
	cmoc -o $(BUILD_PATH)/coco_light.bin \
	$(COCO_OPTS_LIGHT) \
	$(SOURCE_PATH)/main.c \
	display_macros.o cmoc_graphics.o \
	enemy.o \
	level.o character.o text.o \
	strategy.o \
	input_macros.o cmoc_input.o \
	item.o missile.o invincible_enemy.o 	
	

	
	
cmoc_link:
	cmoc display_macros.o cmoc_graphics.o \
	enemy.o \
	level.o character.o text.o \
	strategy.o \
	input_macros.o cmoc_input.o \
	main.o

lwlink_link:
	lwlink display_macros.o cmoc_graphics.o \
	enemy.o \
	level.o character.o text.o \
	strategy.o \
	input_macros.o cmoc_input.o \
	main.o	

pet_8k_LIGHT:
	$(CC65_PATH)$(MYCC65) -O -t pet -DLESS_TEXT -DNO_SLEEP $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_pet_8k.prg

	
# oric1_48k:
	# $(CC65_PATH)$(MYCC65)  -O -D__ORIC1__ -DREDEFINED_CHARS -DFULL_GAME -t atmos --config $(SOURCE_PATH)/../cfg/atmos_better_tap.cfg $(SOURCE_PATH)/atmos/atmos_redefined_characters.c $(SOURCE_PATH)/atmos/atmos_input.c  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_oric1_48k.tap


	#  $(SOURCE_PATH)/c264/c264_sounds.c 
	# -DSOUNDS 


# c16_16k_no_udg:
	# $(CC65_PATH)$(MYCC65) -O -t c16 -DSOUNDS --config $(SOURCE_PATH)/../cfg/c16-16k.cfg $(SOURCE_PATH)/c264/c264_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_c16_16k_no_udg.prg


c16_16k_no_udg:
	$(CC65_PATH)$(MYCC65) -O -t c16 -Cl -DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DSOUNDS --config $(SOURCE_PATH)/../cfg/c16-16k_plus.cfg $(SOURCE_PATH)/c264/c264_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_c16_16k_no_udg.prg


# It works more or less fine (as 16k version) BUT it may not exist as a real model or real expansion
atmos_16k:
	$(CC65_PATH)$(MYCC65)  -O -D__ORIC1__ -DSOUNDS -DREDEFINED_CHARS \
	-t atmos --config $(SOURCE_PATH)/../cfg/atmos_better_tap.cfg \
	$(SOURCE_PATH)/atmos/atmos_redefined_characters.c \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/atmos/atmos_input.c  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  \
	-o $(BUILD_PATH)/LIGHT_atmos_16k.tap

zx80_8k:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 -O3 -vn -D__ZX80__ -DROUND_ENEMIES -DTINY_GAME -lndos -create-app -o  $(BUILD_PATH)/TINY_zx80_8k.prg $(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_zx80_8k.prg
	
zx81_8k:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -O3 -v -D__ZX81__ -DROUND_ENEMIES -DTINY_GAME -lndos -create-app -o  $(BUILD_PATH)/TINY_zx81_8k.prg $(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_zx81_8k.prg

lambda_8k:
	$(Z88DK_PATH)$(MYZ88DK) +lambda -O3 -vn -D__LAMBDA__ -DTINY_GAME -DNO_SET_SCREEN_COLOR -DLESS_TEXT -DNO_SLEEP -lndos -create-app -o  $(BUILD_PATH)/TINY_lambda_8k.prg $(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_lambda_8k.prg	
	
# -----------------------------------------------------------------------------------------------
	

atari5200_light:
	$(CC65_PATH)$(MYCC65) -O -t atari5200 $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_atari5200.rom	
	

vg5k_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k -O3 -vn -DTINY_GAME -D__VG5K__ -lndos -create-app -o $(BUILD_PATH)/TINY_vg5k.prg $(SOURCE_PATH)/vg5k/vg5k_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_vg5k.prg


spectrum_clib_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +zx -O3 -clib=ansi -pragma-define:ansicolumns=32 -vn                           -DCLIB_ANSI -DNO_SLEEP -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL -DLESS_TEXT -DTINY_GAME -D__SPECTRUM__ -lndos -create-app -o $(BUILD_PATH)/TINY_spectrum_clib.prg  $(SOURCE_PATH)/spectrum/spectrum_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_spectrum_clib.prg
	rm $(BUILD_PATH)/TINY_spectrum_clib_BANK_7.bin	


# ISSUE with kbhit and getk: the game is turned-based
	


# gal_22k:
	# $(Z88DK_PATH)$(MYZ88DK) +gal \
	# -pragma-need=ansiterminal -vn \
	# -D__GAL__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	# -lndos -create-app -o  $(BUILD_PATH)/FULL_galaksija.prg \
	# $(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	# $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	# $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	# $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	# rm $(BUILD_PATH)/FULL_galaksija.prg	
	# rm $(BUILD_PATH)/FULL_galaksija.wav	
	
	
conio_nascom:
	$(Z88DK_PATH)$(MYZ88DK) +nascom experiments/coniotest.c -create-app -o  $(BUILD_PATH)/conio.nas
			
conio:
	$(CC65_PATH)$(MYCC65) -O -t gamate experiments/conio.c -o  $(BUILD_PATH)/conio.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/conio.bin

joy-test:
	$(CC65_PATH)$(MYCC65) -O -t gamate experiments/joy-test.c -o  $(BUILD_PATH)/joy-test.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/joy-test.bin

# -DLESS_TEXT -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL
pce_tiny:
	$(CC65_PATH)$(MYCC65) -O -Cl -t pce -DTINY_GAME -DNO_SLEEP -DNO_RANDOM_LEVEL  --config $(SOURCE_PATH)/../cfg/pce_extra.cfg -DTINY_GAME $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_PCE.pce


		
gamate_tiny:
	$(CC65_PATH)$(MYCC65) -O -t gamate -DTINY_GAME $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/gamate/gamate_graphics.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_gamate.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/TINY_gamate.bin
	
gamate_light:
	$(CC65_PATH)$(MYCC65) -O -t gamate $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/gamate/gamate_graphics.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_gamate.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/LIGHT_gamate.bin

	# TODO: Reduce size in order to compile	
	
# No Image displayed	
atari_lynx:
	$(CC65_PATH)$(MYCC65) -O -t lynx -D__ATARI_LYNX__ $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/atari_lynx/atari_lynx_graphics.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_ATARI_LYNX.lnx

# -subtype=gaming
mc1000_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 -DDEBUG -DTINY_GAME -O3 -pragma-define:ansicolumns=32  -clib=ansi -D__MC1000__ -vn -DCLIB_ANSI -lndos -create-app -Cz--audio $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv a.wav $(BUILD_PATH)/TINY_mc1000.wav
	rm a.bin
	rm a.cas	

# -DFULL_GAME
vic20_exp_3k_NO_GFX: 
	$(CC65_PATH)$(MYCC65) -O -Cl -t vic20 -DNO_SLEEP -DLESS_TEXT -DNO_SET_SCREEN_COLORS   -DTINY_GAME --config $(SOURCE_PATH)/../cfg/vic20-3k.cfg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_vic20_exp_3k.prg
	
	
# vic20_unexpanded_merged:
	# $(CC65_PATH)$(MYCC65) -O -Cl -t vic20 \
	# -DALT_PRINT -DNO_SLEEP -DLESS_TEXT -DNO_SET_SCREEN_COLORS \
	# -DTINY_GAME -DVIC20_UNEXPANDED -DNO_RANDOM_LEVEL -DNO_INITIAL_SCREEN \
	# --config $(SOURCE_PATH)/../cfg/vic20_unexpanded.cfg  \
	# $(SOURCE_PATH)/merged.c \
	# -o $(BUILD_PATH)/TINY_vic20_unexpanded_merged.prg
	
# -DLESS_TEXT
creativision_light:
	$(CC65_PATH)$(MYCC65) -O -t creativision -Cl \
	-DNO_SLEEP -DLESS_TEXT -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL -DNO_MESSAGE \
	--config $(SOURCE_PATH)/../cfg/creativision-8k.cfg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/LIGHT_creativision.bin	

# NO Image displayed
creativision_full:
	$(CC65_PATH)$(MYCC65) -O -t creativision --config $(SOURCE_PATH)/../cfg/creativision-16k.cfg -DFULL_GAME $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_creativision.bin	

# It lacks conio and TGI
# --config $(SOURCE_PATH)/../cfg/supervision-16k.cfg
supervision_full:
	$(CC65_PATH)$(MYCC65) -O -t supervision  \
	-DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  \
	-o $(BUILD_PATH)/FULL_supervision.bin	
		

pce_light:
	$(CC65_PATH)$(MYCC65) -O -t pce $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_pce.pce

	
nes_color:
	$(CC65_PATH)ca65 $(SOURCE_PATH)/nes/reset.s
	$(CC65_PATH)cc65 -D__NES__ -DNES_COLOR $(SOURCE_PATH)/display_macros.c
	$(CC65_PATH)ca65 $(SOURCE_PATH)/display_macros.s
	$(CC65_PATH)cc65 $(SOURCE_PATH)/enemy.c 
	$(CC65_PATH)ca65 $(SOURCE_PATH)/enemy.s
	$(CC65_PATH)cc65 $(SOURCE_PATH)/level.c 
	$(CC65_PATH)ca65 $(SOURCE_PATH)/level.s
	$(CC65_PATH)cc65 $(SOURCE_PATH)/character.c 
	$(CC65_PATH)ca65 $(SOURCE_PATH)/character.s	
	$(CC65_PATH)cc65 -D__NES__ $(SOURCE_PATH)/text.c 
	$(CC65_PATH)ca65 $(SOURCE_PATH)/text.s 	
	$(CC65_PATH)cc65 $(SOURCE_PATH)/strategy.c 
	$(CC65_PATH)ca65 $(SOURCE_PATH)/strategy.s	
	$(CC65_PATH)cc65 -D__NES__ $(SOURCE_PATH)/input_macros.c
	$(CC65_PATH)ca65 $(SOURCE_PATH)/input_macros.s	
	$(CC65_PATH)cc65 -D__NES__ -DTINY_GAME $(SOURCE_PATH)/main.c 
	$(CC65_PATH)ca65 $(SOURCE_PATH)/main.s		
	$(CC65_PATH)cc65 $(SOURCE_PATH)/nes/nes_graphics.c
	$(CC65_PATH)ca65 $(SOURCE_PATH)/nes/nes_graphics.s		
	$(CC65_PATH)ld65 -t nes -o $(BUILD_PATH)/TINY_nes_color.nes $(SOURCE_PATH)/nes/reset.o $(SOURCE_PATH)/display_macros.o $(SOURCE_PATH)/nes/nes_graphics.o $(SOURCE_PATH)/enemy.o $(SOURCE_PATH)/level.o $(SOURCE_PATH)/character.o $(SOURCE_PATH)/text.o $(SOURCE_PATH)/strategy.o $(SOURCE_PATH)/input_macros.o nes.lib
	#$(CC65_PATH)$(MYCC65) -O -t nes -DTINY_GAME --config $(SOURCE_PATH)/nes/nes.cfg -DNES_COLOR $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/nes/reset.s $(SOURCE_PATH)/nes/nes_graphics.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_nes_color.nes
	# -C $(SOURCE_PATH)/nes/nes.cfg
	
nes_16k:
	$(CC65_PATH)$(MYCC65) -O -t nes $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_nes.nes

osca:
	$(Z88DK_PATH)$(MYZ88DK) +osca -O3 -clib=ansi -D__OSCA__ -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI -lndos -create-app -o $(BUILD_PATH)/FULL_osca.exe  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	


#All of these may work
ti86s:
	$(Z88DK_PATH)$(MYZ88DK) +ti86s -O3 -D__TI86S__ -clib=ansi -pragma-define:ansicolumns=32 -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI  -lndos -create-app -o $(BUILD_PATH)/FULL_ti86_mz.prg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c


ti86:
	$(Z88DK_PATH)$(MYZ88DK) +ti86 -O3 -D__TI86__ -clib=ansi -pragma-define:ansicolumns=32 -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI  -lndos -create-app -o $(BUILD_PATH)/FULL_ti86_mz.prg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c

ti85:
	$(Z88DK_PATH)$(MYZ88DK) +ti85 -O3 -D__TI85__ -clib=ansi -pragma-define:ansicolumns=32 -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI  -lndos -create-app -o $(BUILD_PATH)/FULL_ti85_mz.prg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c

ti84:
	$(Z88DK_PATH)$(MYZ88DK) +ti84 -O3 -D__TI84__ -clib=ansi -pragma-define:ansicolumns=32 -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI  -lndos -create-app -o $(BUILD_PATH)/FULL_ti84_mz.prg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c

ti83:
	$(Z88DK_PATH)$(MYZ88DK) +ti83 -O3 -D__TI83__ -clib=ansi -pragma-define:ansicolumns=32 -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI  -lndos -create-app -o $(BUILD_PATH)/FULL_ti83_mz.prg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c

ti82:
	$(Z88DK_PATH)$(MYZ88DK) +ti82 -O3 -D__TI82__ -clib=ansi -pragma-define:ansicolumns=32 -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI  -lndos -create-app -o $(BUILD_PATH)/FULL_ti82_mz.prg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c

ti8x:
	$(Z88DK_PATH)$(MYZ88DK) +ti8x -O3 -D__TI8X__ -clib=ansi -pragma-define:ansicolumns=32 -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI  -lndos -create-app -o $(BUILD_PATH)/FULL_ti8x_mz.prg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	
	
# missing conio
srr:
	$(Z88DK_PATH)$(MYZ88DK) +srr -O3  -D__SRR__ -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI -lndos -create-app -o $(BUILD_PATH)/FULL_srr.prg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c


# it may work
mtx_16k:
	$(Z88DK_PATH)$(MYZ88DK) +mtx -startup=2 -O3 -D__MTX__ -clib=ansi -pragma-define:ansicolumns=32 -vn  -DCLIB_ANSI -lndos -create-app -o LIGHT.bin $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv LIGHT $(BUILD_PATH)
	mv LIGHT.bin $(BUILD_PATH)
	mv LIGHT.wav $(BUILD_PATH)
	
	
lambda_light:
	$(Z88DK_PATH)$(MYZ88DK) +lambda -vn -D__LAMBDA__ -lndos -create-app -o  $(BUILD_PATH)/LIGHT_lambda.prg $(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_lambda.prg	
	
msx_no_color_16k:
	$(Z88DK_PATH)$(MYZ88DK) +msx -O3  -zorg=49200 -DSOUNDS -create-app -vn -D__MSX__ -lndos -create-app -o $(BUILD_PATH)/LIGHT_msx_no_color_16k.prg $(SOURCE_PATH)/msx/msx_graphics.c $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_msx_no_color_16k.prg 	
		
cpc_color_light:
	$(Z88DK_PATH)$(MYZ88DKASM) -v   -x$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lib   @$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lst
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 -vn -clib=ansi -DREDEFINED_CHARS -D__CPC__ -DCPCRSLIB          -l$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib -lndos -create-app -o $(BUILD_PATH)/LIGHT_cpc_color.prg                                 $(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/LIGHT_cpc_color.cpc  $(BUILD_PATH)/LIGHT_cpc_color.cdt
	rm $(BUILD_PATH)/LIGHT_cpc_color.cpc 
	rm $(BUILD_PATH)/LIGHT_cpc_color.prg

cpc_color_sound_light:
	$(Z88DK_PATH)$(MYZ88DKASM) -v   -x$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lib   @$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lst
	$(Z88DK_PATH)$(MYZ88DK)   +cpc -O3 -vn -clib=ansi -DREDEFINED_CHARS -D__CPC__ -DCPCRSLIB -DSOUNDS -l$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib -lndos -create-app -o $(BUILD_PATH)/LIGHT_cpc_color_sound.prg $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/LIGHT_cpc_color_sound.cpc  $(BUILD_PATH)/LIGHT_cpc_color_sound.cdt
	rm $(BUILD_PATH)/LIGHT_cpc_color_sound.cpc 
	rm $(BUILD_PATH)/LIGHT_cpc_color_sound.prg	

cpc_color_no_sound:
	$(Z88DK_PATH)$(MYZ88DKASM) -v   -x$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lib   @$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lst
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 -DREDEFINED_CHARS           -DFULL_GAME -vn -clib=ansi -D__CPC__   -DCPCRSLIB  -l$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib -lndos -create-app -o $(BUILD_PATH)/FULL_cpc_color_no_sound.prg                                 $(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_color_no_sound.cpc  $(BUILD_PATH)/FULL_cpc_color_no_sound.cdt
	rm $(BUILD_PATH)/FULL_cpc_color_no_sound.cpc 
	rm $(BUILD_PATH)/FULL_cpc_color_no_sound.prg	
	
# VERY SLOW DUE TO CONIO
cpc_color_no_udg:
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 -DREDEFINED_CHARS -vn -DFULL_GAME -clib=ansi -D__CPC__ -lndos -create-app -o $(BUILD_PATH)/FULL_cpc_color_no_udg.prg $(SOURCE_PATH)/cpc/cpc_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_color_no_udg.cpc  $(BUILD_PATH)/FULL_cpc_color_no_udg.cdt
	rm $(BUILD_PATH)/FULL_cpc_color_no_udg.cpc 
	rm $(BUILD_PATH)/FULL_cpc_color_no_udg.prg
	
cpc_no_color_no_udg:
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 -DREDEFINED_CHARS -DCPC_NO_COLOR -vn -DFULL_GAME -clib=ansi -D__CPC__ -lndos -create-app -o $(BUILD_PATH)/FULL_cpc_no_color.prg $(SOURCE_PATH)/cpc/cpc_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_no_color.cpc  $(BUILD_PATH)/FULL_cpc_no_color.cdt
	rm $(BUILD_PATH)/FULL_cpc_no_color.cpc 
	rm $(BUILD_PATH)/FULL_cpc_no_color.prg	

msx_color_32k_msxdos:
	$(Z88DK_PATH)$(MYZ88DK) +msx -O3 -DSOUNDS -DREDEFINED_CHARS -vn -DMSX_MODE1 -DFULL_GAME -D__MSX__ -lndos -subtype=msxdos -o $(BUILD_PATH)/FULL_msx_color_32k.com $(SOURCE_PATH)/msx/msx_graphics.c $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	