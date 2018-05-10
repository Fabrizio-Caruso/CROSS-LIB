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

# --config $(SOURCE_PATH)/../cfg/atari5200_less_stack.cfg
atari5200: 
	$(CC65_PATH)$(MYCC65) -O -t atari5200 \
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
	-DALT_PRINT \
	-DNO_SLEEP \
	-DLESS_TEXT \
	-DNO_SET_SCREEN_COLORS \
	-DTINY_GAME \
	-DNO_RANDOM_LEVEL \
	-DFORCE_KEYBOARD \
	-DNO_MESSAGE \
	-DNO_STATS \
	-DNO_INITIAL_SCREEN \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DALT_DISPLAY_STATS \
	-DNO_DEAD_GHOSTS \
	-DALT_HIGHSCORE \
	-DFORCE_BOMBS_NUMBER=2 \
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

	
#	-DSOUNDS 
#	$(SOURCE_PATH)/vic20/vic20_sounds.c 
	
vic20_exp_8k_full:
	$(CC65_PATH)$(MYCC65) -O -Cl -t vic20 --config $(SOURCE_PATH)/../cfg/vic20-8k.cfg \
	-DFULL_GAME \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DSIMPLE_STRATEGY \
	-DNO_BLINKING \
	-DLESS_TEXT	\
	-DALT_PRINT \
	-DNO_SET_SCREEN_COLOR \
	-DNO_HINTS \
	-DNO_DEAD_GHOSTS \
	-DSOUNDS \
	-DALT_SLEEP \
	$(SOURCE_PATH)/sleep_macros.c \
	$(SOURCE_PATH)/vic20/vic20_sounds.c \
	$(SOURCE_PATH)/vic20/vic20_alt_print.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/main.c  \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/missile.c \
	$(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/rocket.c \
	-o $(BUILD_PATH)/FULL_vic20_exp_8k_NO_GFX.prg		

vic20_exp_8k_full_no_sound:
	$(CC65_PATH)$(MYCC65) -O -Cl -t vic20 -DREDEFINED_CHARS --config $(SOURCE_PATH)/../cfg/vic20-8k_gfx_tight.cfg \
	-DFULL_GAME \
	-DSIMPLE_STRATEGY \
	-DNO_INITIAL_SCREEN \
	-DNO_SLEEP \
	-DFORCE_KEYBOARD \
	-DNO_RANDOM_LEVEL \
	-DFORCE_GHOSTS_NUMBER=4 \
	-DNO_BLINKING \
	-DLESS_TEXT	\
	-DNO_MESSAGE \
	-DALT_PRINT \
	$(SOURCE_PATH)/vic20/udc_3k.s \
	$(SOURCE_PATH)/vic20/vic20_alt_print.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/main.c  \
	$(SOURCE_PATH)/enemy.c \
	--code-name CODE2 \
	$(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/missile.c \
	$(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/rocket.c \
	-o $(BUILD_PATH)/FULL_vic20_exp_8k_NO_SOUND.prg	
	
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
	python $(TOOLS_PATH)/prg2crt.py $(BUILD_PATH)/FULL_c64_exomized.prg  $(BUILD_PATH)/FULL_c64_exomized.crt
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
	-DFORCE_GHOSTS_NUMBER=9 \
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
	$(SOURCE_PATH)/c264/c264_sounds.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/c264/c264_graphics.c  \
	$(SOURCE_PATH)/item.c $(SOURCE_PATH)/enemy.c \
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

	
# 	--config $(SOURCE_PATH)/../cfg/c16_16k_less_stack.cfg 


c16_16k_full: 
	$(CC65_PATH)$(MYCC65) -O -Cl -t c16 \
	--config $(SOURCE_PATH)/../cfg/c16_16k_less_stack.cfg \
	-DFULL_GAME \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DNO_BLINKING \
	-DLESS_TEXT	\
	-DNO_HINTS \
	-DSOUNDS \
	-DALT_PRINT \
	$(SOURCE_PATH)/c264/c264_sounds.c \
	$(SOURCE_PATH)/c264/c264_alt_print.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/main.c  \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/missile.c \
	$(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/rocket.c \
	-o $(BUILD_PATH)/FULL_c16_16k_NO_GFX.prg	

	
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
	java -jar $(SOURCE_PATH)/../tools/ac.jar -as $(BUILD_PATH)/FULL_apple2.dsk aschase < $(BUILD_PATH)/apple2.bin
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
	java -jar $(SOURCE_PATH)/../tools/ac.jar -as $(BUILD_PATH)/FULL_apple2enh.dsk aschase < $(BUILD_PATH)/apple2enh.bin
	rm $(BUILD_PATH)/apple2enh.bin

osic1p_32k: 
	$(CC65_PATH)$(MYCC65) --start-addr 0x200 -Wl -D,__HIMEM__=0x8000 -O -t osic1p -DFULL_GAME \
	-DTURN_BASED \
	-DFORCE_GHOSTS_NUMBER=9 \
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


osic1p_8k: 
	$(CC65_PATH)$(MYCC65) -Cl --start-addr 0x200 -Wl -D,__HIMEM__=0x2000 -O --config $(SOURCE_PATH)/../cfg/osic1p_less_stack.cfg -t osic1p \
	-DROUND_ENEMIES -DNO_SLEEP  -DNO_RANDOM_LEVEL -DLESS_TEXT -DNO_SET_SCREEN_COLOR -DTINY_GAME \
	-DTURN_BASED \
	-DFORCE_GHOSTS_NUMBER=9 \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_osic1p_8k.lod
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
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	--config $(SOURCE_PATH)/../cfg/creativision-8k.cfg \
	-DTINY_GAME \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/TINY_creativision_8k.bin

creativision_16k:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	-DNO_SLEEP -DLESS_TEXT -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(SOURCE_PATH)/../cfg/creativision-16k.cfg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	-o $(BUILD_PATH)/FULL_creativision_16k.bin	
	dd if=$(BUILD_PATH)/FULL_creativision_16k.bin ibs=1 count=8192 of=$(BUILD_PATH)/FULL_creativision_16k_LOW.bin
	dd if=$(BUILD_PATH)/FULL_creativision_16k.bin ibs=1 skip=8192 of=$(BUILD_PATH)/FULL_creativision_16k_HIGH.bin
	rm $(BUILD_PATH)/FULL_creativision_16k.bin 
	cat $(BUILD_PATH)/FULL_creativision_16k_HIGH.bin $(BUILD_PATH)/FULL_creativision_16k_LOW.bin > $(BUILD_PATH)/FULL_creativision_16k_SWAPPED.bin
	rm $(BUILD_PATH)/FULL_creativision_16k_LOW.bin
	rm $(BUILD_PATH)/FULL_creativision_16k_HIGH.bin	
	
	
atari_lynx:
	$(CC65_PATH)$(MYCC65) -O -t lynx \
	-D__ATARI_LYNX__ \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/atari_lynx/atari_lynx_graphics.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/LIGHT_ATARI_LYNX.lnx
	
	
# -DLESS_TEXT -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL
pce_8k:
	$(CC65_PATH)$(MYCC65) -O -Cl -t pce \
	-DTINY_GAME -DNO_SLEEP -DLESS_TEXT \
	--config $(SOURCE_PATH)/../cfg/pce_8k_less_stack.cfg \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/TINY_PCE_8k.pce


pce_16k:
	$(CC65_PATH)$(MYCC65) -O -t pce \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	--config $(SOURCE_PATH)/../cfg/pce_16k_less_stack.cfg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c \
	$(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_PCE.bin
	dd if=$(BUILD_PATH)/FULL_PCE.bin ibs=1 count=8192 of=$(BUILD_PATH)/FULL_PCE_LOW.bin
	dd if=$(BUILD_PATH)/FULL_PCE.bin ibs=1 skip=8192 of=$(BUILD_PATH)/FULL_PCE_HIGH.bin
	rm $(BUILD_PATH)/FULL_PCE.bin 
	cat $(BUILD_PATH)/FULL_PCE_HIGH.bin $(BUILD_PATH)/FULL_PCE_LOW.bin > $(BUILD_PATH)/FULL_PCE_SWAPPED.pce
	rm $(BUILD_PATH)/FULL_PCE_LOW.bin
	rm $(BUILD_PATH)/FULL_PCE_HIGH.bin	


	
# ------------------------------------------------------------------------------------------
#Z88DK
# 	-DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL \
# 	-SO3 --max-allocs-per-node200000 \

aquarius_exp_4k:
	$(Z88DK_PATH)$(MYZ88DK) +aquarius -compiler=sdcc \
	-vn \
	-DALT_PRINT -D__AQUARIUS__ -DTINY_GAME -DNO_SLEEP -DLESS_TEXT \
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
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
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
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_vz200_16k.cas
	
vz200_32k: 
	$(Z88DK_PATH)$(MYZ88DK) +vz -O3 -vn \
	-DSOUNDS -D__VZ__ -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-clib=ansi -lndos -create-app -o  $(BUILD_PATH)/FULL_vz200_32k.vz \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_vz200_32k.cas
	
# TODO: Adapt code to work with -compiler=sdcc
# -SO3 --max-allocs-per-node200000
# -O3 -zorg=18941 -vn 
#  -DNO_RANDOM_LEVEL
# -DLESS_TEXT -DNO_SLEEP
vg5k: 
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-O3 \
	-vn \
	-D__VG5K__ -DSOUNDS  \
	-DASM_DISPLAY \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_vg5k.prg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/vg5k/vg5k_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	# rm $(BUILD_PATH)/LIGHT_vg5k.k7
	# cat $(SOURCE_PATH)/vg5k/LIGHT_vg5k_header.hex $(BUILD_PATH)/LIGHT_vg5k.prg $(SOURCE_PATH)/vg5k/LIGHT_vg5k_end.hex > $(BUILD_PATH)/LIGHT_vg5k.k7
	rm $(BUILD_PATH)/LIGHT_vg5k.prg

# -compiler=sdcc 
# -SO3 --max-allocs-per-node200000 
# -opt-code-size
# -DSDCC
# -DTURN_BASED
# -DASM_DISPLAY
# 	-pragma-include:$(SOURCE_PATH)/../cfg/zpragma_clib.inc 
vg5k_full_sccz80:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-O3 \
	-DASM_DISPLAY \
	-pragma-include:$(SOURCE_PATH)/../cfg/zpragma_clib.inc \
	-DNO_BLINKING \
	-vn -DFULL_GAME -D__VG5K__ \
	-DLESS_TEXT \
	-DSIMPLE_STRATEGY \
	-DNO_HINTS \
	-DNO_RANDOM_LEVEL \
	-DNO_DEAD_GHOSTS \
	-DNO_SET_SCREEN_COLOR \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DFORCE_BOMBS_NUMBER=4 \
	-DNO_BORDERS \
	-DNO_MESSAGE \
	-create-app -o $(BUILD_PATH)/FULL_vg5k_full_sccz80.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/vg5k/vg5k_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	# rm $(BUILD_PATH)/FULL_vg5k_exp_16k.k7
	# cat $(SOURCE_PATH)/vg5k/FULL_vg5k_header.hex $(BUILD_PATH)/FULL_vg5k_exp_16k.prg $(SOURCE_PATH)/vg5k/LIGHT_vg5k_end.hex > $(BUILD_PATH)/FULL_vg5k_exp_16k.k7
	rm $(BUILD_PATH)/FULL_vg5k_full_sccz80.prg	


# 	-SO3 --max-allocs-per-node200000 \
#   -opt-code-size \
#
vg5k_full_sdcc:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-compiler=sdcc \
	-SO3 --max-allocs-per-node200000 \
	-opt-code-size \
	--reserve-regs-iy \
	-pragma-include:$(SOURCE_PATH)/../cfg/zpragma_clib.inc \
	-DNO_BLINKING \
	-vn -DFULL_GAME -D__VG5K__ \
	-DLESS_TEXT \
	-DSIMPLE_STRATEGY \
	-DNO_HINTS \
	-DNO_RANDOM_LEVEL \
	-DNO_DEAD_GHOSTS \
	-DNO_SET_SCREEN_COLOR \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DFORCE_BOMBS_NUMBER=4 \
	-DNO_MESSAGE \
	-DNO_INITIAL_SCREEN \
	-DNO_SET_SCREEN_COLOR \
	-DNO_SLEEP \
	-create-app -o $(BUILD_PATH)/FULL_vg5k_full_sdcc.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/vg5k/vg5k_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	# rm $(BUILD_PATH)/FULL_vg5k_exp_16k.k7
	# cat $(SOURCE_PATH)/vg5k/FULL_vg5k_header.hex $(BUILD_PATH)/FULL_vg5k_exp_16k.prg $(SOURCE_PATH)/vg5k/LIGHT_vg5k_end.hex > $(BUILD_PATH)/FULL_vg5k_exp_16k.k7
	rm $(BUILD_PATH)/FULL_vg5k_full_sdcc.prg	
	
# -O3 -zorg=18941 -vn
vg5k_exp_16k:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-O3 \
	-DSOUNDS -vn -DFULL_GAME -D__VG5K__ -DBETWEEN_LEVEL -DEND_SCREEN -DASM_DISPLAY \
	-lndos -create-app -o $(BUILD_PATH)/FULL_vg5k_exp_16k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/vg5k/vg5k_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	# rm $(BUILD_PATH)/FULL_vg5k_exp_16k.k7
	# cat $(SOURCE_PATH)/vg5k/FULL_vg5k_header.hex $(BUILD_PATH)/FULL_vg5k_exp_16k.prg $(SOURCE_PATH)/vg5k/LIGHT_vg5k_end.hex > $(BUILD_PATH)/FULL_vg5k_exp_16k.k7
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


zx80_8k:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 -O3 -vn \
	-D__ZX80__ -DROUND_ENEMIES -DTINY_GAME \
	-DTURN_BASED \
	-lndos -create-app -o  $(BUILD_PATH)/TINY_zx80_8k.prg \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_zx80_8k.prg


zx81_8k:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-compiler=sdcc \
	-SO3 --max-allocs-per-node200000 \
	-vn \
	-D__ZX81__ -DTINY_GAME -DROUND_ENEMIES \
	-DALT_SLEEP \
	-DMACRO_SLEEP \
	-lndos \
	-create-app -o  $(BUILD_PATH)/TINY_zx81_8k.prg \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_zx81_8k.prg
	
	
zx80_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 -O3 -vn \
	-D__ZX80__ -DFULL_GAME \
	-DEND_SCREEN \
	-DTURN_BASED \
	-DALT_SLEEP \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx80_16k.prg \
	$(SOURCE_PATH)/sleep_macros.c \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_zx80_16k.prg


zx81_16k_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-compiler=sdcc \
	-SO3 --max-allocs-per-node200000 \
	-vn \
	-D__ZX81__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DALT_SLEEP \
	-DMACRO_SLEEP \
	-DTURN_BASED \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_16k_turn_based.prg \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_zx81_16k_turn_based.prg

	
# 	-SO3 --max-allocs-per-node200000 
	
zx81_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -SO3 --max-allocs-per-node200000 \
	-compiler=sdcc \
	-vn \
	-D__ZX81__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DALT_SLEEP \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_16k.prg \
	$(SOURCE_PATH)/sleep_macros.c \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
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
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 -DREDEFINED_CHARS -vn  -clib=ansi \
	-D__CPC__ -DSOUNDS -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCPCRSLIB \
	-pragma-define:REGISTER_SP=-1 \
	-lndos -create-app -o 	$(BUILD_PATH)/FULL_cpc.prg \
	$(TOOLS_PATH)/cpcrslib/cpc_Chars.asm \
	$(TOOLS_PATH)/cpcrslib/cpc_Chars8.asm \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c \
	$(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc.cpc  $(BUILD_PATH)/FULL_cpc.cdt
	rm -rf FULL_cpc.dsk
	$(TOOLS_PATH)/cpcxfsw -nd FULL_cpc.dsk
	$(TOOLS_PATH)/cpcxfsw FULL_cpc.dsk -p build/FULL_cpc.cpc xchase
	mv FULL_cpc.dsk $(BUILD_PATH)/
	rm $(BUILD_PATH)/FULL_cpc.cpc 
	rm $(BUILD_PATH)/FULL_cpc.prg			


cpc_joystick:
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 -DREDEFINED_CHARS -vn  -clib=ansi \
	-D__CPC__ \
	-D__CPC_PLUS__ \
	-DSOUNDS -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCPCRSLIB \
	-pragma-define:REGISTER_SP=-1 \
	-lndos -create-app -o 	$(BUILD_PATH)/FULL_cpc_joystick.prg \
	$(TOOLS_PATH)/cpcrslib/cpc_Chars.asm \
	$(TOOLS_PATH)/cpcrslib/cpc_Chars8.asm \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c \
	$(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_joystick.cpc  $(BUILD_PATH)/FULL_cpc_joystick.cdt
	rm -rf FULL_cpc_joystick.dsk
	$(TOOLS_PATH)/cpcxfsw -nd FULL_cpc_joystick.dsk
	$(TOOLS_PATH)/cpcxfsw FULL_cpc_joystick.dsk -p build/FULL_cpc_joystick.cpc xchase
	mv FULL_cpc_joystick.dsk $(BUILD_PATH)/
	cp $(TOOLS_PATH)/nocart/*.rom .
	$(TOOLS_PATH)/nocart/nocart.exe  $(BUILD_PATH)/FULL_cpc_joystick.dsk  $(BUILD_PATH)/FULL_gx4000.cpr   -c 'run"xchase'
	rm os.rom
	rm amsdos.rom
	rm basic.rom
	rm $(BUILD_PATH)/FULL_cpc_joystick.cpc 
	rm $(BUILD_PATH)/FULL_cpc_joystick.prg		


cpc_no_udg:
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 -DREDEFINED_CHARS -vn  -clib=ansi \
	-D__CPC__ -DSOUNDS -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-pragma-define:REGISTER_SP=-1 \
	-lndos -create-app -o $(BUILD_PATH)/FULL_cpc_no_udg.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c \
	$(SOURCE_PATH)/cpc/cpc_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_no_udg.cpc  $(BUILD_PATH)/FULL_cpc_no_udg.cdt
	rm $(BUILD_PATH)/FULL_cpc_no_udg.cpc 
	rm $(BUILD_PATH)/FULL_cpc_no_udg.prg	
		
		
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
	-DSOUNDS \
	-create-app -o  $(BUILD_PATH)/LIGHT_svi_318_mode0 \
	$(SOURCE_PATH)/psg/psg_sounds.c \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/svi/svi_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_SVI_318_mode0


#
svi_318:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	-SO3 --max-allocs-per-node200000 \
	-compiler=sdcc \
	-zorg=49152 \
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-pragma-include:$(SOURCE_PATH)/../cfg/zpragma_clib.inc \
	-vn -lndos \
	-D__SVI__ \
	-DSOUNDS \
	-DALT_SLEEP \
	-DMACRO_SLEEP \
	-create-app -o $(BUILD_PATH)/LIGHT_svi_318 \
	$(SOURCE_PATH)/psg/psg_sounds.c	\
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	#rm $(BUILD_PATH)/LIGHT_svi_318	
	
# 	-pragma-need=ansiterminal \

sc3000_16k:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	-O3 \
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/LIGHT_sc3000_16k.prg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_sc3000_16k.prg
	rm $(BUILD_PATH)/LIGHT_sc3000_16k.tap
	
sc3000_32k:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	-O3 \
	-DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/FULL_sc3000_32k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_sc3000_32k.prg
	rm $(BUILD_PATH)/FULL_sc3000_32k.tap	
	
# It hangs if compiled with sdcc.
# syntax error if compiled with sccz80
# $(SOURCE_PATH)/svi/svi_graphics.c	
svi_328:
	$(Z88DK_PATH)$(MYZ88DK) +svi -O3 \
	-clib=ansi -pragma-define:ansicolumns=32 -vn -lndos \
	-DSOUNDS \
	-DFULL_GAME -D__SVI__ -DBETWEEN_LEVEL -DEND_SCREEN \
	-create-app -o $(BUILD_PATH)/FULL_svi_328 \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_svi_328

sharp_mz:
	$(Z88DK_PATH)$(MYZ88DK) +mz -O3 \
	-D__MZ__ -clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DFULL_GAME -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o $(BUILD_PATH)/FULL_sharp_mz.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_sharp_mz.prg
	mv $(BUILD_PATH)/FULL_sharp_mz.mzt $(BUILD_PATH)/FULL_sharp_mz.mzf
	
microbee_16k:
	$(Z88DK_PATH)$(MYZ88DK) +bee -O3 \
	-D__BEE__ -clib=ansi -vn -DSOUNDS -DCLIB_ANSI \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_microbee_16k.prg  \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_microbee_16k.prg
	
microbee_32k:
	$(Z88DK_PATH)$(MYZ88DK) +bee -O3 \
	-D__BEE__ -clib=ansi -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o $(BUILD_PATH)/FULL_microbee_32k.prg  \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_microbee_32k.prg

# import as data into ram at 32768 - call 32768
samcoupe:
	$(Z88DK_PATH)$(MYZ88DK) +sam -O3 \
	-D__SAM__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DFULL_GAME -DCLIB_ANSI -o $(BUILD_PATH)/FULL_samcoupe.bin -lndos \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	cp $(TOOLS_PATH)/samdos2_empty $(TOOLS_PATH)/samdos2
	$(TOOLS_PATH)/pyz80.py -I $(TOOLS_PATH)/samdos2 $(TOOLS_PATH)/sam_wrapper.asm
	mv $(TOOLS_PATH)/sam_wrapper.dsk $(BUILD_PATH)/FULL_samcoupe.dsk
	rm $(BUILD_PATH)/FULL_samcoupe.bin

	

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
	mv a.ihx $(BUILD_PATH)/LIGHT_abc80.ihx 
	
abc80_32k:
	$(Z88DK_PATH)$(MYZ88DK) +abc80 -lm -subtype=hex -zorg=49200 -O3 \
	-D__ABC80__ -clib=ansi -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o a \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm a
	mv a.ihx $(BUILD_PATH)/FULL_abc80.ihx 

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

z9001_32k:
	$(Z88DK_PATH)$(MYZ88DK) +z9001 -O3 -clib=ansi \
	-D__Z9001__ -vn -DFULL_GAME -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o $(BUILD_PATH)/FULL_z9001.z80 \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	rm $(BUILD_PATH)/FULL_z9001.z80	

	
z9001_16k:
	$(Z88DK_PATH)$(MYZ88DK) +z9001 -O3 -clib=ansi \
	-D__Z9001__ -vn  -DCLIB_ANSI \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_z9001.z80 \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	rm $(BUILD_PATH)/LIGHT_z9001.z80	
	
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


# 	-SO3 --max-allocs-per-node200000	
mc1000_16k_full:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 -compiler=sdcc -SO3 --max-allocs-per-node200000 \
	-subtype=gaming -pragma-define:ansicolumns=32 \
	-DFULL_GAME  \
	-DLESS_TEXT \
	-DNO_BLINKING \
	-DNO_HINTS \
	-clib=ansi \
	-D__MC1000__ -DSOUNDS \
	-DEND_SCREEN \
	-DALT_SLEEP \
	-vn -DCLIB_ANSI -lndos -create-app -Cz--audio \
	$(SOURCE_PATH)/sleep_macros.c \
	$(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv a.wav $(BUILD_PATH)/FULL_mc1000_16k_less_text.wav
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
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/spectrum/udg.asm $(SOURCE_PATH)/spectrum/spectrum_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_spectrum_48k.prg
	rm $(BUILD_PATH)/FULL_spectrum_48k_BANK_7.bin	

spectrum_48k_putc4x6:
	$(Z88DK_PATH)$(MYZ88DK) +zx -O3 -clib=ansi -vn  \
	-pragma-redirect:ansifont=_udg -pragma-define:ansifont_is_packed=0 -pragma-define:ansicolumns=32 \
	-DFULL_GAME -DREDEFINED_CHARS -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL -D__SPECTRUM__ \
	-DZ88DK_PUTC4X6 \
	-DNO_SLEEP -DLESS_TEXT -DNO_WAIT \
	-lndos -create-app \
	-o $(BUILD_PATH)/FULL_spectrum_48k_putc4x6.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/spectrum/udg.asm $(SOURCE_PATH)/spectrum/spectrum_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_spectrum_48k_putc4x6.prg
	rm $(BUILD_PATH)/FULL_spectrum_48k_putc4x6_BANK_7.bin	
	
pc6001_16k:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 -O3 -Cz--audio -clib=ansi -subtype=32k \
	-D__PC6001__ -vn -DCLIB_ANSI \
	-DALT_SLEEP \
	-DMACRO_SLEEP \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_pc6001.prg  \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_pc6001.prg
	rm $(BUILD_PATH)/LIGHT_pc6001.wav
	mv $(BUILD_PATH)/LIGHT_pc6001.cas $(BUILD_PATH)/LIGHT_pc6001.cp6	

	
# Warning at file 'c:/z88dk/\lib\pc6001_crt0.asm' line 112: integer '66384' out of range
# Warning at file 'stdio/ansi/pc6001/f_ansi_char.asm' line 46: integer '66657' out of range
pc6001_32k:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 -O3 -Cz--audio -clib=ansi -subtype=32k \
	-D__PC6001__ -vn -DFULL_GAME -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL \
	-DALT_SLEEP \
	-DMACRO_SLEEP \
	-lndos -create-app -o $(BUILD_PATH)/FULL_pc6001_32k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_pc6001_32k.prg
	rm $(BUILD_PATH)/FULL_pc6001_32k.wav
	mv $(BUILD_PATH)/FULL_pc6001_32k.cas $(BUILD_PATH)/FULL_pc6001_32k.cp6

	
# kbhit KO
# Everything displayed on the same line
nascom_32k:
	$(Z88DK_PATH)$(MYZ88DK) +nascom -O3 -clib=ansi -vn -lndos \
	-D__NASCOM__ -DCLIB_ANSI -D__NASCOM__ -DSOUNDS -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o $(BUILD_PATH)/FULL_nascom_32k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_nascom_32k.prg
	
	
# -DSOUNDS
# -pragma-define:ansicolumns=32  -Cz-audio 
nascom_16k:
	$(Z88DK_PATH)$(MYZ88DK) +nascom -O3 -clib=ansi -vn -lndos \
	-D__NASCOM__ -DCLIB_ANSI -D__NASCOM__ -DSOUNDS \
	-create-app -o $(BUILD_PATH)/LIGHT_nascom_16k.prg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	rm $(BUILD_PATH)/LIGHT_nascom_16k.prg
	


z1013:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 -O3 -clib=ansi \
	-vn -lndos \
	-D__Z1013__ -DCLIB_ANSI -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \
	-create-app -o
	mv $(BUILD_PATH)/../A.Z80 $(BUILD_PATH)/FULL_z1013.z80
	rm $(BUILD_PATH)/../a.bin	


	
px8_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px32k \
	-D__PX8__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-create-app -o$(BUILD_PATH)/TINY_px8.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_px8.bin


px8:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px32k \
	-D__PX8__ \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-create-app -o$(BUILD_PATH)/FULL_px8.bin \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_px8.bin



px4_putc4x6_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-pragma-define:ansicolumns=40 \
	-pragma-define:ansipixels=240 -pragma-define:ansicolumns=60 \
 	-subtype=px4ansi \
	-D__PX4__ \
	-DTINY_GAME \
	-DNO_WAIT \
	-DZ88DK_PUTC4X6 \
	-Cz–-32k \
	-create-app -o $(BUILD_PATH)/TINY_px4_putc4x6.bin \
	-vn -lndos \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_px4_putc4x6.bin

	
px4_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-D__PX4__ \
	-DTINY_GAME \
	-subtype=px4ansi -Cz–-32k \
	-pragma-define:ansicolumns=40 \
	-create-app -o $(BUILD_PATH)/TINY_px4.bin \
	-vn -lndos \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_px4.bin


px4:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-D__PX4__ \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-subtype=px4ansi -Cz–-32k \
	-pragma-define:ansicolumns=40 \
	-create-app -o $(BUILD_PATH)/FULL_px4.bin \
	-vn -lndos \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_px4.bin
	

px4_putc4x6:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-pragma-define:ansicolumns=40 \
	-pragma-define:ansipixels=240 -pragma-define:ansicolumns=60 \
 	-subtype=px4ansi \
	-D__PX4__ \
	-DFULL_GAME \
	-DNO_WAIT \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DZ88DK_PUTC4X6 \
	-Cz–-32k \
	-create-app -o $(BUILD_PATH)/FULL_px4_putc4x6.bin \
	-vn -lndos \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_px4_putc4x6.bin


x1_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +x1 \
	-D__X1__ \
	-DTINY_GAME -DNO_SLEEP \
	-create-app -o $(BUILD_PATH)/TINY_x1.bin -vn -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	rm $(BUILD_PATH)/TINY_x1.bin
	
	
x1:
	$(Z88DK_PATH)$(MYZ88DK) +x1 \
	-D__X1__ \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-create-app -o $(BUILD_PATH)/FULL_x1.bin -vn -lndos \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_x1.bin

# -subtype=disk
trs80_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	-lm -create-app \
	-D__TRS80__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-o$(BUILD_PATH)/TINY_trs80.bin \
	$(SOURCE_PATH)/trs80/trs80_input.c \
	$(SOURCE_PATH)/trs80/trs80_graphics.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c	
	rm $(BUILD_PATH)/TINY_trs80.bin
	

trs80:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	-lm -create-app \
	-D__TRS80__ \
	-DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-o$(BUILD_PATH)/FULL_trs80.bin \
	$(SOURCE_PATH)/trs80/trs80_input.c \
	$(SOURCE_PATH)/trs80/trs80_graphics.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \	
	rm $(BUILD_PATH)/FULL_trs80.bin	


kc_sprites_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap \
	-D__KC__ \
	-DZ88DK_SPRITES \
	-DTINY_GAME \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DREDEFINED_CHARS \
	-create-app -o$(BUILD_PATH)/TINY_kc_sprites.bin \
	$(SOURCE_PATH)/z88dk_sprites/z88dk_sprites_graphics.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_kc_sprites.bin	

nc100_sprites_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc100 \
	-D__NC100__ \
	-DZ88DK_SPRITES \
	-DTINY_GAME \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DREDEFINED_CHARS \
	-create-app -o$(BUILD_PATH)/TINY_nc100_sprites.bin \
	$(SOURCE_PATH)/z88dk_sprites/z88dk_sprites_graphics.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_nc100_sprites.bin


nc100_sprites_light:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc100 \
	-D__NC100__ \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DREDEFINED_CHARS \
	-create-app -o$(BUILD_PATH)/LIGHT_nc100_sprites.bin \
	$(SOURCE_PATH)/z88dk_sprites/z88dk_sprites_graphics.c \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_nc100_sprites.bin


cpm_adm3a_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-D__CPM_ADM3A__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-create-app -o$(BUILD_PATH)/TINY_cpm_adm3a.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_cpm_adm3a.bin

	
cpm_vt52_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-D__CPM_VT52__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-create-app -o$(BUILD_PATH)/TINY_cpm_vt52.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_cpm_vt52.bin	
	
cpm_vt100_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-D__CPM_VT100__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-create-app -o$(BUILD_PATH)/TINY_cpm_vt100.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_cpm_vt100.bin	
	

c128_z80_40col:
	$(Z88DK_PATH)$(MYZ88DK) +c128 -O3 -lndos \
	-D__CPM_VT100__ -D__C128_Z80__ -DFORCE_XSIZE=40 \
	-DTINY_GAME \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-create-app
	rm a.bin
	mv A.T64 $(BUILD_PATH)/TINY_c128_z80_40col.T64


c128_z80_40col_disk:
	$(Z88DK_PATH)$(MYZ88DK) +c128 -O3 -lndos -subtype=disk \
	-D__CPM_VT100__ -D__C128_Z80__ -DFORCE_XSIZE=40 \
	-DTINY_GAME \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-create-app
	$(TOOLS_PATH)/c1541 -format "crosschase,0" d64 FULL_c128_z80_40col.d64
	$(TOOLS_PATH)/c1541 -attach FULL_c128_z80_40col.d64 -write a.ldr
	$(TOOLS_PATH)/c1541 -attach FULL_c128_z80_40col.d64 -write a
	mv FULL_c128_z80_40col.d64 $(BUILD_PATH)/
	rm A.LDR
	rm A

	
# 	$(TOOLS_PATH)/cc1541 -n mydisk.d64 write a.ldr write a exit 


	
	
c128_z80_40col_zsdcc:
	$(Z88DK_PATH)$(MYZ88DK) +c128 -compiler=sdcc -SO3 -lndos \
	-D__CPM_VT100__ -D__C128_Z80__ -DFORCE_XSIZE=40 \
	-DTINY_GAME \
	-DNO_SLEEP \
	-DFORCE_CONIO \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-create-app
	mv A.T64 $(BUILD_PATH)/TINY_c128_z80_40col_zsdcc.T64
	

	
einstein:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -leinstein \
	-D__EINSTEIN__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-create-app -o$(BUILD_PATH)/TINY_einstein.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_einstein.bin
	

	
	
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
	cmoc -o $(BUILD_PATH)/FULL_CoCoDragon.bin \
	$(COCO_OPTS) \
	$(SOURCE_PATH)/main.c \
	display_macros.o cmoc_graphics.o \
	enemy.o \
	level.o character.o text.o \
	strategy.o \
	input_macros.o cmoc_input.o \
	item.o missile.o invincible_enemy.o \
	rocket.o horizontal_missile.o end_screen.o
	rm 	display_macros.o cmoc_graphics.o \
	enemy.o \
	level.o character.o text.o \
	strategy.o \
	input_macros.o cmoc_input.o \
	item.o missile.o invincible_enemy.o \
	rocket.o horizontal_missile.o end_screen.o
		


.PHONY: mtx vic20exp_8k vic20exp_16k  atari_color atari_no_color atari_no_color_16k atari5200 atmos c128_40col c128_80col c16_16k c16_32k c64 pet cbm510 cbm610 nes apple2 apple2enh

# KO: 
# OK: 30
# ------------
cc65_targets: \
	vic20_unexpanded vic20_exp_3k vic20_exp_8k vic20_exp_16k \
	atari_color atari_no_color atari_no_color_16k \
	atmos oric1_16k \
	c16_16k c16_32k c16_16k_full \
	pet_8k pet_16k \
	cbm510 cbm610 \
	apple2 apple2enh \
	c64 c128_40col c128_80col \
	pce_8k pce_16k \
	atari5200 nes \
	creativision_8k creativision_16k \
	gamate atari_lynx \
	osic1p_8k osic1p_32k


# KO:   
#
# OK:  


z88dk_targets: \
	z1013 \
	x1_tiny x1 \
	px4_tiny px4 px4_putc4x6 \
	px8_tiny px8 \
	kc_sprites_tiny \
	trs80_tiny trs80 \
	nc100_sprites_light \
	cpm_adm3a_tiny \
	nascom_16k nascom_32k \
	pc6001_16k pc6001_32k \
	z9001_16k z9001_32k \
	vg5k vg5k_exp_16k \
	sc3000_16k sc3000_32k \
	ace_exp_16k 
	cpc cpc_no_udg \
 	mc1000_16k mc1000_48k \
 	sharp_mz \
	mtx \
	abc80_16k abc80_32k \
	p2000_16k p2000_32k \
	svi_318 svi_318_mode0 svi_328 \
	msx_color_16k msx_color_32k_rom msx_color_32k \
	spectrum_16k spectrum_48k samcoupe \
	aquarius_exp_4k aquarius_exp_16k \
	vz200_16k vz200_32k \
	microbee_16k microbee_32k \
	gal_22k lambda_16k \
	zx80_16k zx80_8k \
	zx81_16k zx81_8k


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
	
vic20_targets: \
	vic20_unexpanded vic20_exp_3k vic20_exp_8k vic20_exp_16k

c264_targets: \
	c16_16k c16_16k_full c16_32k
	
	
	
	
	
	

####################################################################################################################
	
# DEBUG

nc200_sprites_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc200 \
	-D__NC200__ \
	-DZ88DK_SPRITES \
	-DTINY_GAME \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DREDEFINED_CHARS \
	-create-app -o$(BUILD_PATH)/TINY_nc200_sprites.bin \
	$(SOURCE_PATH)/z88dk_sprites/z88dk_sprites_graphics.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_nc200_sprites.bin


kc_putc4x6_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap \
	-D__KC__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-DZ88DK_PUTC4X6 \
	-create-app -o$(BUILD_PATH)/TINY_kc_putc4x6_.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_kc_putc4x6_.bin
	

eg2k_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos -subtype=eg2000disk \
	-lm -create-app \
	-D__EG2K__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-o$(BUILD_PATH)/TINY_eg2k.bin \
	$(SOURCE_PATH)/trs80/trs80_input.c \
	$(SOURCE_PATH)/trs80/trs80_graphics.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_eg2k.bin	

kc_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap \
	-D__KC__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-create-app -o$(BUILD_PATH)/TINY_kc.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_kc.bin

# -o$(BUILD_PATH)/TINY_abc800.bin 	
abc800_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +abc800 -zorg=40000 \
	-D__ABC800__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-o$(BUILD_PATH)/TINY_abc800.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c


m5_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	-lm -create-app -Cz--audio \
	-D__M5__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-o$(BUILD_PATH)/TINY_m5.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c


cpc_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 -DREDEFINED_CHARS -vn  -clib=ansi \
	-D__CPC__ 	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-DCPCRSLIB \
	-pragma-define:REGISTER_SP=-1 \
	-lndos -create-app -o 	$(BUILD_PATH)/TINY_cpc.prg \
	$(TOOLS_PATH)/cpcrslib/cpc_Chars.asm \
	$(TOOLS_PATH)/cpcrslib/cpc_Chars8.asm \
	$(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/TINY_cpc.cpc  $(BUILD_PATH)/TINY_cpc.cdt
	rm $(BUILD_PATH)/TINY_cpc.cpc 
	rm $(BUILD_PATH)/TINY_cpc.prg			


cpc_cpcrslib:
	$(Z88DK_PATH)$(MYZ88DKASM) -v \
	-x$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lib \
	@$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lst	
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 \
	-pragma-define:REGISTER_SP=-1 \
	-DREDEFINED_CHARS -DSOUNDS -DFULL_GAME -clib=ansi -D__CPC__ -DCPCRSLIB -DBETWEEN_LEVEL -DEND_SCREEN \
	-l$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib -lndos \
	-create-app -o $(BUILD_PATH)/FULL_cpc_cpcrslib.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c \
	$(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_cpcrslib.cpc  $(BUILD_PATH)/FULL_cpc_cpcrslib.cdt
	rm $(BUILD_PATH)/FULL_cpc_cpcrslib.cpc 
	rm $(BUILD_PATH)/FULL_cpc_cpcrslib.prg



enterprise_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +enterprise \
	-create-app -o $(BUILD_PATH)/TINY_enterprise.app \
	-lm -vn -lndos \
	-DTINY_GAME \
	-DNO_INPUT \
	-DNO_SLEEP -DLESS_TEXT -DALT_PRINT \
	$(SOURCE_PATH)/enterprise/enterprise_graphics.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c


gcc_tiny:
	gcc -D__GCC__ \
	-DTINY_GAME \
	-DNO_SLEEP -DLESS_TEXT -DALT_PRINT \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c

	
sms_libctest:
	$(Z88DK_PATH)$(MYZ88DK) +sms \
	-vn -lndos \
	-create-app \
	-o $(BUILD_PATH)/sms_libctest.sms \
	$(SOURCE_PATH)/../experiments/libctest.c

c128_hello:
	$(Z88DK_PATH)$(MYZ88DK) +c128 \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/hello.c \
	-create-app
	
vg5k_hello:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k -compiler=sdcc \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/vg5k_hello.c \
	-create-app


sms_hello:
	$(Z88DK_PATH)$(MYZ88DK) +sms \
	-vn -lndos \
	-create-app \
	-o $(BUILD_PATH)/sms_hello.sms \
	$(SOURCE_PATH)/../experiments/sms_hello.c

sms_chicken:
	$(Z88DK_PATH)$(MYZ88DK) +sms \
	-vn -lndos \
	-create-app \
	-o $(BUILD_PATH)/sms_chicken.sms \
	$(SOURCE_PATH)/../experiments/chicken/chicken.c \
	$(SOURCE_PATH)/../experiments/chicken/chicken_graphics.asm	


sms_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +sms \
	-D__SMS__ \
	-DTINY_GAME \
	-DNO_SLEEP -DLESS_TEXT -DALT_PRINT \
	-create-app \
	-o $(BUILD_PATH)/TINY_sms.bin \
	-vn -lndos \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv $(BUILD_PATH)/TINY_sms.bin $(BUILD_PATH)/TINY_sms.sms

# 	-pragma-need=ansiterminal -pragma-define:ansipixels=240 -pragma-define:ansicolumns=40 
# -DNO_BORDER


creativision_32k:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	-DNO_SLEEP -DLESS_TEXT -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(SOURCE_PATH)/../cfg/creativision-32k.cfg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	-o $(BUILD_PATH)/FULL_creativision_32k.bin

zx81_light:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-compiler=sdcc \
	-SO3 --max-allocs-per-node200000 \
	-vn \
	-D__ZX81__ -DNO_SLEEP -DLESS_TEXT \
	-lndos \
	-create-app -o  $(BUILD_PATH)/LIGHT_zx81_8k.prg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/missile.c \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_zx81_8k.prg


z88_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +z88 \
	-D__Z88__ \
	-DTINY_GAME \
	-subtype=app -create-app \
	-o $(BUILD_PATH)/TINY_z88.bin \
	-vn -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c


osca_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +osca \
	-D__OSCA__ -DNO_SLEEP -DLESS_TEXT \
	-DTINY_GAME \
	-o $(BUILD_PATH)/TINY_osca.bin -vn -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c

m5_hello:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	-create-app -vn -o$(BUILD_PATH)/m5_hello -lndos \
	$(SOURCE_PATH)/../experiments/hello.c


trs80_hello:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 \
	-create-app -vn -o$(BUILD_PATH)/trs80_hello -lndos \
	$(SOURCE_PATH)/../experiments/hello.c	
	
x1_hello:
	$(Z88DK_PATH)$(MYZ88DK) +x1 \
	-o $(BUILD_PATH)/x1_hello.bin -vn -lndos \
	$(SOURCE_PATH)/../experiments/hello.c
	

x1_wait_press:
	$(Z88DK_PATH)$(MYZ88DK) +x1 \
	-o $(BUILD_PATH)/x1_wait_press.bin -vn -lndos \
	$(SOURCE_PATH)/../experiments/wait_press.c

z1013_getk:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 \
	-o $(BUILD_PATH)/z1013_getk.bin -vn -lndos \
	$(SOURCE_PATH)/../experiments/wait_press.c	
	
x1_getk:
	$(Z88DK_PATH)$(MYZ88DK) +x1 \
	-o $(BUILD_PATH)/x1_getk.bin -vn -lndos \
	$(SOURCE_PATH)/../experiments/test_getk.c
	
ts2068:
	$(Z88DK_PATH)$(MYZ88DK) +ts2068 -O3 \
	-D__TS2068__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DFULL_GAME  -lndos \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_ts2068.bin



ts2068_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +ts2068 \
	-D__TS2068__ \
	-DTINY_GAME \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	-o $(BUILD_PATH)/FULL_ts2068.bin -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	
z1013_light:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 -O3 -clib=ansi \
	-vn -lndos \
	-D__Z1013__ -DCLIB_ANSI \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \
	-create-app -o
	mv $(BUILD_PATH)/../A.Z80 $(BUILD_PATH)/LIGHT_z1013.z80
	rm $(BUILD_PATH)/../a.bin


z1013_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 -O3 -clib=ansi \
	-D__Z1013__ -vn  -DCLIB_ANSI -DTINY_GAME -DNO_SLEEP \
	-lndos \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c  $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \
	-create-app -o
	mv $(BUILD_PATH)/../A.Z80 $(BUILD_PATH)/TINY_z1013.z80
	rm $(BUILD_PATH)/../a.bin
	
	
# import as data into ram at 32768 - call 32768
samcoupe_light:
	$(Z88DK_PATH)$(MYZ88DK) +sam -O0 \
	-D__SAM__  \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DCLIB_ANSI -o $(BUILD_PATH)/LIGHT_samcoupe.bin -lndos \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	
	
samcoupe_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +sam -O0 \
	-D__SAM__ -DTINY_GAME \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DCLIB_ANSI -o $(BUILD_PATH)/TINY_samcoupe.bin -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	
	
zx81_16k_sccz80:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-O3 \
	-vn \
	-D__ZX81__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o  $(BUILD_PATH)/FULL_zx81_16k_sccz80.prg \
	$(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_zx81_16k_sccz80.prg


supervision_tiny:
	$(CC65_PATH)$(MYCC65) -t supervision \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-o $(BUILD_PATH)/TINY_supervision.ws \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/supervision/supervision_graphics.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	
watara_test:
	$(CC65_PATH)$(MYCC65) -t supervision \
	$(SOURCE_PATH)/../experiments/supervision_test.c \
	-o $(BUILD_PATH)/supervision_test.ws

sound_test:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	-clib=ansi -pragma-define:ansicolumns=32 -vn -lndos \
	-create-app -o $(BUILD_PATH)/sound_test \
	experiments/sound_test.c


sc3000_tiny: 
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	-O3 \
	-pragma-need=ansiterminal \
	-DTINY_GAME -DNO_SLEEP -DLESS_TEXT -D__GAL__ -DNO_RANDOM_LEVEL -DALT_PRINT -DNO_MESSAGE -DNO_STATS \
	-clib=ansi \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/TINY_sc3000.prg \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	#rm $(BUILD_PATH)/TINY_sc3000.prg


pointerToFunction:
	$(CC65_PATH)$(MYCC65) -t pce \
	$(SOURCE_PATH)/../experiments/pointerToFunction.c \
	-o $(BUILD_PATH)/pointerToFunctions.pce
	

# -DNO_MESSAGE \
# 	-compiler=sdcc \
#	-SO3 --max-allocs-per-node200000 \
# -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL
# -DALT_PRINT
# -pragma-include:$(SOURCE_PATH)/../cfg/zpragma.inc

gal_6k_sccz80: 
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-O3 \
	-pragma-need=ansiterminal \
	-DTINY_GAME -DNO_SLEEP -DLESS_TEXT -D__GAL__ -DNO_RANDOM_LEVEL -DALT_PRINT -DNO_MESSAGE -DNO_STATS \
	-vn -lndos -create-app -Cz--audio \
	-o  $(BUILD_PATH)/TINY_galaksija_6k_sccz80.prg \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_galaksija_6k_sccz80.prg
	#rm $(BUILD_PATH)/TINY_galaksija_6k_sccz80.wav
	

gal_6k:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-compiler=sdcc \
	-SO3 --max-allocs-per-node200000 \
	-zorg=10239 \
	--opt-code-size \
	-pragma-need=ansiterminal \
	-pragma-include:$(SOURCE_PATH)/../cfg/zpragma.inc \
	-DTINY_GAME \
	-DNO_SLEEP -DLESS_TEXT \
	-D__GAL__ \
	-DNO_RANDOM_LEVEL \
	-DNO_SET_SCREEN_COLOR \
	-DNO_STATS \
	-DNO_INITIAL_SCREEN \
	-DALT_PRINT \
	-DNO_MESSAGE \
	-DFORCE_BOMBS_NUMBER=2 \
	-DNO_DEAD_GHOSTS \
	-vn -lndos -create-app -Cz--audio -o  $(BUILD_PATH)/TINY_galaksija_6k.prg \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	#rm $(BUILD_PATH)/TINY_galaksija_6k.prg
	#rm $(BUILD_PATH)/TINY_galaksija_6k.wav
		
	
# too big for a 16k machine ?
# -DSOUNDS $(SOURCE_PATH)/psg/psg_sounds.c
# 
# sdcc ONLY without PSG
# sccz80 works with AND without PSG
svi_318_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +svi -zorg=49152 \
	-clib=ansi -pragma-define:ansicolumns=32 -vn -lndos \
	-compiler=sdcc \
	-DTINY_GAME \
	-D__SVI__ -create-app -o $(BUILD_PATH)/TINY_svi_318 \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	#rm $(BUILD_PATH)/TINY_svi_318




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


	
# zx81_8k_sccz80:
	# $(Z88DK_PATH)$(MYZ88DK) +zx81 -O3 -v \
	# -D__ZX81__ -DROUND_ENEMIES -DTINY_GAME \
	# -lndos -create-app -o  $(BUILD_PATH)/TINY_zx81_8k_sccz80.prg \
	# $(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	# $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	# $(SOURCE_PATH)/main.c
	# rm $(BUILD_PATH)/TINY_zx81_8k_sccz80.prg
	
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

	
lambda_8k:
	$(Z88DK_PATH)$(MYZ88DK) +lambda -O3 -vn -D__LAMBDA__ -DTINY_GAME -DNO_SET_SCREEN_COLOR -DLESS_TEXT -DNO_SLEEP -lndos -create-app -o  $(BUILD_PATH)/TINY_lambda_8k.prg $(SOURCE_PATH)/zx81/zx81_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_lambda_8k.prg	
	
# -----------------------------------------------------------------------------------------------
	

atari5200_light:
	$(CC65_PATH)$(MYCC65) -O -t atari5200 $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_atari5200.rom	
	

vg5k_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k -O0 -vn -DTINY_GAME -D__VG5K__ -lndos -create-app -o $(BUILD_PATH)/TINY_vg5k.prg \
	$(SOURCE_PATH)/vg5k/vg5k_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
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

		
gamate_tiny:
	$(CC65_PATH)$(MYCC65) -O -t gamate -DTINY_GAME $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/gamate/gamate_graphics.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_gamate.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/TINY_gamate.bin
	
gamate_light:
	$(CC65_PATH)$(MYCC65) -O -t gamate $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/gamate/gamate_graphics.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_gamate.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/LIGHT_gamate.bin

	# TODO: Reduce size in order to compile	


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
	
	
creativision_16k_2:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	-DNO_SLEEP -DLESS_TEXT \
	--config $(SOURCE_PATH)/../cfg/creativision-16k_2.cfg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c \
	--code-name CODE2 \
	$(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	-o $(BUILD_PATH)/LIGHT_creativision_16k.bin	
	dd if=$(BUILD_PATH)/LIGHT_creativision_16k.bin ibs=1 count=8192 of=$(BUILD_PATH)/LIGHT_creativision_16k_LOW.bin
	dd if=$(BUILD_PATH)/LIGHT_creativision_16k.bin ibs=1 skip=8192 of=$(BUILD_PATH)/LIGHT_creativision_16k_HIGH.bin
	rm $(BUILD_PATH)/LIGHT_creativision_16k.bin 
	cat $(BUILD_PATH)/LIGHT_creativision_16k_HIGH.bin $(BUILD_PATH)/LIGHT_creativision_16k_LOW.bin > $(BUILD_PATH)/LIGHT_creativision_16k.bin
	rm $(BUILD_PATH)/LIGHT_creativision_16k_LOW.bin
	rm $(BUILD_PATH)/LIGHT_creativision_16k_HIGH.bin
	

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
	$(CC65_PATH)$(MYCC65) -O -t pce -Cl \
	--config $(SOURCE_PATH)/../cfg/pce_extra.cfg \
	-DNO_SLEEP -DLESS_TEXT -DNO_COLOR -DNO_RANDOM_LEVEL \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c \
	$(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/LIGHT_pce.pce

	
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


	
creativision_hello:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	$(SOURCE_PATH)/../experiments/hello.c \
	-o $(BUILD_PATH)/creativision_hello.bin
	
gal_hello:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-compiler=sdcc \
	-SO3 --max-allocs-per-node200000 \
	-pragma-need=ansiterminal \
	-pragma-include:$(SOURCE_PATH)/../cfg/zpragma.inc \
	-vn -lndos -create-app -Cz--audio \
	$(SOURCE_PATH)/../experiments/hello.c \
	-o  $(BUILD_PATH)/hello.prg
	rm $(BUILD_PATH)/hello.prg
	
cpc_hello:
	$(Z88DK_PATH)$(MYZ88DK) +cpc -O3 $(SOURCE_PATH)/../experiments/hello.c \
	-lndos \
	-create-app -o $(BUILD_PATH)/hello.prg
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/hello.cpc $(BUILD_PATH)/hello.cdt
	rm $(BUILD_PATH)/hello.cpc 
	rm $(BUILD_PATH)/hello.prg	

z1013_hello:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 -O3 $(SOURCE_PATH)/../experiments/hello.c \
	-lndos -vn -clib=ansi \
	-create-app -o
	mv $(BUILD_PATH)/../A.Z80 $(BUILD_PATH)/z1013_hello.z80
	rm $(BUILD_PATH)/../a.bin

vg5k_wait_press:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k $(SOURCE_PATH)/../experiments/wait_press.c \
	-lndos -vn  -zorg=19000 \
	-create-app -o $(BUILD_PATH)/vg5k_wait_press.prg


z1013_wait_press:
	$(Z88DK_PATH)$(MYZ88DK) +z1013  $(SOURCE_PATH)/../experiments/wait_press.c \
	-lndos -vn -clib=ansi \
	-create-app -o
	mv $(BUILD_PATH)/../A.Z80 $(BUILD_PATH)/z1013_wait_press.z80
	rm $(BUILD_PATH)/../a.bin	

	
msx_color_32k_msxdos:
	$(Z88DK_PATH)$(MYZ88DK) +msx -O3 -DSOUNDS -DREDEFINED_CHARS -vn -DMSX_MODE1 -DFULL_GAME -D__MSX__ -lndos -subtype=msxdos -o $(BUILD_PATH)/FULL_msx_color_32k.com $(SOURCE_PATH)/msx/msx_graphics.c $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	
