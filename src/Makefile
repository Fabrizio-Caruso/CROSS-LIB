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

ifeq ($(DO_WIN),1)
COMPILEDEXT = .exe
else
COMPILEDEXT = .out
endif


SOURCE_PATH := chase
CROSS_LIB_PATH := cross_lib

CC65_PATH ?= /cygdrive/c/cc65-snapshot-win32/bin/
Z88DK_PATH ?= /cygdrive/c/z88dk/bin/
Z88DK_PATH_20171210 ?= /cygdrive/c/z88dk_20171210/bin/
Z88DK_INCLUDE ?= /cygdrive/c/z88dk/include
BUILD_PATH ?= ../build
MYCC65 ?= cl65$(EXEEXT)
MYZ88DK ?= zcc$(EXEEXT)
MYZ88DKASM ?= z80asm$(EXEEXT)
SCCZ80_OPTS ?= -O3
ZSDCC_OPTS ?= 
#-SO3 --max-allocs-per-node200000

TOOLS_PATH ?= ../tools

CFG_PATH ?= $(CROSS_LIB_PATH)/cfg/

BASIC_CROSS_LIB_FILES ?= \
	$(CROSS_LIB_PATH)/display/display_macros.c \
	$(CROSS_LIB_PATH)/display/init_images.c \
	$(CROSS_LIB_PATH)/input/input_macros.c \
	$(CROSS_LIB_PATH)/text/text_macros.c

NO_CHASE_FILES ?= \
	$(BASIC_CROSS_LIB_FILES) \
	$(SOURCE_PATH)/move_player.c \
	$(SOURCE_PATH)/ghost.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
	$(SOURCE_PATH)/main.c

	
TINY_FILES ?= $(NO_CHASE_FILES) $(SOURCE_PATH)/strategy.c

TEST_FILES ?= $(NO_CHASE_FILES)

LIGHT_ONLY_FILES ?= $(SOURCE_PATH)/item.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/bullet.c 
FULL_ONLY_FILES ?= $(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c 

LIGHT_FILES ?= $(TINY_FILES) $(LIGHT_ONLY_FILES) 

FULL_FILES ?= $(LIGHT_FILES) $(FULL_ONLY_FILES)

# For cygwin posix build: use gcc
# For windows 32 non-posix build: x86_64-w64-mingw32-gcc
_CC ?= gcc
_CC_OPTS ?= -Wpedantic -Wall


COCO_OPTS_TINY  ?= -O0 -Wno-const  -D__COCO__ -DTINY_GAME

COCO_OPTS_LIGHT ?= -O0 -Wno-const -D__COCO__

COCO_OPTS       ?= -O0 -Wno-const -DALT_CLEAR_SCREEN -D__CMOC__ -D__COCO__ -DMEMORY_MAPPED -DNO_INIT_GRAPHICS -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL -DCMOC_RAND_FIX


VECTREX_OPTS    ?= -O0 --vectrex -DNO_SLEEP -DNO_WAIT -DNO_INPUT -DNO_PRINT -Wno-const -DALT_CLEAR_SCREEN -D__CMOC__ -D__VECTREX__ -DVECTORIAL -DNO_INIT_GRAPHICS -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL -DCMOC_RAND_FIX



_ACK ?= ack
_ACK_OPTS ?= -O6
# -O 
# -ansi


# -DNO_SLEEP

# ------------------------------------------------------------------------------------------
#CC65
# 

# 	$(CROSS_LIB_PATH)/display/cc65/atari/atari_mode1_redefined_chars_init_graphics.c 

atari_color: 
	$(CC65_PATH)$(MYCC65) -O -t atari \
	-DANIMATE_PLAYER -DFULL_GAME -DATARI_MODE1 -DSOUNDS -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(CFG_PATH)/cc65/atari_mode1_redefined_chars.cfg \
	-DMEMORY_MAPPED \
	-DALT_CLEAR_SCREEN \
	-DNO_SET_SCREEN_COLORS \
	-DALT_PRINT \
	$(CROSS_LIB_PATH)/display/alt_print/atari_mode1_alt_print.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atari/disable_setcursor.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atari/atari_mode1_redefined_chars_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/pokey/pokey_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_atari_color.xex

atari_no_color: 
	$(CC65_PATH)$(MYCC65) -O -t atari \
	-DFULL_GAME -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(CROSS_LIB_PATH)/sound/cc65/pokey/pokey_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_atari_no_color.xex


# --config $(SOURCE_PATH)/../cfg/cc65/atari5200_less_stack.cfg
# atari5200: 
	# $(CC65_PATH)$(MYCC65) -O -t atari5200 \
	# -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	# -DCONIO -DNO_INIT_GRAPHICS \
	# $(SOURCE_PATH)/end_screen.c \
	# $(FULL_FILES) \
	# -o $(BUILD_PATH)/FULL_atari5200.rom

	
atari5200: 
	$(CC65_PATH)$(MYCC65) -O -t atari5200 \
	--config $(CFG_PATH)/cc65/atari5200_redefined_chars.cfg \
	-D__ATARI5200__ \
	-DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DMEMORY_MAPPED \
	-DANIMATE_PLAYER \
	-DSOUNDS \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/sound/cc65/pokey/pokey_sounds.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atari5200/atari5200_init_graphics.c \
	-o $(BUILD_PATH)/FULL_atari5200.rom
	

atmos: 
	$(CC65_PATH)$(MYCC65)  -O  -DSOUNDS -DANIMATE_PLAYER -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-t atmos \
	-DALT_MOVE \
	--config $(CFG_PATH)/cc65/atmos_better_tap.cfg \
	-DMEMORY_MAPPED \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atmos/atmos_redefined_characters_init_graphics.c \
	$(FULL_FILES) \
	$(SOURCE_PATH)/end_screen.c \
	-o $(BUILD_PATH)/FULL_atmos_and_oric1_48k.tap


oric1_16k: 
	$(CC65_PATH)$(MYCC65)  -O -D__ORIC1__ -DSOUNDS -DANIMATE_PLAYER \
	-t atmos \
	--config $(CFG_PATH)/cc65/atmos_better_tap.cfg \
	-DMEMORY_MAPPED \
	-DALT_MOVE \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atmos/atmos_redefined_characters_init_graphics.c \
	$(LIGHT_FILES) \
	-o $(BUILD_PATH)/LIGHT_oric1_16k.tap

	
# 	$(CC65_PATH)$(MYCC65) -O -Cl -t vic20 -m 746e3d1.map -Wl -vm 
# 	-DNO_INITIAL_SCREEN 
# 	-DNO_MESSAGE 
vic20_unexpanded:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 \
	-DANIMATE_PLAYER \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_TEXT_COLOR \
	-DVIC20_UNEXPANDED \
	-DNO_SLEEP \
	-DLESS_TEXT \
	-DNO_SET_SCREEN_COLORS \
	-DTINY_GAME \
	-DNO_STATS \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DNO_DEAD_GHOSTS \
	-DFORCE_BOMBS_NUMBER=4 \
	-DMEMORY_MAPPED \
	-DALT_DISPLAY_STATS \
	-DALT_HIGHSCORE \
	-DROUND_ENEMIES \
	--config $(CFG_PATH)/cc65/vic20_unexpanded.cfg  \
	$(TINY_FILES) \
	$(SOURCE_PATH)/vic20_memory.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/vic20_alt_stats.c \
	-o $(BUILD_PATH)/TINY_vic20_unexpanded.prg
	
vic20_unexpanded_turn_based:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 \
	-DFORCE_KEYBOARD \
	-DTURN_BASED \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_TEXT_COLOR \
	-DVIC20_UNEXPANDED \
	-DNO_SLEEP \
	-DLESS_TEXT \
	-DNO_SET_SCREEN_COLORS \
	-DTINY_GAME \
	-DNO_RANDOM_LEVEL \
	-DNO_STATS \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DNO_DEAD_GHOSTS \
	-DFORCE_BOMBS_NUMBER=4 \
	-DMEMORY_MAPPED \
	-DALT_DISPLAY_STATS \
	-DFLAT_ENEMIES \
	--config $(CFG_PATH)/cc65/vic20_unexpanded.cfg  \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/vic20_alt_stats.c \
	-o $(BUILD_PATH)/TINY_vic20_unexpanded.prg	
	
vic20_exp_3k:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 \
	-DREDEFINED_CHARS \
	-DANIMATE_PLAYER -DLESS_TEXT -DNO_SET_SCREEN_COLORS -DNO_DEAD_GHOSTS -DFORCE_GHOSTS_NUMBER=8 \
	-DROUND_ENEMIES \
	-DTINY_GAME -DSOUNDS \
	--config $(CFG_PATH)/cc65/vic20-3k_GFX.cfg \
	-DMEMORY_MAPPED \
	-DVIC20_EXP_3K \
	-DALT_PRINT \
	-DCBM_SCREEN_CODES \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds_3k.c \
	$(CROSS_LIB_PATH)/display/graphics_data/cc65/udc_3k.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(TINY_FILES) \
	$(SOURCE_PATH)/vic20_memory.s \
	-o $(BUILD_PATH)/TINY_vic20_exp_3k.prg

	
	
vic20_exp_8k_full:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 --config $(CFG_PATH)/cc65/vic20-8k.cfg \
	-DFULL_GAME \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DSIMPLE_STRATEGY \
	-DLESS_TEXT	\
	-DALT_PRINT \
	-DNO_HINTS \
	-DSOUNDS \
	-DALT_SLEEP \
	-DANIMATE_PLAYER \
	-DMEMORY_MAPPED \
	-DVIC20_EXP_8K \
	-DBETWEEN_LEVEL \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	$(SOURCE_PATH)/vic20_memory.s \
	-o $(BUILD_PATH)/FULL_vic20_exp_8k_NO_GFX.prg		


vic20_exp_8k: 
	$(CC65_PATH)$(MYCC65) -Or -t vic20  -DSOUNDS -DANIMATE_PLAYER --config $(CFG_PATH)/cc65/vic20-8k_GFX.cfg \
	-DCONIO -DVIC20_EXP_8K \
	-DREDEFINED_CHARS \
	$(CROSS_LIB_PATH)/display/graphics_data/cc65/udc.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c  $(CROSS_LIB_PATH)/text/text_macros.c \
	$(SOURCE_PATH)/skull.c $(SOURCE_PATH)/ghost.c $(CROSS_LIB_PATH)/input/input_macros.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/bullet.c \
	$(SOURCE_PATH)/move_player.c \
	$(CROSS_LIB_PATH)/display/init_images.c \
	$(SOURCE_PATH)/item.c	\
	--code-name CODE2 \
	$(CROSS_LIB_PATH)/display/display_macros.c  \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/vic20_memory.s \
	-o $(BUILD_PATH)/LIGHT_vic20_exp_8k.prg

	
vic20_exp_16k: 
	$(CC65_PATH)$(MYCC65) -Or -t vic20 -DANIMATE_PLAYER -DFULL_GAME -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	--config $(CFG_PATH)/cc65/vic20-16k_GFX.cfg \
	-DCONIO -DVIC20_EXP_16K \
	-DREDEFINED_CHARS \
	$(CROSS_LIB_PATH)/display/graphics_data/cc65/udc.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(SOURCE_PATH)/vic20_memory.s \
	-o $(BUILD_PATH)/FULL_vic20_exp_16k.prg

	
c64: 
	$(CC65_PATH)$(MYCC65) -O -t c64 \
	-DFULL_GAME -DANIMATE_PLAYER -DSOUNDS -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(CFG_PATH)/cc65/c64_GFXat0xC000.cfg  \
	-DCBM_SCREEN_CODES \
	-DMEMORY_MAPPED \
	$(CROSS_LIB_PATH)/sound/cc65/sid/sid_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c64/graphics.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c64/c64_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c64.prg
	$(TOOLS_PATH)/generic/exomizer sfx basic $(BUILD_PATH)/FULL_c64.prg -o $(BUILD_PATH)/FULL_c64_exomized.prg
	python $(TOOLS_PATH)/cc65/c64/prg2crt.py $(BUILD_PATH)/FULL_c64_exomized.prg  $(BUILD_PATH)/FULL_c64_exomized.crt
	rm $(BUILD_PATH)/FULL_c64.prg


c64_8k_cart: 
	$(CC65_PATH)$(MYCC65) -O -t c64 \
	-DFULL_GAME -DANIMATE_PLAYER -DSOUNDS \
	-DLESS_TEXT \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DEND_SCREEN \
	-DALT_PRINT \
	-DBETWEEN_LEVEL \
	-DROUND_ENEMIES \
	-DCBM_SCREEN_CODES \
	-DMEMORY_MAPPED \
	--config $(CFG_PATH)/cc65/c64_GFXat0xC000.cfg \
	$(CROSS_LIB_PATH)/sound/cc65/sid/sid_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c64/graphics.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c64/c64_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c64_8k_cart.prg
	$(TOOLS_PATH)/generic/exomizer sfx basic  $(BUILD_PATH)/FULL_c64_8k_cart.prg -o $(BUILD_PATH)/FULL_c64_8k_exomized.prg
	python $(TOOLS_PATH)/cc65/c64/prg2crt.py $(BUILD_PATH)/FULL_c64_8k_exomized.prg  $(BUILD_PATH)/FULL_c64_8k_exomized.crt
	rm $(BUILD_PATH)/FULL_c64_8k_cart.prg
	rm $(BUILD_PATH)/FULL_c64_8k_exomized.prg
	
c128_40col: 
	$(CC65_PATH)$(MYCC65) -O -t c128 \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/sound/cc65/sid/sid_sounds.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c128_40col.prg

c128_80col: 
	$(CC65_PATH)$(MYCC65) -O -t c128  \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-DFORCE_GHOSTS_NUMBER=9 \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c128/c128_80col_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/sid/sid_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c128_80col.prg

# -DNO_SLEEP -DLESS_TEXT -DNO_RANDOM_LEVEL
# -DSOUNDS $(SOURCE_PATH)/c264/c264_sounds.c
# -Cl 
c16_16k: 
	$(CC65_PATH)$(MYCC65) -Or -t c16 \
	--config $(CFG_PATH)/cc65/c16-16k.cfg \
	-DANIMATE_PLAYER  -DSOUNDS \
	-DREDEFINED_CHARS \
	-DCONIO \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c264/c264_redefined_chars_init_graphics.c  \
	$(LIGHT_FILES) \
	-o $(BUILD_PATH)/LIGHT_c16_16k.prg
	
c16_32k: 
	$(CC65_PATH)$(MYCC65) -Or -t c16 --config $(CFG_PATH)/cc65/c16-32k.cfg \
	-DANIMATE_PLAYER -DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DREDEFINED_CHARS \
	-DCONIO \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c264/c264_redefined_chars_init_graphics.c  \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c16_32k.prg

# 	-DFLAT_ENEMIES
	
c16_16k_full: 
	$(CC65_PATH)$(MYCC65) -Or -Cl -t c16 \
	--config $(CFG_PATH)/cc65/c16_16k_less_stack.cfg \
	-DFULL_GAME \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DLESS_TEXT	\
	-DSOUNDS \
	-DALT_PRINT \
	-DANIMATE_PLAYER \
	-DMEMORY_MAPPED \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c264/c264_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c16_16k_NO_GFX.prg	

	
pet_8k_buffered: 
	$(CC65_PATH)$(MYCC65) -O -t pet -Cl \
	-DTINY_GAME -DNO_INIT_GRAPHICS \
	-DBUFFERED \
	-DALT_PRINT \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	-o $(BUILD_PATH)/TINY_pet_8k_buffered.prg	
	
pet_8k: 
	$(CC65_PATH)$(MYCC65) -O -t pet -Cl \
	-DTINY_GAME -DNO_INIT_GRAPHICS -DCONIO \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_pet_8k.prg
	
	
pet_16k: 
	$(CC65_PATH)$(MYCC65) -O -t pet \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_pet_16k.prg


cbm610: 
	$(CC65_PATH)$(MYCC65) -O -t cbm610 \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_cbm610.prg
	
cbm510: 
	$(CC65_PATH)$(MYCC65) -O -t cbm510 \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_cbm510.prg

nes: 
	$(CC65_PATH)$(MYCC65) -O -t nes \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_nes.nes	
	
	
	
nes_fx:
	$(CC65_PATH)$(MYCC65) -O -t nes \
	--config $(CFG_PATH)/cc65/nes_fx.cfg \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO \
	-DANIMATE_PLAYER \
	-DALT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/nes/nes_init_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/graphics_data/cc65/nes_tiles.s \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_nes_fx.nes
	
apple2:	 
	$(CC65_PATH)$(MYCC65) -O -t apple2 \
	-DFULL_GAME -DALT_SLEEP \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	-o $(BUILD_PATH)/apple2.bin
	cp $(TOOLS_PATH)/cc65/apple2/MASTER_BOOT_ASCHASE.DSK $(BUILD_PATH)/FULL_apple2.dsk
	java -jar $(TOOLS_PATH)/cc65/apple2/ac.jar -as $(BUILD_PATH)/FULL_apple2.dsk aschase < $(BUILD_PATH)/apple2.bin
	rm $(BUILD_PATH)/apple2.bin


apple2enh: 
	$(CC65_PATH)$(MYCC65) -O -t apple2enh \
	-DFULL_GAME -DALT_SLEEP \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	-o $(BUILD_PATH)/apple2enh.bin
	cp $(TOOLS_PATH)/cc65/apple2/MASTER_BOOT_ASCHASE.DSK $(BUILD_PATH)/FULL_apple2enh.dsk
	java -jar $(TOOLS_PATH)/cc65/apple2/ac.jar -as $(BUILD_PATH)/FULL_apple2enh.dsk aschase < $(BUILD_PATH)/apple2enh.bin
	rm $(BUILD_PATH)/apple2enh.bin


apple2enh_80col: 
	$(CC65_PATH)$(MYCC65) -O -t apple2enh \
	-DFULL_GAME -DALT_SLEEP \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-DFORCE_XSIZE=80 \
	-DFORCE_GHOSTS_NUMBER=9 \
	-DAPPLE2ENH_80COL_VIDEO_MODE \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/apple2enh/apple2enh_80col_init_graphics.c \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	-o $(BUILD_PATH)/apple2enh_80col.bin
	cp $(TOOLS_PATH)/cc65/apple2/MASTER_BOOT_ASCHASE.DSK $(BUILD_PATH)/FULL_apple2enh_80col.dsk
	java -jar $(TOOLS_PATH)/cc65/apple2/ac.jar -as $(BUILD_PATH)/FULL_apple2enh_80col.dsk aschase < $(BUILD_PATH)/apple2enh_80col.bin
	rm $(BUILD_PATH)/apple2enh_80col.bin	

	
osic1p_32k: 
	$(CC65_PATH)$(MYCC65) --start-addr 0x200 -Wl -D,__HIMEM__=0x8000 -O -t osic1p -DFULL_GAME \
	-DALT_SLEEP \
	-DNO_SET_SCREEN_COLORS \
	-DTURN_BASED \
	-DFORCE_GHOSTS_NUMBER=9 \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	-o $(BUILD_PATH)/FULL_osic1p_32k.lod
	$(TOOLS_PATH)/cc65/osic1p/srec_cat $(BUILD_PATH)/FULL_osic1p_32k.lod -binary -offset 0x200 -o $(BUILD_PATH)/FULL_osic1p_32k.c1p -Ohio_Scientific -execution-start-address=0x200	
	rm $(BUILD_PATH)/FULL_osic1p_32k.lod
	mv $(BUILD_PATH)/FULL_osic1p_32k.c1p $(BUILD_PATH)/FULL_osic1p_32k.lod


osic1p_8k: 
	$(CC65_PATH)$(MYCC65) -Cl --start-addr 0x200 -Wl -D,__HIMEM__=0x2000 -O --config $(CFG_PATH)/cc65/osic1p_less_stack.cfg -t osic1p \
	-DROUND_ENEMIES -DNO_SLEEP  -DNO_RANDOM_LEVEL -DLESS_TEXT -DNO_SET_SCREEN_COLORS -DTINY_GAME \
	-DTURN_BASED \
	-DFORCE_GHOSTS_NUMBER=9 \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_osic1p_8k.lod	
	$(TOOLS_PATH)/cc65/osic1p/srec_cat $(BUILD_PATH)/TINY_osic1p_8k.lod -binary -offset 0x200 -o $(BUILD_PATH)/TINY_osic1p_8k.c1p -Ohio_Scientific -execution-start-address=0x200	
	rm $(BUILD_PATH)/TINY_osic1p_8k.lod
	mv $(BUILD_PATH)/TINY_osic1p_8k.c1p $(BUILD_PATH)/TINY_osic1p_8k.lod
	
		
gamate: 
	$(CC65_PATH)$(MYCC65) -O -t gamate   --config $(CFG_PATH)/cc65/gamate_reduced_stack.cfg \
	-DFULL_GAME \
	-DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DSIMPLE_STRATEGY \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_gamate.bin
	$(TOOLS_PATH)/cc65/gamate/gamate-fixcart $(BUILD_PATH)/FULL_gamate.bin

gamate_light: 
	$(CC65_PATH)$(MYCC65) -O -t gamate \
	-DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DFORCE_GHOSTS_NUMBER=7 \
	-DSIMPLE_STRATEGY \
	$(LIGHT_FILES) \
	-o $(BUILD_PATH)/LIGHT_gamate.bin
	$(TOOLS_PATH)/cc65/gamate/gamate-fixcart $(BUILD_PATH)/LIGHT_gamate.bin
	
gamate_tiny: 
	$(CC65_PATH)$(MYCC65) -O -t gamate \
	-DTINY_GAME \
	-DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DFORCE_GHOSTS_NUMBER=7 \
	-DSIMPLE_STRATEGY \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_gamate.bin
	$(TOOLS_PATH)/cc65/gamate/gamate-fixcart $(BUILD_PATH)/TINY_gamate.bin
	
creativision_8k_tiny: 
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	--config $(CFG_PATH)/cc65/creativision-8k.cfg \
	-DTINY_GAME -DNO_SLEEP \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_creativision_8k.bin

creativision_8k_light: 
	$(CC65_PATH)$(MYCC65) -Or -Cl -t creativision \
	-DLESS_TEXT \
	-DNO_SET_SCREEN_COLORS \
	-DFLAT_ENEMIES \
	-DNO_BLINKING \
	-DNO_RANDOM_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DFORCE_NARROW \
	-DALT_SLEEP \
	-DFORCE_GHOSTS_NUMBER=8 \
	--config $(CFG_PATH)/cc65/creativision-8k_less_stack.cfg \
	$(LIGHT_FILES) \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	-o $(BUILD_PATH)/LIGHT_creativision_8k_less_text.bin	
	
creativision_16k:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	-DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN -DALT_SLEEP \
	-DCONIO -DNO_INIT_GRAPHICS \
	--config $(CFG_PATH)/cc65/creativision-16k.cfg \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
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
	-DNO_PRINT \
	-DTGI \
	-DALT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atari_lynx/atari_lynx_init_graphics.c \
	$(LIGHT_FILES) \
	-o $(BUILD_PATH)/LIGHT_ATARI_LYNX.lnx
	
	
# -DLESS_TEXT -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL
pce_8k:
	$(CC65_PATH)$(MYCC65) -O -Cl -t pce \
	-DTINY_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	--config $(CFG_PATH)/cc65/pce_8k_less_stack.cfg \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_PCE_8k.pce

pce_fx:
	$(CC65_PATH)$(MYCC65) -O -t pce \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	--config $(CFG_PATH)/cc65/pce_16k_less_stack.cfg \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/pce/pce_init_graphics.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_PCE.bin
	dd if=$(BUILD_PATH)/FULL_PCE.bin ibs=1 count=8192 of=$(BUILD_PATH)/FULL_PCE_LOW.bin
	dd if=$(BUILD_PATH)/FULL_PCE.bin ibs=1 skip=8192 of=$(BUILD_PATH)/FULL_PCE_HIGH.bin
	rm $(BUILD_PATH)/FULL_PCE.bin 
	cat $(BUILD_PATH)/FULL_PCE_HIGH.bin $(BUILD_PATH)/FULL_PCE_LOW.bin > $(BUILD_PATH)/FULL_PCE_SWAPPED.pce
	rm $(BUILD_PATH)/FULL_PCE_LOW.bin
	rm $(BUILD_PATH)/FULL_PCE_HIGH.bin	

pce_16k:
	$(CC65_PATH)$(MYCC65) -O -t pce \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	--config $(CFG_PATH)/cc65/pce_16k_less_stack.cfg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_PCE.bin
	dd if=$(BUILD_PATH)/FULL_PCE.bin ibs=1 count=8192 of=$(BUILD_PATH)/FULL_PCE_LOW.bin
	dd if=$(BUILD_PATH)/FULL_PCE.bin ibs=1 skip=8192 of=$(BUILD_PATH)/FULL_PCE_HIGH.bin
	rm $(BUILD_PATH)/FULL_PCE.bin 
	cat $(BUILD_PATH)/FULL_PCE_HIGH.bin $(BUILD_PATH)/FULL_PCE_LOW.bin > $(BUILD_PATH)/FULL_PCE_SWAPPED.pce
	rm $(BUILD_PATH)/FULL_PCE_LOW.bin
	rm $(BUILD_PATH)/FULL_PCE_HIGH.bin	



# It lacks conio and TGI
# --config $(SOURCE_PATH)/../cfg/cc65/supervision-16k.cfg
supervision:
	$(CC65_PATH)$(MYCC65) -O -t supervision  \
	-DNO_PRINT -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN -DNO_SLEEP \
	-DBIT_MAPPED -DNO_INIT_GRAPHICS \
	-DALT_CLEAR_SCREEN \
	-DNO_WAIT \
	-DNO_SLEEP \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_supervision.sv
	
# ------------------------------------------------------------------------------------------
#Z88DK
# 	-DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL \
# 	$(ZSDCC_OPTS) \

aquarius_exp_4k:
	$(Z88DK_PATH)$(MYZ88DK) +aquarius \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-v \
	-D__AQUARIUS__ -DTINY_GAME \
	-DNO_RANDOM_LEVEL \
	-DALT_PRINT \
	-DNO_DEAD_GHOSTS \
	-DFORCE_NARROW \
	-DALT_CLEAR_SCREEN \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-lndos -o TINY_aquarius_exp_4k -create-app \
	-DMEMORY_MAPPED \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/aquarius/aquarius_init_graphics.c
	rm TINY_aquarius_exp_4k
	tar czvf TINY_aquarius_exp_4k.tar.zip TINY_aquarius_exp_4k.caq _TINY_aquarius_exp_4k.caq
	rm TINY_aquarius_exp_4k.caq
	rm _TINY_aquarius_exp_4k.caq
	mv TINY_aquarius_exp_4k.tar.zip $(BUILD_PATH)



aquarius_exp_16k: 
	$(Z88DK_PATH)$(MYZ88DK) +aquarius -clib=ansi $(SCCZ80_OPTS) -vn \
	-DSOUNDS -D__AQUARIUS__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-create-app	\
	-o FULL_aquarius_exp_16k
	rm FULL_aquarius_exp_16k
	tar czvf FULL_aquarius_exp_16k.tar.zip FULL_aquarius_exp_16k.caq _FULL_aquarius_exp_16k.caq
	rm FULL_aquarius_exp_16k.caq
	rm _FULL_aquarius_exp_16k.caq
	mv FULL_aquarius_exp_16k.tar.zip $(BUILD_PATH)	
	# rm $(SOURCE_PATH)/../FULL_aquarius_exp_16k
	# mv $(SOURCE_PATH)/../FULL_aquarius_exp_16k.caq $(BUILD_PATH)
	# mv $(SOURCE_PATH)/../_FULL_aquarius_exp_16k.caq $(BUILD_PATH)


vz200_8k:
	$(Z88DK_PATH)$(MYZ88DK) +vz -v \
	-DTINY_GAME \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc \
	-compiler=sdcc \
	--opt-code-size \
	$(ZSDCC_OPTS) \
	-D__VZ__ -clib=ansi \
	-DLESS_TEXT \
	-DNO_BLINKING \
	-DNO_RANDOM_LEVEL \
	-DNO_DEAD_GHOSTS \
	-DFORCE_GHOSTS_NUMBER=4 \
	-DFORCE_BOMBS_NUMBER=2 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_STATS \
	-DNO_INITIAL_SCREEN \
	-DNO_MESSAGE \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o $(BUILD_PATH)/TINY_vz200_8k.vz \
	$(TINY_FILES) 
	rm $(BUILD_PATH)/TINY_vz200_8k.cas


vz200_18k: 	
	$(Z88DK_PATH)$(MYZ88DK) +vz -vn \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-DSOUNDS -D__VZ__ -clib=ansi \
	-DFULL_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_vz200_18k.vz \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_vz200_18k.cas


laser500: 	
	$(Z88DK_PATH)$(MYZ88DK) +laser500 -vn -create-app -Cz--audio \
	$(SCCZ80_OPTS) \
	-D__LASER500__ \
	-DFULL_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app  \
	$(FULL_FILES)
	mv a.bin $(BUILD_PATH)/FULL_vz500.bin
	rm a.cas
	mv a.wav $(BUILD_PATH)/FULL_vz500.wav
	

vz200_sprites_18k:
	$(Z88DK_PATH)$(MYZ88DK) +vz \
	$(SCCZ80_OPTS) \
	-DSOUNDS \
	-D__VZ__ \
	-DFULL_GAME \
	-create-app -o $(BUILD_PATH)/FULL_vz200_sprites_18k.vz \
	-vn -lndos \
	-DALT_PRINT \
	-DFORCE_NARROW \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_vz200_sprites_18k.cas	
	

vz700: 	
	$(Z88DK_PATH)$(MYZ88DK) +vz700 -vn \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-DSOUNDS -D__VZ__ -clib=ansi \
	-DFULL_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_vz350.vz \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_vz700.cas

		
	
	
vz200_24k: 
	$(Z88DK_PATH)$(MYZ88DK) +vz $(SCCZ80_OPTS) -vn \
	-DSOUNDS -D__VZ__ -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	-clib=ansi -lndos -create-app -o  $(BUILD_PATH)/FULL_vz200_24k.vz \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_vz200_24k.cas



# 	$(ZSDCC_OPTS) 
#   --opt-code-size 
#	-DSOUNDS
#	-DNO_INITIAL_SCREEN
# 	-DNO_MESSAGE
# 	-DFORCE_NARROW
vg5k:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	--opt-code-size \
	--reserve-regs-iy \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-DNO_BLINKING \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_HINTS \
	-DALT_SLEEP \
	-DNO_RANDOM_LEVEL \
	-DFLAT_ENEMIES \
	-DFORCE_NARROW \
	-vn -DFULL_GAME -D__VG5K__ \
	-DLESS_TEXT \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DFORCE_BOMBS_NUMBER=4 \
	-DSOUNDS \
	-DSIMPLE_STRATEGY \
	-create-app -o $(BUILD_PATH)/FULL_vg5k_less_text.prg \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c
#	rm $(BUILD_PATH)/FULL_vg5k_less_text.prg	

vg5k_udg:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	--reserve-regs-iy \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-DNO_BLINKING \
	-vn -DFULL_GAME -D__VG5K__ \
	-DLESS_TEXT \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DFORCE_BOMBS_NUMBER=4 \
	-DSOUNDS \
	-DSIMPLE_STRATEGY \
	-create-app -o $(BUILD_PATH)/FULL_vg5k_udg_less_text.prg \
	-DCONIO \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(FULL_FILES) 
	rm $(BUILD_PATH)/FULL_vg5k_udg_less_text.prg		
	
	
# $(SCCZ80_OPTS) -zorg=18941 -vn
# 	-DCONIO_VT52 
#	-DFORCE_NO_CONIO_LIB 
vg5k_exp_16k_udg:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	$(SCCZ80_OPTS) \
	-DSOUNDS -vn -DFULL_GAME -D__VG5K__ -DBETWEEN_LEVEL -DEND_SCREEN -DASM_DISPLAY \
	-lndos -create-app -o $(BUILD_PATH)/FULL_vg5k_exp_16k_udg.prg \
	-DCONIO \
	-clib=default \
	-DUDG_GRAPHICS \
	-DANIMATE_PLAYER \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_vg5k_exp_16k_udg.prg	

vg5k_exp_16k:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	$(SCCZ80_OPTS) \
	-DSOUNDS -vn -DFULL_GAME -D__VG5K__ -DBETWEEN_LEVEL -DEND_SCREEN -DASM_DISPLAY \
	-lndos -create-app -o $(BUILD_PATH)/FULL_vg5k_exp_16k.prg \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_vg5k_exp_16k.prg	

# 	-clib=ansi
ace_exp_16k_udg:
	$(Z88DK_PATH)$(MYZ88DK) +ace $(SCCZ80_OPTS) \
	-D__ACE__ -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DSOUNDS \
	-DCONIO \
	-clib=default \
	-DUDG_GRAPHICS \
	-DANIMATE_PLAYER \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o full -Cz--audio -create-app	
	cp full.wav $(BUILD_PATH)/FULL_ace_exp_16k_udg.wav
	rm full.wav
	rm full.tap
	rm full
	
	
ace_exp_16k:
	$(Z88DK_PATH)$(MYZ88DK) +ace $(SCCZ80_OPTS) \
	-D__ACE__ -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DSOUNDS \
	-DCONIO -DNO_INIT_GRAPHICS \
	-clib=ansi -o full -Cz--audio -create-app \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	cp full.wav $(BUILD_PATH)/FULL_ace_exp_16k.wav
	rm full.wav
	rm full.tap
	rm full


zx80_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 $(SCCZ80_OPTS) -vn \
	-D__ZX80__ -DFULL_GAME \
	-DEND_SCREEN \
	-DALT_SLEEP \
	-DCONIO \
	-DZX80_GEN_TV_FIELD \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx80_16k.prg \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/zx80/zx80_init_graphics.c \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_zx80_16k.prg	
	
zx80_16k_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 $(SCCZ80_OPTS) -vn \
	-D__ZX80__ -DFULL_GAME \
	-DEND_SCREEN \
	-DTURN_BASED \
	-DALT_SLEEP \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx80_16k_turn_based.prg \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_zx80_16k_turn_based.prg
	

zx81_16k_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-v \
	-D__ZX81__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DTURN_BASED \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_16k_turn_based.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_zx81_16k_turn_based.prg

	
zx81_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	$(ZSDCC_OPTS) \
	-compiler=sdcc \
	-vn \
	-D__ZX81__ -DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_16k.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_zx81_16k.prg
	
	
lambda_16k:
	$(Z88DK_PATH)$(MYZ88DK) +lambda $(SCCZ80_OPTS) \
	-vn -D__LAMBDA__ -DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o  $(BUILD_PATH)/FULL_lambda_16k.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_lambda_16k.prg		
	


# cpc:
	# $(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) -DANIMATE_PLAYER -v -clib=ansi \
	# -D__CPC__ -DSOUNDS -DFULL_GAME \
	# -DBETWEEN_LEVEL -DEND_SCREEN \
	# -DCPCRSLIB \
	# -pragma-define:REGISTER_SP=-1 \
	# -lndos -create-app -o 	$(BUILD_PATH)/FULL_cpc.prg \
	# $(CROSS_LIB_PATH)/display/graphics_data/z88dk/cpc_Chars8.asm \
	# $(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	# $(CROSS_LIB_PATH)/display/init_graphics/z88dk/cpc/cpc_cpcrslib_init_graphics.c \
	# $(SOURCE_PATH)/end_screen.c \
	# $(FULL_FILES)
	# $(TOOLS_PATH)/z88dk/cpc/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc.cpc  $(BUILD_PATH)/FULL_cpc.cdt
	# $(TOOLS_PATH)/z88dk/cpc/cpcxfsw -nd FULL_cpc.dsk
	# $(TOOLS_PATH)/z88dk/cpc/cpcxfsw FULL_cpc.dsk -p $(BUILD_PATH)/FULL_cpc.cpc xchase
	# mv FULL_cpc.dsk $(BUILD_PATH)/
	# rm $(BUILD_PATH)/FULL_cpc.cpc 
	# rm $(BUILD_PATH)/FULL_cpc.prg			

# cpc_udg:
	# $(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) \
	# -D__CPC__ \
	# -create-app -o $(BUILD_PATH)/FULL_cpc_udg.cpc -vn \
	# -DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	# -DCONIO \
	# -DFORCE_SCREEN_MODE=2 \
	# -lndos \
	# -DANIMATE_PLAYER \
	# -DUDG_GRAPHICS \
	# -clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	# $(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	# $(SOURCE_PATH)/end_screen.c \
	# $(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	# $(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	# $(FULL_FILES)	
	# $(TOOLS_PATH)/z88dk/cpc/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_udg.cpc  $(BUILD_PATH)/FULL_cpc_udg.cdt
	# $(TOOLS_PATH)/z88dk/cpc/cpcxfsw -nd FULL_cpc_udg.dsk
	# $(TOOLS_PATH)/z88dk/cpc/cpcxfsw FULL_cpc_udg.dsk -p $(BUILD_PATH)/FULL_cpc_udg.cpc xchase
	# mv FULL_cpc_udg.dsk $(BUILD_PATH)/
	# rm $(BUILD_PATH)/FULL_cpc_udg.cpc

cpc:
	$(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) -DANIMATE_PLAYER -vn  -clib=ansi \
	-D__CPC__ \
	-DZ88DK_JOYSTICK \
	-DSOUNDS -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCPCRSLIB \
	-pragma-define:REGISTER_SP=-1 \
	-lndos -create-app -o 	$(BUILD_PATH)/FULL_cpc.prg \
	-DCPCRSLIB_GRAPHICS \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/cpc_Chars8.asm \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/cpc/cpc_cpcrslib_init_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	$(TOOLS_PATH)/z88dk/cpc/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc.cpc  $(BUILD_PATH)/FULL_cpc.cdt
	rm -rf FULL_cpc.dsk
	$(TOOLS_PATH)/z88dk/cpc/cpcxfsw -nd FULL_cpc.dsk
	$(TOOLS_PATH)/z88dk/cpc/cpcxfsw FULL_cpc.dsk -p $(BUILD_PATH)/FULL_cpc.cpc xchase
	mv FULL_cpc.dsk $(BUILD_PATH)/
	cp $(TOOLS_PATH)/z88dk/cpc/nocart/*.rom .
	$(TOOLS_PATH)/z88dk/cpc/nocart/nocart.exe  $(BUILD_PATH)/FULL_cpc.dsk  $(BUILD_PATH)/FULL_gx4000.cpr   -c 'run"xchase'
	rm os.rom
	rm amsdos.rom
	rm basic.rom
	rm $(BUILD_PATH)/FULL_cpc.cpc 
	rm $(BUILD_PATH)/FULL_cpc.prg		

		
# msx_16k:
	# $(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) -zorg=49200 \
	# -DSOUNDS -DANIMATE_PLAYER -create-app -vn -DMSX_VPOKE -D__MSX__ -lndos \
	# -create-app -o $(BUILD_PATH)/LIGHT_msx_16k.prg \
	# -DALT_PRINT \
	# -DLESS_TEXT \
	# -DMEMORY_MAPPED \
	# $(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	# $(CROSS_LIB_PATH)/display/init_graphics/z88dk/msx/msx_redefined_chars_init_graphics.c \
	# $(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	# $(LIGHT_FILES)
	# rm $(BUILD_PATH)/LIGHT_msx_16k.prg 	


msx_32k:
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) \
	-DSOUNDS -DANIMATE_PLAYER \
	-vn -DMSX_VPOKE -DFULL_GAME -D__MSX__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_msx_32k.prg \
	-DMEMORY_MAPPED \
	-DALT_PRINT \
	-DLESS_TEXT \
	-DZ88DK_JOYSTICK \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/msx/msx_redefined_chars_init_graphics.c	\
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_msx_32k.prg 


msx_32k_rom:
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) \
	-DSOUNDS -DANIMATE_PLAYER \
	-vn -DMSX_VPOKE -DFULL_GAME -D__MSX__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -subtype=rom \
	-create-app -o $(BUILD_PATH)/FULL_msx_32k.rom \
	-DMEMORY_MAPPED \
	-DALT_PRINT \
	-DLESS_TEXT \
	-DZ88DK_JOYSTICK \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/msx/msx_redefined_chars_init_graphics.c	\
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_msx_32k_BSS.bin
	rm $(BUILD_PATH)/FULL_msx_32k_DATA.bin
	

svi_318_mode0:
	$(Z88DK_PATH)$(MYZ88DK) +svi $(SCCZ80_OPTS) -zorg=49200 -vn -lndos \
	-D__SVI__ -DMSX_MODE0 \
	-DSOUNDS \
	-create-app \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/svi/svi_graphics.c \
	$(LIGHT_FILES)
	mv a.cas $(BUILD_PATH)/LIGHT_svi_318_mode0.cas
	rm a.bin

#
svi_318:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	$(ZSDCC_OPTS) \
	-compiler=sdcc \
	-DFORCE_CONIO_LIB \
	-zorg=49152 \
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-vn -lndos \
	-D__SVI__ \
	-DSOUNDS \
	-DFORCE_BIT_BANG \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app \
	$(LIGHT_FILES)
	mv a.cas $(BUILD_PATH)/LIGHT_svi_318.cas
	rm a.bin


# -pragma-define:ansicolumns=32 -clib=ansi 
# 	-DCONIO_VT52 
#	-DFORCE_NO_CONIO_LIB 
svi_328_udg_cursor:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	-DZ88DK_JOYSTICK \
	-DUDG_GRAPHICS \
	-DANIMATE_PLAYER \
	-pragma-redirect:fputc_cons=fputc_cons_generic $(SCCZ80_OPTS) \
	 -vn -lndos \
	-DSOUNDS \
	-DFULL_GAME -D__SVI__ -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO \
	-DFORCE_CONIO_LIB \
	-pragma-define:ansicolumns=32 \
	-create-app \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c  \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.cas $(BUILD_PATH)/FULL_svi_328_udg.cas
	rm a.bin
	
svi_328:
	$(Z88DK_PATH)$(MYZ88DK) +svi -pragma-define:ansicolumns=32 -clib=ansi $(SCCZ80_OPTS) \
	-vn -lndos \
	-DSOUNDS \
	-DFULL_GAME -D__SVI__ -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DFORCE_CONIO_LIB \
	-create-app \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c  \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.cas $(BUILD_PATH)/FULL_svi_328.cas
	rm a.bin
	

sc3000_16k:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	$(SCCZ80_OPTS) \
	-DZ88DK_JOYSTICK \
	-clib=ansi \
	-D__SC3000__ \
	-DCONIO -DNO_INIT_GRAPHICS \
	-pragma-define:ansicolumns=32 \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/LIGHT_sc3000_16k.prg \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_sc3000_16k.prg
	rm $(BUILD_PATH)/LIGHT_sc3000_16k.tap

sc3000_32k_udg:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	$(SCCZ80_OPTS) \
	-D__SC3000__ \
	-DFULL_GAME \
	-DSOUNDS \
	-DFORCE_BIT_BANG \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DZ88DK_JOYSTICK \
	-DCONIO \
	-vn -lndos -create-app -Cz--audio \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	-o $(BUILD_PATH)/FULL_sc3000_32k_udg.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_sc3000_32k_udg.prg
	rm $(BUILD_PATH)/FULL_sc3000_32k_udg.tap	
	
sg1000_udg:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 -subtype=rom \
	$(SCCZ80_OPTS) \
	-D__SC3000__ \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME \
	-DSOUNDS \
	-DFORCE_BIT_BANG \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-vn -lndos -create-app \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	-o $(BUILD_PATH)/FULL_sg1000_udg.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_sg1000_udg.prg
	rm $(BUILD_PATH)/FULL_sg1000_udg_BSS.bin
	rm $(BUILD_PATH)/FULL_sg1000_udg_DATA.bin
	
	
sc3000_32k:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	$(SCCZ80_OPTS) \
	-DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-clib=ansi \
	-DCONIO -DNO_INIT_GRAPHICS \
	-pragma-define:ansicolumns=32 \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/FULL_sc3000_32k.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_sc3000_32k.prg
	rm $(BUILD_PATH)/FULL_sc3000_32k.tap	

	
sg1000:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 -subtype=rom \
	$(SCCZ80_OPTS) \
	-DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-vn -lndos -create-app \
	-o $(BUILD_PATH)/FULL_sg1000.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_sg1000.prg
	rm $(BUILD_PATH)/FULL_sg1000_BSS.bin
	rm $(BUILD_PATH)/FULL_sg1000_DATA.bin


mz:
	$(Z88DK_PATH)$(MYZ88DK) +mz $(SCCZ80_OPTS) \
	-D__MZ__ -clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_mz.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_mz.prg
	mv $(BUILD_PATH)/FULL_mz.mzt $(BUILD_PATH)/FULL_mz.mzf
	
microbee_16k:
	$(Z88DK_PATH)$(MYZ88DK) +bee $(SCCZ80_OPTS) \
	-D__BEE__ -clib=ansi -vn -DSOUNDS  \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_microbee_16k.prg  \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_microbee_16k.prg
	
microbee_32k:
	$(Z88DK_PATH)$(MYZ88DK) +bee $(SCCZ80_OPTS) \
	-D__BEE__ \
	-clib=ansi \
	-vn \
	-DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_microbee_32k.prg  \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_microbee_32k.prg

microbee_32k_wav:
	$(Z88DK_PATH)$(MYZ88DK) +bee $(SCCZ80_OPTS) -subtype=wav \
	-D__BEE__ \
	-clib=ansi \
	-vn \
	-DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_microbee_32k.prg  \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_microbee_32k.prg	
	
	
# import as data into ram at 32768 - call 32768
samcoupe:
	$(Z88DK_PATH)$(MYZ88DK) +sam $(SCCZ80_OPTS) \
	-D__SAM__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DFULL_GAME  -o $(BUILD_PATH)/FULL_samcoupe.bin -lndos \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	cp $(TOOLS_PATH)/z88dk/samcoupe/samdos2_empty $(TOOLS_PATH)/z88dk/samcoupe/samdos2
	$(TOOLS_PATH)/z88dk/samcoupe/pyz80.py -I $(TOOLS_PATH)/z88dk/samcoupe/samdos2 $(TOOLS_PATH)/z88dk/samcoupe/sam_wrapper.asm
	mv $(TOOLS_PATH)/z88dk/samcoupe/sam_wrapper.dsk $(BUILD_PATH)/FULL_samcoupe.dsk
	rm $(BUILD_PATH)/FULL_samcoupe.bin

# 	-clib=ansi -pragma-define:ansicolumns=32 
mtx_udg:
	$(Z88DK_PATH)$(MYZ88DK) +mtx -startup=2 $(SCCZ80_OPTS) \
	-D__MTX__ \
	-create-app -o FULL.bin -vn \
	-DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm FULL.bin
	mv FULL.wav $(BUILD_PATH)/FULL_mtx_udg.wav
	mv FULL $(BUILD_PATH)/FULL_mtx_udg.mtx
	
mtx:
	$(Z88DK_PATH)$(MYZ88DK) +mtx -startup=2 $(SCCZ80_OPTS) \
	-D__MTX__ -clib=ansi -pragma-define:ansicolumns=32 -create-app  \
	-DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm a.bin
	mv a.wav $(BUILD_PATH)/
	mv a $(BUILD_PATH)/a.mtx
	
# mtx:
	# $(Z88DK_PATH)$(MYZ88DK) +mtx -startup=2 $(SCCZ80_OPTS) \
	# -D__MTX__ -clib=ansi -pragma-define:ansicolumns=32 -create-app -o FULL.bin -vn \
	# -DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	# -DCONIO -DNO_INIT_GRAPHICS \
	# -lndos \
	# $(SOURCE_PATH)/end_screen.c \
	# $(FULL_FILES)
	# rm FULL.bin
	# mv FULL.wav $(BUILD_PATH)/FULL_mtx.wav
	# mv FULL $(BUILD_PATH)/FULL_mtx.mtx
	
abc80_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +abc80 -lm -subtype=hex -zorg=49200 $(SCCZ80_OPTS) \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DTINY_GAME \
	-DNO_WAIT \
	-D__ABC80__ -clib=ansi -vn  -lndos -create-app -o a \
	$(TINY_FILES)
	#rm a
	mv a.ihx $(BUILD_PATH)/TINY_abc80.ihx 
	
abc80_light:
	$(Z88DK_PATH)$(MYZ88DK) +abc80 -lm \
	-subtype=hex \
	-zorg=49200 $(SCCZ80_OPTS) \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DNO_WAIT \
	-D__ABC80__ -clib=ansi -vn  -lndos -create-app  \
	$(LIGHT_FILES)
	mv a.bin $(BUILD_PATH)/LIGHT_abc80.bin
	mv a.ihx $(BUILD_PATH)/LIGHT_abc80.ihx 
	
abc800_light:
	$(Z88DK_PATH)$(MYZ88DK) +abc800 -lm \
	-zorg=49200 $(SCCZ80_OPTS) \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DCONIO_VT100 \
	-DNO_WAIT \
	-D__ABC80__ -vn  -lndos  \
	$(LIGHT_FILES)
	$(TOOLS_PATH)/z88dk/abc80/bin2bac2$(COMPILEDEXT) -800 a.bin $(BUILD_PATH)/LIGHT_ABC800.bac 49200	
	mv a.bin $(BUILD_PATH)/LIGHT_abc800.bin 
	

abc80:
	$(Z88DK_PATH)$(MYZ88DK) +abc80 -lm -subtype=hex -zorg=49200 \
	$(SCCZ80_OPTS) \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-D__ABC80__ -clib=ansi -vn -DFULL_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DLESS_TEXT \
	-DSIMPLE_STRATEGY \
	-DNO_BLINKING \
	-DNO_HINTS \
	-DNO_WAIT \
	-DNO_BORDER \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_RANDOM_LEVEL \
	-DNO_SET_SCREEN_COLORS \
	-DNO_SLEEP \
	-DFLAT_ENEMIES \
	-lndos -create-app \
	$(FULL_FILES)
	$(TOOLS_PATH)/z88dk/abc80/bin2bac2$(COMPILEDEXT) a.bin $(BUILD_PATH)/xchase.bac 49200
	$(TOOLS_PATH)/z88dk/abc80/dosgen$(COMPILEDEXT) $(BUILD_PATH)/FULL_abc80.dsk 
	$(TOOLS_PATH)/z88dk/abc80/doscopy$(COMPILEDEXT) $(BUILD_PATH)/FULL_abc80.dsk -b $(BUILD_PATH)/xchase.bac
	rm a.bin 
	mv $(BUILD_PATH)/xchase.bac $(BUILD_PATH)/FULL_abc80.bac
	mv a.ihx $(BUILD_PATH)/FULL_abc80.ihx 
	
	
abc800:
	$(Z88DK_PATH)$(MYZ88DK) +abc800 -lm -subtype=hex -zorg=49200 \
	$(SCCZ80_OPTS) \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-DFORCE_CONIO_LIB \
	-D__ABC800__  -vn -DFULL_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DLESS_TEXT \
	-DSIMPLE_STRATEGY \
	-DNO_BLINKING \
	-DNO_HINTS \
	-DNO_WAIT \
	-DNO_BORDER \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_RANDOM_LEVEL \
	-DNO_SET_SCREEN_COLORS \
	-DNO_SLEEP \
	-DFLAT_ENEMIES \
	-lndos -create-app \
	$(FULL_FILES)
	$(TOOLS_PATH)/z88dk/abc80/bin2bac2$(COMPILEDEXT) a.bin $(BUILD_PATH)/xchase.bac 49200
	$(TOOLS_PATH)/z88dk/abc80/dosgen$(COMPILEDEXT) $(BUILD_PATH)/FULL_abc800.dsk 
	$(TOOLS_PATH)/z88dk/abc80/doscopy$(COMPILEDEXT) $(BUILD_PATH)/FULL_abc800.dsk -b $(BUILD_PATH)/xchase.bac
	rm a.bin 
	rm $(BUILD_PATH)/xchase.bac
	mv a.ihx $(BUILD_PATH)/FULL_abc800.ihx 	
# zsdcc is producing wrong binaries

	
abc800_32k:
	$(Z88DK_PATH)$(MYZ88DK) +abc800 -lm  $(SCCZ80_OPTS) \
	-D__ABC80__ -vn -DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DFORCE_CONIO_LIB -DNO_INIT_GRAPHICS \
	-lndos \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	
p2000_16k:
	$(Z88DK_PATH)$(MYZ88DK) +p2000 $(SCCZ80_OPTS) -clib=ansi -D__P2000__ -vn \
	-DSOUNDS  \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_p2000.prg \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_p2000.prg


p2000_32k:
	$(Z88DK_PATH)$(MYZ88DK) +p2000 $(SCCZ80_OPTS) -clib=ansi -D__P2000__ -vn \
	-DFULL_GAME -DSOUNDS  -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_p2000.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_p2000.prg	
	

z9001_32k:
	$(Z88DK_PATH)$(MYZ88DK) +z9001 $(SCCZ80_OPTS) -clib=ansi \
	-D__Z9001__ -vn -DFULL_GAME  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_z9001.z80 \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_z9001.z80	

	
z9001_16k:
	$(Z88DK_PATH)$(MYZ88DK) +z9001 $(SCCZ80_OPTS) -clib=ansi \
	-D__Z9001__ -vn   \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_z9001.z80 \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_z9001.z80	


mc1000_48k_joystick_udg:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 $(SCCZ80_OPTS) \
	-subtype=gaming \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME  \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-DSOUND \
	-D__MC1000__ -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-vn  -lndos -create-app -Cz--audio \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.wav $(BUILD_PATH)/FULL_mc1000_48k_joystick_udg.wav
	mv a.bin $(BUILD_PATH)/FULL_mc1000_48k_joystick_udg.bin
	rm a.cas	
	
# 	-pragma-define:ansicolumns=32
mc1000_48k_udg_joystick:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 $(SCCZ80_OPTS) \
	-subtype=gaming \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME  \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-DSOUND \
	-D__MC1000__ -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-vn  -lndos -create-app -Cz--audio \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.wav $(BUILD_PATH)/FULL_mc1000_48k_udg_joystick.wav
	mv a.bin $(BUILD_PATH)/FULL_mc1000_48k_udg_joystick.bin
	rm a.cas

mc1000_48k_udg:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 $(SCCZ80_OPTS) \
	-subtype=gaming \
	-DFULL_GAME  \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-DSOUND \
	-D__MC1000__ -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-vn  -lndos -create-app -Cz--audio \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.wav $(BUILD_PATH)/FULL_mc1000_48k_udg.wav
	mv a.bin $(BUILD_PATH)/FULL_mc1000_48k_udg.bin
	rm a.cas	
	
mc1000_48k:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 $(SCCZ80_OPTS) \
	-subtype=gaming -pragma-define:ansicolumns=32 \
	-DFULL_GAME  \
	-clib=ansi \
	-D__MC1000__ -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-vn  -lndos -create-app -Cz--audio \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.wav $(BUILD_PATH)/FULL_mc1000_48k.wav
	rm a.bin
	rm a.cas		


# 	$(ZSDCC_OPTS)	
mc1000_16k_full:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 -compiler=sdcc $(ZSDCC_OPTS) \
	-subtype=gaming -pragma-define:ansicolumns=32 \
	-DFULL_GAME  \
	-DLESS_TEXT \
	-DNO_BLINKING \
	-DNO_HINTS \
	-clib=ansi \
	-D__MC1000__ -DSOUNDS \
	-DEND_SCREEN \
	-DALT_SLEEP \
	-DCONIO -DNO_INIT_GRAPHICS \
	-vn  -lndos -create-app -Cz--audio \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.wav $(BUILD_PATH)/FULL_mc1000_16k_less_text.wav
	rm a.bin
	rm a.cas

gal_22k:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-pragma-need=ansiterminal \
	-D__GAL__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-vn -lndos -create-app -o  $(BUILD_PATH)/FULL_galaksija.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_galaksija.prg	
	rm $(BUILD_PATH)/FULL_galaksija.wav
	

# -DLESS_TEXT -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL 
# $(ZSDCC_OPTS)
# spectrum_16k:
	# $(Z88DK_PATH)$(MYZ88DK) +zx --opt-code-size  -startup=1 -zorg=24055 \
	# -pragma-include:$(CFG_PATH)/z88dk/zpragma.inc -clib=sdcc_iy \
	# -DNO_SLEEP -DLESS_TEXT -DTINY_GAME -v  -D__SPECTRUM__ \
	# -create-app -o $(BUILD_PATH)/TINY_spectrum_16k.prg \
	# -DCONIO -DNO_INIT_GRAPHICS \
	# $(TINY_FILES)
	# rm $(BUILD_PATH)/TINY_spectrum_16k_CODE.bin 
	# rm $(BUILD_PATH)/TINY_spectrum_16k_UNASSIGNED.bin

spectrum_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx $(SCCZ80_OPTS) -v -zorg=24055 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-pragma-redirect:getk=getk_inkey \
	-DZ88DK_JOYSTICK \
	-DTINY_GAME \
	-DFORCE_NARROW \
	-DANIMATE_PLAYER \
	-D__SPECTRUM__ \
	-lndos -create-app \
	-DCONIO \
	-DUDG_GRAPHICS \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	-o $(BUILD_PATH)/TINY_spectrum_16k.prg \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_spectrum_16k.prg
	rm $(BUILD_PATH)/TINY_spectrum_16k_BANK_7.bin		
	
spectrum_16k_light:
	$(Z88DK_PATH)$(MYZ88DK) +zx \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	--max-allocs-per-node200000 -SO3 --opt-code-size \
	-v -zorg=24055 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-pragma-redirect:getk=getk_inkey \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_HINTS \
	-DSIMPLE_STRATEGY \
	-DANIMATE_PLAYER \
	-D__SPECTRUM__ \
	-DALT_MOVE \
	-lndos -create-app \
	-DCONIO \
	-DUDG_GRAPHICS \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	-o $(BUILD_PATH)/LIGHT_spectrum_16k.prg \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(LIGHT_FILES)
	# rm $(BUILD_PATH)/LIGHT_spectrum_16k.prg
	# rm $(BUILD_PATH)/LIGHT_spectrum_16k_BANK_7.bin			
	
	
# -pragma-redirect:ansifont=_font_8x8_zx_system -pragma-define:ansifont_is_packed=0
spectrum_48k:
	$(Z88DK_PATH)$(MYZ88DK) +zx $(SCCZ80_OPTS) -v  \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-pragma-redirect:getk=getk_inkey \
	-DFULL_GAME \
	-DZ88DK_JOYSTICK \
	-DANIMATE_PLAYER \
	-DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-D__SPECTRUM__ \
	-lndos -create-app \
	-DCONIO \
	-DUDG_GRAPHICS \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	-o $(BUILD_PATH)/FULL_spectrum_48k.prg \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_spectrum_48k.prg
	rm $(BUILD_PATH)/FULL_spectrum_48k_BANK_7.bin	

	
spectrum_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +zx $(SCCZ80_OPTS) -vn  \
	-DTINY_GAME \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-pragma-redirect:getk=getk_inkey \
	-DBUFFERED \
	-DFORCE_XSIZE=29 \
	-DFORCE_YSIZE=19 \
	-DTURN_BASED \
	-DNO_INIT_GRAPHICS \
	-D__SPECTRUM__ \
	-lndos -create-app \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	-o $(BUILD_PATH)/TINY_spectrum_buffered.prg \
	$(TINY_FILES)	
	rm $(BUILD_PATH)/TINY_spectrum_buffered.prg
	rm $(BUILD_PATH)/TINY_spectrum_buffered_BANK_7.bin	
	
# spectrum_48k_joystick:
	# $(Z88DK_PATH)$(MYZ88DK) +zx $(SCCZ80_OPTS) -clib=ansi -v  \
	# -pragma-redirect:ansifont=_udg -pragma-define:ansifont_is_packed=0 -pragma-define:ansicolumns=32 \
	# -DFULL_GAME -DANIMATE_PLAYER -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL -D__SPECTRUM__ \
	# -DZ88DK_JOYSTICK \
	# -lndos -create-app \
	# -DCONIO \
	# -o $(BUILD_PATH)/FULL_spectrum_48k_joystick.prg \
	# $(CROSS_LIB_PATH)/display/init_graphics/z88dk/spectrum/udg.asm \
	# $(CROSS_LIB_PATH)/display/init_graphics/z88dk/spectrum/spectrum_redefined_chars_init_graphics.c \
	# $(SOURCE_PATH)/end_screen.c \
	# $(FULL_FILES)
	# rm $(BUILD_PATH)/FULL_spectrum_48k_joystick.prg
	# rm $(BUILD_PATH)/FULL_spectrum_48k_joystick_BANK_7.bin		
	
# spectrum_48k_putc4x6:
	# $(Z88DK_PATH)$(MYZ88DK) +zx $(SCCZ80_OPTS) -clib=ansi -vn  \
	# -pragma-redirect:ansifont=_udg -pragma-define:ansifont_is_packed=0 -pragma-define:ansicolumns=32 \
	# -DFULL_GAME -DANIMATE_PLAYER -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL -D__SPECTRUM__ \
	# -DZ88DK_PUTC4X6 \
	# -DNO_SLEEP -DLESS_TEXT -DNO_WAIT \
	# -lndos -create-app \
	# -o $(BUILD_PATH)/FULL_spectrum_48k_putc4x6.prg \
	# $(CROSS_LIB_PATH)/display/z88dk/spectrum/udg.asm \
	# $(CROSS_LIB_PATH)/display/z88dk/spectrum/spectrum_graphics.c \
	# $(SOURCE_PATH)/end_screen.c \
	# $(FULL_FILES) \
	# rm $(BUILD_PATH)/FULL_spectrum_48k_putc4x6.prg
	# rm $(BUILD_PATH)/FULL_spectrum_48k_putc4x6_BANK_7.bin	
	

pc6001_16k:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 $(SCCZ80_OPTS) -Cz--audio -clib=ansi \
	-D__PC6001__ -v  \
	-DALT_SLEEP \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_pc6001.prg  \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_pc6001.prg
	rm $(BUILD_PATH)/LIGHT_pc6001.wav
	mv $(BUILD_PATH)/LIGHT_pc6001.cas $(BUILD_PATH)/LIGHT_pc6001.cp6	

	
# Warning at file 'c:/z88dk/\lib\pc6001_crt0.asm' line 112: integer '66384' out of range
# Warning at file 'stdio/ansi/pc6001/f_ansi_char.asm' line 46: integer '66657' out of range
#  -DEND_SCREEN -DBETWEEN_LEVEL
# 	$(SOURCE_PATH)/end_screen.c 
#  -DSOUNDS
# 	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c 

pc6001_32k:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 $(SCCZ80_OPTS) -Cz--audio -clib=ansi -subtype=32k \
	-D__PC6001__ -v \
	-DFULL_GAME \
	-DALT_SLEEP \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_pc6001_32k.prg \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_pc6001_32k.prg
	rm $(BUILD_PATH)/FULL_pc6001_32k.wav
	mv $(BUILD_PATH)/FULL_pc6001_32k.cas $(BUILD_PATH)/FULL_pc6001_32k.cp6
	
	
	
pc6001_32k_udg:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 $(SCCZ80_OPTS) -Cz--audio -clib=ansi -subtype=32k \
	-D__PC6001__ -v -DFULL_GAME \
	-DCONIO \
	-DANIMATE_PLAYER -DUDG_GRAPHICS \
	-DFORCE_SCREEN_MODE=2 \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	-lndos -create-app -o $(BUILD_PATH)/FULL_pc6001_32k_udg.prg \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_pc6001_32k_udg.prg
	rm $(BUILD_PATH)/FULL_pc6001_32k_udg.wav
	mv $(BUILD_PATH)/FULL_pc6001_32k_udg.cas $(BUILD_PATH)/FULL_pc6001_32k_udg.cp6
	

	
# kbhit KO
# Everything displayed on the same line
nascom_32k_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +nascom $(SCCZ80_OPTS) -clib=ansi -vn -lndos \
	-D__NASCOM__ \
	-DTURN_BASED \
	-DSOUNDS \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_nascom_32k_turn_based.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_nascom_32k_turn_based.prg
	


nascom_32k:
	$(Z88DK_PATH)$(MYZ88DK) +nascom $(SCCZ80_OPTS) -clib=ansi -vn -lndos \
	-D__NASCOM__ \
	-DSOUNDS \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_nascom_32k.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_nascom_32k.prg
	
	
# -DSOUNDS
# -pragma-define:ansicolumns=32  -Cz-audio 
nascom_16k:
	$(Z88DK_PATH)$(MYZ88DK) +nascom $(SCCZ80_OPTS) -clib=ansi -vn -lndos \
	-D__NASCOM__ \
	-DSOUNDS \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o $(BUILD_PATH)/LIGHT_nascom_16k.prg \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_nascom_16k.prg
	

z1013:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 $(SCCZ80_OPTS) -clib=ansi \
	-vn -lndos \
	-D__Z1013__  -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-create-app
	mv $(SOURCE_PATH)/../A.Z80 $(BUILD_PATH)/FULL_z1013.z80
	rm $(SOURCE_PATH)/../a.bin	


px8_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px32k \
	-D__PX8__ \
	-DCONIO_ADM3A \
	-DTINY_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app \
	$(TINY_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/TINY_px8.ROM

px8_chars:	
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px32k \
	$(SCCZ80_OPTS) \
	-D__PX8__ \
	-DCONIO_ADM3A \
	-DFULL_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DALT_CLEAR_SCREEN \
	-create-app \
	$(FULL_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px8_chars.ROM	
	# cp $(BUILD_PATH)/FULL_px8_chars.ROM /cygdrive/c/Emulation/TEST/PX-8/px8emul/BASIC.ROM
	

px8:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	$(SCCZ80_OPTS) \
	-D__PX8__ \
	-DFULL_GAME \
	-subtype=px32k  -Cz--32k \
	-create-app \
	-vn -lndos \
	-DALT_PRINT \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px8.ROM
	# cp $(BUILD_PATH)/FULL_px8.ROM /cygdrive/c/Emulation/TEST/PX-8/px8emul/BASIC.ROM


px4_tiny_putc4x6:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-pragma-define:ansicolumns=40 \
	-pragma-define:ansipixels=240 -pragma-define:ansicolumns=60 \
 	-subtype=px4ansi \
	-D__PX4__ \
	-DTINY_GAME \
	-DNO_WAIT \
	-DZ88DK_PUTC4X6 \
	-DCONIO -DNO_INIT_GRAPHICS \
	-Cz–-32k \
	-create-app \
	-vn -lndos \
	$(TINY_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/TINY_px4_putc4x6.ROM


px4_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-D__PX4__ \
	-DTINY_GAME \
	-subtype=px4ansi -Cz–-32k \
	-pragma-define:ansicolumns=40 \
	-create-app \
	-vn -lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/TINY_px4.ROM


oz_tiny_buffered: 	
	$(Z88DK_PATH)$(MYZ88DK) +oz \
	-compiler=sdcc \
	-SO3 \
	-D__OZ__ \
	-DTINY_GAME \
	-DFORCE_XSIZE=59 \
	-DFORCE_YSIZE=9 \
	-DFORCE_NARROW \
	-vn -lndos \
	-DBUFFERED -DTURN_BASED -DNO_INIT_GRAPHICS \
	-DLESS_TEXT \
	-DNO_COLOR \
	-DSIMPLE_STRATEGY \
	-DNO_SET_SCREEN_COLORS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c	
	$(TOOLS_PATH)/z88dk/oz/makewzd.exe a
	rm a.bin
	mv a.wzd $(BUILD_PATH)/TINY_oz_buffered.wzd
	
	
oz_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +oz \
	$(SCCZ80_OPTS) \
	-D__OZ__ \
	-DTINY_GAME \
	-vn -lndos \
	-DFORCE_XSIZE=59 \
	-DFORCE_YSIZE=9 \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DNO_COLOR \
	-DNO_RANDOM_LEVEL \
	-DSIMPLE_STRATEGY \
	-DALT_MOVE \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_SET_SCREEN_COLORS \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DALT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	$(TOOLS_PATH)/z88dk/oz/makewzd.exe a
	rm a.bin
	mv a.wzd $(BUILD_PATH)/TINY_oz.wzd
	
	
# it fails with sdcc
z88_tiny_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +z88 -subtype=app \
	-SO3 \
	-D__Z88__ \
	-DTINY_GAME \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=6 \
	-DFORCE_NARROW \
	-vn -lndos \
	-DBUFFERED -DTURN_BASED \
	-DLESS_TEXT \
	-DNO_COLOR \
	-DSIMPLE_STRATEGY \
	-DNO_SET_SCREEN_COLORS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/z88/z88_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES) \
	-create-app
	rm a.bin
	rm a_BSS.bin
	rm a_DATA.bin
	rm a_bss_fardata.bin
	mv a.63 $(BUILD_PATH)/TINY_z88_buffered.63
	mv a.epr $(BUILD_PATH)/TINY_z88_buffered.epr
		
	
z88_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +z88 -subtype=app \
	-D__Z88__ \
	-DTINY_GAME \
	-vn -lndos \
	-DALT_PRINT \
	-DZ88DK_SPRITES \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DFORCE_BOMBS_NUMBER=2 \
	-DFORCE_GHOSTS_NUMBER=6 \
	-DALT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/z88/z88_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES) \
	-create-app
	rm a.bin
	rm a_BSS.bin
	rm a_DATA.bin
	mv a.63 $(BUILD_PATH)/TINY_z88.63
	mv a.epr $(BUILD_PATH)/TINY_z88.epr
	
z88:
	$(Z88DK_PATH)$(MYZ88DK) +z88 -subtype=app \
	-D__Z88__ \
	-DFULL_GAME \
	-vn -lndos \
	-DALT_PRINT \
	-DZ88DK_SPRITES \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DALT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/z88/z88_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-create-app
	rm a.bin
	rm a_BSS.bin
	rm a_DATA.bin
	rm a_bss_fardata.bin
	mv a.63 $(BUILD_PATH)/FULL_z88.63
	mv a.epr $(BUILD_PATH)/FULL_z88.epr
	

	
px4:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-D__PX4__ \
	-DFULL_GAME \
	-subtype=px4 -Cz--32k \
	-create-app \
	-vn -lndos \
	-DALT_PRINT \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px4.ROM		
	#cp $(BUILD_PATH)/FULL_px4.ROM /cygdrive/c/Emulation/TEST/PX-4/hc40emulator/
	
px4_chars:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-D__PX4__ \
	-DFULL_GAME \
	-subtype=px4ansi -Cz–-32k \
	-pragma-define:ansicolumns=40 \
	-create-app \
	-vn -lndos \
	-DNO_PRINT \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(FULL_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px4.ROM	

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
	-DNO_PRINT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-Cz–-32k \
	-create-app \
	-vn -lndos \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px4_putc4x6.ROM


x1_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +x1 \
	-D__X1__ \
	-DTINY_GAME -DNO_SLEEP \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o $(BUILD_PATH)/TINY_x1.bin -vn -lndos \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_x1.bin
	
	
x1:
	$(Z88DK_PATH)$(MYZ88DK) +x1 \
	-D__X1__ \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o $(BUILD_PATH)/FULL_x1.bin -vn -lndos \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_x1.bin


# -subtype=disk
trs80_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	-pragma-redirect:getk=getk_inkey \
	-lm -create-app \
	-D__TRS80__ \
	-DCONIO_VT52 \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-DCONIO -DNO_INIT_GRAPHICS \
	-o$(BUILD_PATH)/TINY_trs80.bin \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_trs80.bin
	

trs80_chars:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	-pragma-redirect:getk=getk_inkey \
	-pragma-define:REGISTER_SP=0xffb5 \
	-lm -create-app \
	-D__TRS80__ \
	-DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO_VT52 \
	-DCONIO -DNO_INIT_GRAPHICS \
	-o$(BUILD_PATH)/FULL_trs80_chars.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_trs80_chars.bin	


trs80:	
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	$(SCCZ80_OPTS) \
	-pragma-redirect:getk=getk_inkey \
	-pragma-define:REGISTER_SP=0xffb5 \
	-D__TRS80__ \
	-DFULL_GAME \
	-vn -lndos \
	-DALT_PRINT \
	-DFORCE_NARROW \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-create-app \
	-DBETWEEN_LEVEL \
	-o$(BUILD_PATH)/FULL_trs80.bin \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_trs80.bin	
	
	
trs80_wav:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos -subtype=wav \
	-pragma-redirect:getk=getk_inkey \
	-pragma-define:REGISTER_SP=0xffb5 \
	-lm -create-app \
	-D__TRS80__ \
	-DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO_VT52 \
	-DCONIO -DNO_INIT_GRAPHICS \
	-o$(BUILD_PATH)/FULL_trs80.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_trs80.bin	
	
trs80_cmd:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos -subtype=disk \
	-lm -create-app \
	-D__TRS80__ \
	-pragma-redirect:getk=getk_inkey \
	-DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO_VT52 \
	-DCONIO -DNO_INIT_GRAPHICS \
	-o$(BUILD_PATH)/FULL_trs80.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_trs80.bin	
	
eg2k:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 \
	-D__EG2K__ \
	-subtype=eg2000disk \
	-lndos \
	-lm \
	-D__TRS80__ \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO_VT52 \
	-create-app \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.cmd $(BUILD_PATH)/FULL_eg2k.cmd
	rm a.bin



eg2k_udg:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 \
	-D__EG2K__ \
	-subtype=eg2000disk \
	-pragma-redirect:getk=getk_inkey \
	$(SCCZ80_OPTS) \
	-D__EG2K__ \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-DLESS_TEXT \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_eg2k_udg.bin
	rm $(BUILD_PATH)/FULL_eg2k_udg.bin	


tiki100_sprites_light:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -ltiki100 \
	-D__TIKI100__ \
	-DNO_PRINT \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DANIMATE_PLAYER \
	-create-app -o$(BUILD_PATH)/LIGHT_tiki100_sprites.bin \
	-DNO_INIT_GRAPHICS \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_tiki100_sprites.bin	

	

tiki100: 
	$(Z88DK_PATH)$(MYZ88DK) +cpm -startup=2 -lm -ltiki100 \
	$(SCCZ80_OPTS) \
	-D__TIKI100__ \
	-DNO_PRINT \
	-DFULL_GAME \
	-DNO_COLOR \
	-DALT_MOVE \
	-DNO_RANDOM_LEVEL \
	-DNO_SLEEP \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DNO_HINTS \
	-DNO_SET_SCREEN_COLORS \
	-DANIMATE_PLAYER \
	-create-app \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES)
	cp $(TOOLS_PATH)/z88dk/tiki100/empty.dsk $(BUILD_PATH)/FULL_tiki100.dsk
	$(TOOLS_PATH)/z88dk/tiki100/tikidisk$(COMPILEDEXT) $(BUILD_PATH)/FULL_tiki100.dsk a A.COM
	rm A.COM
	rm a.bin
	
kaypro: 
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lgfxkp \
	-D__KAYPRO__ \
	-DNO_PRINT \
	-DFULL_GAME \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DANIMATE_PLAYER \
	-create-app -o$(BUILD_PATH)/FULL_kaypro.bin \
	-DNO_INIT_GRAPHICS \
	-DINITIAL_GHOST_FREEZE \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_kaypro.bin

	# -DEND_SCREEN -DBETWEEN_LEVEL 
	# $(SOURCE_PATH)/end_screen.c 


alphatro:
	$(Z88DK_PATH)$(MYZ88DK) +alphatro $(SCCZ80_OPTS) \
	-D__ALPHATRO__ \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-lndos \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_alphatro.bin
	rm $(BUILD_PATH)/FULL_alphatro.bin
	rm $(BUILD_PATH)/FULL_alphatro_DATA.bin
	rm $(BUILD_PATH)/FULL_alphatro_BSS.bin	


microbee_sprites:
	$(Z88DK_PATH)$(MYZ88DK) +bee -lgfxnc100 \
	-D__NC100__ \
	-DALT_PRINT \
	-DFULL_GAME \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DANIMATE_PLAYER \
	-create-app -o$(BUILD_PATH)/FULL_microbee_sprites.bin \
	-DNO_INIT_GRAPHICS \
	-DINITIAL_GHOST_FREEZE \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_microbee_sprites.bin
	
nc100:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc100 \
	-D__NC100__ \
	-DALT_PRINT \
	-DFULL_GAME \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DANIMATE_PLAYER \
	-create-app -o$(BUILD_PATH)/FULL_nc100_alt_print.bin \
	-DNO_INIT_GRAPHICS \
	-DINITIAL_GHOST_FREEZE \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_nc100_alt_print.bin
	# cp $(BUILD_PATH)/FULL_nc100_alt_print.rom /cygdrive/c/Emulation/TEST/nc100em-1.2-2014/nc100.card
	# rm -f /cygdrive/c/Emulation/TEST/nc100em-1.2-2014/nc100.ram


nc200:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc200 \
	-D__NC200__ \
	-DALT_PRINT \
	-DFULL_GAME \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DANIMATE_PLAYER \
	-create-app -o$(BUILD_PATH)/FULL_nc200_alt_print.bin \
	-DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_nc200_alt_print.bin
	# cp $(BUILD_PATH)/FULL_nc200_alt_print.rom /cygdrive/c/Emulation/TEST/nc100em-1.2-2014/nc200.card
	# rm -f /cygdrive/c/Emulation/TEST/nc100em-1.2-2014/nc200.ram	
	

attache_chars:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lgfxattache -subtype=attache \
	-DCONIO_ADM3A \
	-D__CPM_80X24__ \
	-DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_attache_chars.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_attache_chars.bin

attache:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lgfxattache -subtype=attache \
	$(SCCZ80_OPTS) \
	-D__ATTACHE__ \
	-DFULL_GAME \
	-DALT_PRINT \
	-DFULL_GAME \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DANIMATE_PLAYER \
	-DFORCE_GHOSTS_NUMBER=8 \
	-create-app -o$(BUILD_PATH)/FULL_attache.bin \
	-DNO_INIT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)	
	rm $(BUILD_PATH)/FULL_attache.bin


aussie:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -laussie  \
	-D__AUSSIE__ \
	-DCONIO_ADM3A \
	-D__CPM_80X24__ \
	-DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_aussie.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_aussie.bin

	
# -subtype=osborne1
osborne1_chars:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lgfxosborne1 -subtype=osborne1 \
	-DCONIO_ADM3A \
	-D__CPM_80X24__ \
	-DFULL_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_osborne1_chars.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_osborne1_chars.bin

osborne1:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lgfxosborne1 -subtype=osborne1 \
	-D__OSBORNE1__ \
	-DFULL_GAME -DLESS_TEXT \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DALT_PRINT \
	-create-app -o$(BUILD_PATH)/FULL_osborne1.bin \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	rm $(BUILD_PATH)/FULL_osborne1.bin	
	
	
cpm_adm3a:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-DCONIO_ADM3A \
	-D__CPM_80X24__ \
	-DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_cpm_adm3a.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_cpm_adm3a.bin

	
sos_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +sos \
	$(SCCZ80_OPTS) \
	-lndos \
	-DSTDLIB \
	-DNO_COLOR \
	-DBUFFERED -DALT_PRINT \
	-DTURN_BASED \
	-DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DNO_INIT_GRAPHICS \
	-DALT_CLEAR_SCREEN \
	-create-app \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	rm a.bin
	mv a.obj $(BUILD_PATH)/FULL_sos_buffered.obj
	
sos_tiny_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +sos \
	$(SCCZ80_OPTS) \
	-lndos \
	-DSTDLIB \
	-DNO_COLOR \
	-DBUFFERED -DALT_PRINT \
	-DTURN_BASED \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DNO_INIT_GRAPHICS \
	-DALT_CLEAR_SCREEN \
	-create-app \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES)
	rm a.bin
	mv a.obj $(BUILD_PATH)/TINY_sos_buffered.obj
		

	
	
c7420:
	$(Z88DK_PATH)$(MYZ88DK) +c7420 \
	$(SCCZ80_OPTS) \
	-lndos \
	-DSTDLIB \
	-DNO_COLOR \
	-DBUFFERED -DALT_PRINT \
	-DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DNO_INIT_GRAPHICS \
	-create-app \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	rm a.bin
	mv _a.bas $(BUILD_PATH)/FULL_c7420.bas	


c7420_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +c7420 \
	$(SCCZ80_OPTS) \
	-lndos \
	-DSTDLIB \
	-DNO_COLOR \
	-DBUFFERED -DALT_PRINT \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DNO_INIT_GRAPHICS \
	-create-app \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES)
	rm a.bin
	mv _a.bas $(BUILD_PATH)/TINY_c7420.bas	
	
	
	
cpm_vt52:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-DCONIO_VT52 \
	-D__CPM_80X24__ \
	-DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_cpm_vt52.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_cpm_vt52.bin	
	

cpm_vt100:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-DCONIO_VT100 \
	-D__CPM_80X24__ \
	-DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_cpm_vt100.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_cpm_vt100.bin	
	
	

c128_z80_80col:
	$(Z88DK_PATH)$(MYZ88DK) +c128 -lgfx128hr \
	$(SCCZ80_OPTS) \
	-lndos -subtype=disk \
	-D__C128_Z80__ \
	-DZ88DK_SPRITES -DANIMATE_PLAYER \
	-DFULL_GAME -DEND_SCREEN \
	-DLESS_TEXT \
	-DSOUNDS \
	-DFORCE_BIT_BANG \
	-DZ88DK_JOYSTICK \
	-DSIMPLE_STRATEGY \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DALT_PRINT \
	-DNO_COLOR \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/c128/c128_init_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-create-app
	$(TOOLS_PATH)/generic/c1541 -format "crosschase,0" d64 FULL_c128_z80_80col.d64
	$(TOOLS_PATH)/generic/c1541 -attach FULL_c128_z80_80col.d64 -write a.ldr
	$(TOOLS_PATH)/generic/c1541 -attach FULL_c128_z80_80col.d64 -write a
	mv FULL_c128_z80_80col.d64 $(BUILD_PATH)/
	rm A.LDR
	rm A
	rm a.bin

c128_z80_40col:
	$(Z88DK_PATH)$(MYZ88DK) +c128 \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-lndos -subtype=disk \
	-D__C128_Z80__ -DFORCE_XSIZE=40 \
	-DFULL_GAME -DEND_SCREEN -DNO_BLINKING \
	-DFORCE_CONIO_LIB \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-create-app
	$(TOOLS_PATH)/generic/c1541 -format "crosschase,0" d64 FULL_c128_z80_40col.d64
	$(TOOLS_PATH)/generic/c1541 -attach FULL_c128_z80_40col.d64 -write a.ldr
	$(TOOLS_PATH)/generic/c1541 -attach FULL_c128_z80_40col.d64 -write a
	mv FULL_c128_z80_40col.d64 $(BUILD_PATH)/
	rm A.LDR
	rm A
	rm a.bin


c128_z80_40col_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +c128 $(SCCZ80_OPTS) \
	-lndos -subtype=disk \
	-D__C128_Z80__ -DFORCE_XSIZE=40 \
	-DFULL_GAME -DEND_SCREEN \
	-DTURN_BASED \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-create-app
	$(TOOLS_PATH)/generic/c1541 -format "crosschase,0" d64 FULL_c128_z80_40col_turn_based.d64
	$(TOOLS_PATH)/generic/c1541 -attach FULL_c128_z80_40col_turn_based.d64 -write a.ldr
	$(TOOLS_PATH)/generic/c1541 -attach FULL_c128_z80_40col_turn_based.d64 -write a
	mv FULL_c128_z80_40col_turn_based.d64 $(BUILD_PATH)/
	rm A.LDR
	rm A
	rm a.bin
	
	
# Opt
einstein:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=einstein -leinstein \
	-pragma-define:ansicolumns=32 \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-D__EINSTEIN__ \
	-DFORCE_CONIO_LIB \
	-DFULL_GAME -DLESS_TEXT \
	-clib=ansi \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o$(BUILD_PATH)/FULL_einstein.bin \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_einstein.bin


ti82_chars:
	$(Z88DK_PATH)$(MYZ88DK) +ti82 \
	-DZ88DK_JOYSTICK \
	$(SCCZ80_OPTS) -D__TI82__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-DLESS_TEXT -DSIMPLE_STRATEGY -DNO_HINTS -DNO_BLINKING \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app  \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../a.82p $(BUILD_PATH)/FULL_ti82_chars.82p
	rm $(SOURCE_PATH)/../a.bin

ti82:
	$(Z88DK_PATH)$(MYZ88DK) +ti82 \
	$(SCCZ80_OPTS) \
	-D__TI82__ \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME \
	-vn -lndos \
	-DALT_PRINT \
	-DFORCE_NARROW \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-create-app \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../a.82p $(BUILD_PATH)/FULL_ti82.82p
	rm $(SOURCE_PATH)/../a.bin
	# cp $(BUILD_PATH)/FULL_ti82.82p "/cygdrive/c/Emulation/TI Calc/vti/BACKED_UP_SHELLS/"
	
# ti82_tiny_turn_based:
	# $(Z88DK_PATH)$(MYZ88DK) +ti82 \
	# $(SCCZ80_OPTS) -D__TI82__ \
	# -DZ88DK_JOYSTICK \
	# -clib=ansi -pragma-define:ansicolumns=32 \
	# -vn \
	# -DTINY_GAME  \
	# -DTURN_BASED -DNO_WAIT \
	# -DLESS_TEXT -DSIMPLE_STRATEGY -DNO_HINTS -DNO_BLINKING \
	# -lndos \
	# -DCONIO -DNO_INIT_GRAPHICS \
	# $(TINY_FILES) \
	# -create-app
	# mv $(SOURCE_PATH)/../a.82p $(BUILD_PATH)/TINY_ti82_turn_based.82p
	# rm $(SOURCE_PATH)/../a.bin


ti83_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ti83 \
	$(SCCZ80_OPTS) -D__TI83__ \
	-DZ88DK_JOYSTICK \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-DTURN_BASED \
	-DNO_WAIT \
	-DLESS_TEXT -DNO_HINTS -DNO_BLINKING -DNO_COLOR \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app
	mv $(SOURCE_PATH)/../a.83p $(BUILD_PATH)/FULL_ti83_turn_based.83p
	rm $(SOURCE_PATH)/../a.bin

	
ti83_tiny_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ti83 \
	$(SCCZ80_OPTS) -D__TI83__ \
	-DZ88DK_JOYSTICK \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DTURN_BASED \
	-DNO_WAIT \
	-DTINY_GAME  \
	-DLESS_TEXT -DNO_HINTS -DNO_BLINKING -DNO_COLOR \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app \
	$(TINY_FILES)
	mv $(SOURCE_PATH)/../a.83p $(BUILD_PATH)/TINY_ti83_turn_based.83p	
	rm $(SOURCE_PATH)/../a.bin

ti83_chars:
	$(Z88DK_PATH)$(MYZ88DK) +ti83 \
	$(SCCZ80_OPTS) -D__TI83__ \
	-DZ88DK_JOYSTICK \
	-DFORCE_XSIZE=32 \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -oxchase \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../xchase.83p $(BUILD_PATH)/FULL_ti83_chars.83p	
	rm $(SOURCE_PATH)/../xchase
	# cp $(BUILD_PATH)/FULL_ti83.83p "/cygdrive/c/Emulation/TI Calc/vti/BACKED_UP_SHELLS/"
	
	
ti83:
	$(Z88DK_PATH)$(MYZ88DK) +ti83 \
	$(SCCZ80_OPTS) \
	-D__TI83__ \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME \
	-vn -lndos \
	-DALT_PRINT \
	-DFORCE_NARROW \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-create-app -oxchase \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../xchase.83p $(BUILD_PATH)/FULL_ti83.83p
	rm $(SOURCE_PATH)/../xchase	
	
ti8x:
	$(Z88DK_PATH)$(MYZ88DK) +ti8x -clib=ansi  -subtype=ion -Cz--altfmt \
	-D__TI8X__ \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME \
	-vn -lndos \
	-DALT_PRINT \
	-DFORCE_NARROW \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-create-app -oxchase \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../xchase.8xp $(BUILD_PATH)/FULL_ti8x.8xp	
	mv $(SOURCE_PATH)/../xchase $(BUILD_PATH)/FULL_ti8x.bin
	# rm $(SOURCE_PATH)/../xchase

	
ti86_light:
	$(Z88DK_PATH)$(MYZ88DK) +ti86 \
	$(SCCZ80_OPTS) \
	-D__TI86__ \
	-DZ88DK_JOYSTICK \
	-DLESS_TEXT	\
	-DNO_COLOR \
	-DNO_BLINKING \
	-DNO_HINTS \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app  \
	$(LIGHT_FILES)
	mv $(SOURCE_PATH)/../a.86p $(BUILD_PATH)/LIGHT_ti86.86p	
	rm $(SOURCE_PATH)/../a.bin	
	# cp $(BUILD_PATH)/LIGHT_ti86.86p "/cygdrive/c/Emulation/TI Calc/vti/BACKED_UP_SHELLS/"

	
ti85_chars:
	$(Z88DK_PATH)$(MYZ88DK) +ti85 \
	$(SCCZ80_OPTS) -D__TI85__ \
	-DZ88DK_JOYSTICK \
	-DFORCE_XSIZE=32 \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app  \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../a.85s $(BUILD_PATH)/FULL_ti85_chars.85s	
	rm $(SOURCE_PATH)/../a.bin

ti85:
	$(Z88DK_PATH)$(MYZ88DK) +ti85 \
	$(SCCZ80_OPTS) \
	-D__TI85__ \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME \
	-vn -lndos \
	-DALT_PRINT \
	-DFORCE_NARROW \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-create-app \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../a.85s $(BUILD_PATH)/FULL_ti85.85s
	rm $(SOURCE_PATH)/../a.bin
	# cp $(BUILD_PATH)/FULL_ti85.85s "/cygdrive/c/Emulation/TI Calc/vti/BACKED_UP_SHELLS/"
		
	
ti85_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ti85 \
	$(SCCZ80_OPTS) \
	-D__TI85__ \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME \
	-DTURN_BASED \
	-vn -lndos \
	-DALT_PRINT \
	-DFORCE_NARROW \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-create-app \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../a.85s $(BUILD_PATH)/FULL_ti85_turn_based.85s
	rm $(SOURCE_PATH)/../a.bin
	# cp $(BUILD_PATH)/FULL_ti85_turn_based.85s "/cygdrive/c/Emulation/TI Calc/vti/BACKED_UP_SHELLS/"
		
m5_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	-lm -create-app -Cz--audio -subtype=tape \
	-D__M5__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-DTINY_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-o$(BUILD_PATH)/TINY_m5.bin \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_m5.bin
	rm $(BUILD_PATH)/TINY_m5.cas


# 	-DNO_SLEEP -DNO_WAIT
m5_udg:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	-lm -create-app -Cz--audio -subtype=tape \
	-pragma-define:REGISTER_SP=65000 \
	-D__M5__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-DFULL_GAME -DLESS_TEXT \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO \
	-DSOUNDS \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o$(BUILD_PATH)/FULL_m5_udg.bin	
	rm $(BUILD_PATH)/FULL_m5_udg.bin
	rm $(BUILD_PATH)/FULL_m5_udg.cas
		
m5:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	-lm -create-app -Cz--audio -subtype=tape \
	-pragma-define:REGISTER_SP=65000 \
	-D__M5__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	-o$(BUILD_PATH)/FULL_m5.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_m5.bin
	rm $(BUILD_PATH)/FULL_m5.cas
	

srr_udg:
	$(Z88DK_PATH)$(MYZ88DK) +srr $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__SRR__ -vn \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL -DNO_WAIT \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	-create-app \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm a.srr
	mv a.wav $(BUILD_PATH)/FULL_srr_udg.wav

	
srr:
	$(Z88DK_PATH)$(MYZ88DK) +srr $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__SRR__ -vn \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL -DNO_WAIT \
	-DCONIO_VT52 \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm a.srr
	mv a.wav $(BUILD_PATH)/FULL_srr.wav


pv2000:
	$(Z88DK_PATH)$(MYZ88DK) +pv2000 $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__SRR__ -vn \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL -DNO_WAIT \
	-DCONIO_VT52 \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.rom $(BUILD_PATH)/FULL_pv2000.rom
	rm a.bin
	rm a_BSS.bin
	rm a_DATA.bin	
	
fp1100:
	$(Z88DK_PATH)$(MYZ88DK) +fp1100 $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__FP1100__ -vn \
	-DCONIO_VT52 \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-create-app
	mv a.d88 $(BUILD_PATH)/FULL_fp1100.d88
	rm a.bin
	rm a_BOOTSTRAP.bin
	

fp1100_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +fp1100 $(SCCZ80_OPTS) \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__FP1100__ -vn \
	-DCONIO_VT52 \
	-DTINY_GAME -DSOUNDS \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES) \
	-create-app
	mv a.d88 $(BUILD_PATH)/TINY_fp1100.d88
	rm a.bin
	rm a_BOOTSTRAP.bin

camputers_lynx:
	$(Z88DK_PATH)$(MYZ88DK) +lynx $(SCCZ80_OPTS) \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	-pragma-redirect:getk=getk_inkey \
	-D__CAMPUTERS_LYNX__ \
	-vn \
	-DCONIO_VT52 \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-create-app -o $(BUILD_PATH)/FULL_camputers_lynx.bin
	rm $(BUILD_PATH)/FULL_camputers_lynx.bin
	
camputers_lynx_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +lynx $(SCCZ80_OPTS) \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	-pragma-redirect:getk=getk_inkey \
	-D__CAMPUTERS_LYNX__ \
	-vn \
	-DCONIO_VT52 \
	-DNO_COLOR \
	-DTINY_GAME \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES) \
	-create-app -o $(BUILD_PATH)/TINY_camputers_lynx.bin
	rm $(BUILD_PATH)/TINY_camputers_lynx.bin	

newbrain_newline:
	$(Z88DK_PATH)$(MYZ88DK) +newbrain $(SCCZ80_OPTS) \
	-lndos \
	$(SOURCE_PATH)/../experiments/newline_test.c \
	-create-app
	tar czvf newline_test.tar.zip _dir.txt a.bas a.dat
	mv newline_test.tar.zip $(BUILD_PATH)
	rm _dir.txt
	rm a.bas
	rm a.dat
	rm a.bin	
	
newbrain_hello:
	$(Z88DK_PATH)$(MYZ88DK) +newbrain $(SCCZ80_OPTS) \
	-lndos \
	$(SOURCE_PATH)/../experiments/hello.c \
	-create-app
	tar czvf xchase.tar.zip _dir.txt a.bas a.dat
	rm _dir.txt
	rm a.bas
	rm a.dat
	rm a.bin
	
c7420_hello:
	$(Z88DK_PATH)$(MYZ88DK) +c7420 $(SCCZ80_OPTS) \
	-lndos \
	$(SOURCE_PATH)/../experiments/hello.c \
	-create-app	
	
oz_hello:
	$(Z88DK_PATH)$(MYZ88DK) +oz $(SCCZ80_OPTS) \
	$(SOURCE_PATH)/../experiments/hello.c 
	$(TOOLS_PATH)/z88dk/oz/makewzd.exe a
	rm a.bin
	mv a.wzd $(BUILD_PATH)/oz_hello.wzd
	

z88_hello:
	$(Z88DK_PATH)$(MYZ88DK) +z88 $(SCCZ80_OPTS) -D__Z88__ -create-app -subtype=app \
	$(SOURCE_PATH)/../experiments/hello.c
	rm a.bin
	rm a_BSS.bin
	rm a_DATA.bin
	mv a.63 $(BUILD_PATH)/
	mv a.epr $(BUILD_PATH)/
	
x07_hello:
	$(Z88DK_PATH)$(MYZ88DK) +x07 $(SCCZ80_OPTS) \
	-Cz--audio \
	$(SOURCE_PATH)/../experiments/hello.c \
	-create-app 

cpm_hello:
	$(Z88DK_PATH)$(MYZ88DK) +cpm $(SCCZ80_OPTS) \
	$(SOURCE_PATH)/../experiments/hello.c \
	-create-app
	rm a.bin
	mv A.COM $(BUILD_PATH)/CPMHELLO.COM
	
	
x07_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +x07 $(SCCZ80_OPTS) \
	-Cz--audio \
	-DFORCE_BOMBS_NUMBER=2 \
	-DFORCE_GHOSTS_NUMBER=6 \
	-DLESS_TEXT \
	-DNO_BLINKING \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_RANDOM_LEVEL \
	-DNO_SET_SCREEN_COLORS \
	-D__X07__ \
	-vn \
	-DCONIO_VT52 \
	-DNO_COLOR \
	-DTINY_GAME \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES) \
	-create-app -o $(BUILD_PATH)/TINY_x07.bin
	rm $(BUILD_PATH)/TINY_x07.bin	
	
	
rex:
	$(Z88DK_PATH)$(MYZ88DK) +rex $(SCCZ80_OPTS) \
	-D__REX__ \
	-DTURN_BASED \
	-vn \
	-DREX \
	-DSTDLIB \
	-DNO_COLOR \
	-DFULL_GAME -DSOUNDS \
	-lndos \
	-DNO_PRINT \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DNO_INPUT \
	-DNO_INIT_GRAPHICS \
	-DALT_CLEAR_SCREEN \
	$(FULL_FILES) \
	-create-app -o $(BUILD_PATH)/FULL_rex.bin	
	rm $(BUILD_PATH)/FULL_rex_BSS.bin
	rm $(BUILD_PATH)/FULL_rex.bin

	
rex_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +rex $(SCCZ80_OPTS) \
	-D__REX__ \
	-DTURN_BASED \
	-vn \
	-DBUFFERED \
	-DSTDLIB \
	-DNO_COLOR \
	-DFULL_GAME -DSOUNDS \
	-lndos \
	-DNO_PRINT \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DNO_INPUT \
	-DNO_INIT_GRAPHICS \
	-DALT_CLEAR_SCREEN \
	$(FULL_FILES) \
	-create-app -o $(BUILD_PATH)/FULL_rex_buffered.bin
	rm $(BUILD_PATH)/FULL_rex_buffered_BSS.bin
	rm $(BUILD_PATH)/FULL_rex_buffered.bin	
	

g800_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +g800 -clib=g850b $(SCCZ80_OPTS) \
	-D__G800__ \
	-DTURN_BASED \
	-DNO_BLINKING \
	-vn \
	-DFULL_GAME \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o 
	rm a.bin 
	rm a.rom
	mv a.ihx $(BUILD_PATH)/FULL_g800_turn_based.ihx
	
g800_tiny_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +g800 -clib=g850b $(SCCZ80_OPTS) \
	-D__G800__ \
	-DTURN_BASED \
	-DNO_BLINKING \
	-vn \
	-DFORCE_CONIO_LIB \
	-DTINY_GAME \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES) \
	-create-app -o
	rm a.bin
	rm a.rom
	mv a.ihx $(BUILD_PATH)/TINY_g800_turn_based.ihx

ts2068:
	$(Z88DK_PATH)$(MYZ88DK) +ts2068 $(SCCZ80_OPTS) \
	-clib=ansi \
	-D__TS2068__ \
	-DNO_COLOR \
	-vn \
	-DFULL_GAME \
	-DLESS_TEXT \
	-lndos \
	-DALT_PRINT \
	-DZ88DK_SPRITES \
	-DANIMATE_PLAYER \
	-DNO_INIT_GRAPHICS \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	$(FULL_FILES) \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	-create-app -o $(BUILD_PATH)/FULL_ts2068.bin 
	rm $(BUILD_PATH)/FULL_ts2068.bin		
		
	
ts2068_characters:
	$(Z88DK_PATH)$(MYZ88DK) +ts2068 $(SCCZ80_OPTS) \
	-clib=ansi \
	-D__TS2068__ \
	-DNO_COLOR \
	-vn \
	-DFULL_GAME \
	-lndos \
	-DFORCE_CONIO_LIB \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o $(BUILD_PATH)/FULL_ts2068.bin 
	rm $(BUILD_PATH)/FULL_ts2068.bin		
	
ts2068_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ts2068 $(SCCZ80_OPTS) \
	-clib=ansi \
	-DTURN_BASED \
	-D__TS2068__ \
	-DNO_COLOR \
	-vn \
	-DFULL_GAME \
	-lndos \
	-DFORCE_CONIO_LIB \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o $(BUILD_PATH)/FULL_ts2068_turn_based.bin 
	rm $(BUILD_PATH)/FULL_ts2068_turn_based.bin	

aussie_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -laussie  \
	$(SCCZ80_OPTS) \
	-vn \
	-D__AUSSIE__ \
	-DSOUNDS \
	-DFORCE_BIT_BANG \
	-DFORCE_XSIZE=16 \
	-DFORCE_YSIZE=12 \
	-DNO_COLOR \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-create-app	
	rm a.bin
	mv a.COM $(BUILD_PATH)/FULL_aussie.COM
	
# TODO: Try -subtype=com
enterprise_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +enterprise  \
	-compiler=sdcc \
	-vn \
	-D__ENTERPRISE__ \
	-DFORCE_XSIZE=20 \
	-DFORCE_YSIZE=16 \
	-DNO_COLOR \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	-DSTDLIB \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-create-app
	mv a.app $(BUILD_PATH)/FULL_enterprise.app
	rm a.bin

mz2500_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +mz2500 \
	-SO3 \
	-compiler=sdcc \
	-DFORCE_XSIZE=16 \
	-DFORCE_YSIZE=12 \
	-DFORCE_GHOSTS_NUMBER=6 \
	-DFORCE_BOMBS_NUMBER=4 \
	-D__MZ2500__ \
	-vn \
	-DNO_COLOR \
	-DFULL_GAME \
	-DNO_BLINKING \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_DEAD_GHOSTS \
	-DNO_SLEEP \
	-DNO_INITIAL_SCREEN \
	-DNO_SET_SCREEN_COLORS \
	-lndos \
	-DBUFFERED -DNO_INIT_GRAPHICS \
	-DFORCE_NARROW \
	-DNO_STATS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	-DSTDLIB \
	-DNO_WAIT \
	-DTURN_BASED \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	-create-app 
	mv a.2D $(BUILD_PATH)/FULL_mz2500_buffered.2D
	rm a.bin
# mz2500:
	# $(Z88DK_PATH)$(MYZ88DK) +mz2500 $(SCCZ80_OPTS) \
	# -D__MZ2500__ \
	# -vn \
	# -DCONIO_VT52 \
	# -DNO_COLOR \
	# -DFULL_GAME -DSOUNDS \
	# -DEND_SCREEN -DBETWEEN_LEVEL \
	# -lndos \
	# -DCONIO -DNO_INIT_GRAPHICS \
	# $(SOURCE_PATH)/end_screen.c \
	# $(FULL_FILES) \
	# -create-app
	
mz2500_random:
	$(Z88DK_PATH)$(MYZ88DK) +mz2500 -compiler=sdcc -SO3 $(SCCZ80_OPTS) \
	$(SOURCE_PATH)/../experiments/random_test.c \
	-create-app -o $(BUILD_PATH)/mz2500_random_test.bin

# It almost works except for TURN_BASED
mz2500_tiny_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +mz2500 \
	-SO3 \
	-compiler=sdcc \
	-DFORCE_XSIZE=16 \
	-DFORCE_YSIZE=12 \
	-DFORCE_GHOSTS_NUMBER=6 \
	-DFORCE_BOMBS_NUMBER=4 \
	-D__MZ2500__ \
	-vn \
	-DNO_COLOR \
	-DTINY_GAME \
	-DNO_BLINKING \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_DEAD_GHOSTS \
	-DNO_SLEEP \
	-DNO_INITIAL_SCREEN \
	-DNO_SET_SCREEN_COLORS \
	-lndos \
	-DBUFFERED -DNO_INIT_GRAPHICS \
	-DFORCE_NARROW \
	-DNO_STATS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	-DSTDLIB \
	-DNO_WAIT \
	-DTURN_BASED \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	-create-app 
	mv a.dsk $(BUILD_PATH)/TINY_mz2500_buffered.dsk
	rm a.bin


newbrain:
	$(Z88DK_PATH)$(MYZ88DK) +newbrain $(SCCZ80_OPTS) \
	-D__NEWBRAIN__ \
	-SO3 \
	-compiler=sdcc \
	-DFORCE_XSIZE=20 \
	-DFORCE_YSIZE=11 \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DFORCE_BOMBS_NUMBER=4 \
	-vn \
	-DNO_COLOR \
	-DFULL_GAME \
	-DNO_BLINKING \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_DEAD_GHOSTS \
	-DNO_SLEEP \
	-DNO_INITIAL_SCREEN \
	-DNO_SET_SCREEN_COLORS \
	-lndos \
	-DBUFFERED -DNO_INIT_GRAPHICS \
	-DFORCE_NARROW \
	-DNO_STATS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	-DSTDLIB \
	-DNO_WAIT \
	-DTURN_BASED \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	-create-app
	tar czvf FULL_newbrain.tar.zip _dir.txt a.bas a.dat
	mv FULL_newbrain.tar.zip $(BUILD_PATH)
	rm _dir.txt
	rm a.bas
	rm a.dat
	rm a.bin	

	
test:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-SO3 \
	-compiler=sdcc \
	-DFORCE_XSIZE=16 \
	-DFORCE_YSIZE=16 \
	-DFORCE_GHOSTS_NUMBER=6 \
	-DFORCE_BOMBS_NUMBER=2 \
	-vn \
	-DNO_COLOR \
	-DTINY_GAME \
	-DNO_BLINKING \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_DEAD_GHOSTS \
	-DNO_SLEEP \
	-DNO_INITIAL_SCREEN \
	-DNO_SET_SCREEN_COLORS \
	-lndos \
	-DBUFFERED -DNO_INIT_GRAPHICS \
	-DFORCE_NARROW \
	-DNO_STATS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	-DSTDLIB \
	-DNO_WAIT \
	-DTURN_BASED \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES) \
	-create-app
	rm a.bin
	mv A.COM $(BUILD_PATH)/TINY_CPM_TINY_BUFFERED.COM

	
# OK
cpm_tiny_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	$(SCCZ80_OPTS) \
	-DSTDLIB \
	-DFORCE_XSIZE=24 \
	-DFORCE_YSIZE=16 \
	-DNO_COLOR \
	-DTINY_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES) \
	-create-app
	rm a.bin
	mv A.COM $(BUILD_PATH)/TINY_CPM_TINY_BUFFERED.COM
	
# mz2500_tiny:
	# $(Z88DK_PATH)$(MYZ88DK) +mz2500 \
	# -compiler=sdcc \
	# -DFORCE_XSIZE=31 \
	# -DFORCE_YSIZE=20 \
	# -D__MZ2500__ \
	# -vn \
	# -DNO_COLOR \
	# -DTINY_GAME \
	# -DNO_BLINKING \
	# -DNO_CONTROL_INSTRUCTIONS \
	# -DNO_DEAD_GHOSTS \
	# -DNO_SLEEP \
	# -DNO_WAIT \
	# -DNO_INITIAL_SCREEN \
	# -DNO_SET_SCREEN_COLORS \
	# -lndos \
	# -DCONIO -DNO_INIT_GRAPHICS \
	# -DFORCE_NARROW \
	# -DNO_STATS \
	# -DNO_PRINT \
	# -pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	# $(TINY_FILES) \
	# -create-app -o $(BUILD_PATH)/TINY_mz2500.bin
	# rm $(BUILD_PATH)/TINY_mz2500.bin	
	
pps:
	$(Z88DK_PATH)$(MYZ88DK) +pps $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__PPS__ -vn \
	-DCONIO_VT52 \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.bin $(BUILD_PATH)/FULL_pps.exe	
	
pps_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +pps $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__PPS__ -vn \
	-DCONIO_VT52 \
	-DTURN_BASED \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.bin $(BUILD_PATH)/FULL_pps_turn_based.exe

	
# ------------------------------------

	# $(CROSS_LIB_PATH)/display/init_images.c \
	# $(CROSS_LIB_PATH)/display/cc65/c64/c64_init_graphics.c \
	# $(CROSS_LIB_PATH)/display/memory_mapped/memory_mapped_graphics.c \
	# $(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \

coco:
	cmoc \
	$(COCO_OPTS) \
	-c $(CROSS_LIB_PATH)/display/display_macros.c
	cmoc \
	$(COCO_OPTS) \
	-c $(CROSS_LIB_PATH)/display/init_images.c	
	cmoc \
	$(COCO_OPTS) \
	-c $(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	cmoc \
	$(COCO_OPTS) \
	-c $(CROSS_LIB_PATH)/input/input_macros.c
	cmoc \
	$(COCO_OPTS) \
	-c $(CROSS_LIB_PATH)/text/text_macros.c	
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/move_player.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/ghost.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/level.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/character.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/game_text.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/strategy.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/item.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/bullet.c
	cmoc \
	$(COCO_OPTS) \
	-c $(SOURCE_PATH)/skull.c	
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
	display_macros.o \
	init_images.o \
	memory_mapped_alt_print.o \
	ghost.o \
	move_player.o \
	level.o character.o \
	text_macros.o game_text.o \
	strategy.o \
	input_macros.o \
	item.o bullet.o skull.o \
	rocket.o horizontal_missile.o end_screen.o
	rm 	display_macros.o \
	init_images.o \
	memory_mapped_alt_print.o \
	ghost.o \
	move_player.o \
	level.o character.o \
	game_text.o text_macros.o \
	strategy.o \
	input_macros.o \
	item.o bullet.o skull.o \
	rocket.o horizontal_missile.o end_screen.o
		

hello_vec:
	cmoc --vectrex -Wno-const -c $(SOURCE_PATH)/../experiments/hello_vec.c 
	cmoc --vectrex -o $(BUILD_PATH)/hello_vec.bin $(SOURCE_PATH)/../hello_vec.o
	rm $(SOURCE_PATH)/../hello_vec.o
	
hello_coco:
	cmoc -D__CMOC__ -Wno-const -c $(SOURCE_PATH)/../experiments/hello.c 
	cmoc -o $(BUILD_PATH)/hello_coco.bin $(SOURCE_PATH)/../hello.o
	rm $(SOURCE_PATH)/../hello.o
	
vectrex:
	cmoc \
	$(VECTREX_OPTS) \
	-c $(CROSS_LIB_PATH)/display/display_macros.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(CROSS_LIB_PATH)/display/init_images.c	
	cmoc \
	$(VECTREX_OPTS) \
	-c $(CROSS_LIB_PATH)/input/input_macros.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(CROSS_LIB_PATH)/text/text_macros.c	
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/move_player.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/ghost.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/level.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/character.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/game_text.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/strategy.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/item.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/bullet.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/skull.c	
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/horizontal_missile.c
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/rocket.c 
	cmoc \
	$(VECTREX_OPTS) \
	-c $(SOURCE_PATH)/end_screen.c	
	cmoc -o $(BUILD_PATH)/FULL_Vectrex.bin \
	$(VECTREX_OPTS) \
	$(SOURCE_PATH)/main.c \
	display_macros.o \
	init_images.o \
	ghost.o \
	move_player.o \
	level.o character.o \
	text_macros.o game_text.o \
	strategy.o \
	input_macros.o \
	item.o bullet.o skull.o \
	rocket.o horizontal_missile.o end_screen.o
	rm 	display_macros.o \
	init_images.o \
	ghost.o \
	move_player.o \
	level.o character.o \
	game_text.o text_macros.o \
	strategy.o \
	input_macros.o \
	item.o bullet.o skull.o \
	rocket.o horizontal_missile.o end_screen.o
		
		
		
		
# ------------------------------------

pc8086_tiny_buffered:
	$(_ACK) $(_ACK_OPTS) -mpc86 \
	-DFORCE_XSIZE=20 \
	-DFORCE_YSIZE=20 \
	-DCONIO_ADM3A \
	-DNO_COLOR \
	-DACK \
	-D__PC8086__ \
	-DTINY_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DNO_PRINT \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DALT_CLEAR_SCREEN \
	$(TINY_FILES)
	mv pc86.img $(BUILD_PATH)/TINY_pc8086_turn_based.img

cpm8080_buffered:
	$(_ACK) $(_ACK_OPTS) -mcpm \
	-DFORCE_XSIZE=24 \
	-DFORCE_YSIZE=16 \
	-DCONIO_ADM3A \
	-DNO_COLOR \
	-DACK \
	-D__CPM8080__ \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DSTDLIB \
	-DNO_INIT_GRAPHICS \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DALT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv cpm.com $(BUILD_PATH)/FULL_cpm8080_buffered.com
	rm $(SOURCE_PATH)/../*.o

	
cpm8080_adm3a:
	$(_ACK) $(_ACK_OPTS) -mcpm \
	-DFORCE_XSIZE=78 \
	-DFORCE_YSIZE=22 \
	-DCONIO_ADM3A \
	-DNO_COLOR \
	-DACK \
	-D__CPM8080__ \
	-DFULL_GAME \
	-DTURN_BASED \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-DNO_WAIT \
	-DNO_SLEEP \
	$(FULL_FILES)
	mv cpm.com $(BUILD_PATH)/FULL_cpm8080_adm3a.com
	rm $(SOURCE_PATH)/../*.o	

	
# buffered	
pc8086:
	$(_ACK) $(_ACK_OPTS) -mpc86 \
	-DFORCE_XSIZE=78 \
	-DFORCE_YSIZE=22 \
	-DCONIO_ADM3A \
	-DNO_COLOR \
	-DACK \
	-D__PC8086__ \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DALT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv pc86.img $(BUILD_PATH)/FULL_pc8086_turn_based.img
	rm $(SOURCE_PATH)/../*.o

	
# buffered
linuxppc:
	$(_ACK) $(_ACK_OPTS) -mlinuxppc \
	-DFORCE_XSIZE=78 \
	-DFORCE_YSIZE=21 \
	-DCONIO_ADM3A \
	-DNO_COLOR \
	-DACK \
	-D__LINUXPPC__ \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DALT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv linuxppc.exe $(BUILD_PATH)/FULL_linuxppc_turn_based.exe
	rm $(SOURCE_PATH)/../*.o


#buffered
linux68k:
	$(_ACK) $(_ACK_OPTS) -mlinux68k \
	-DFORCE_XSIZE=78 \
	-DFORCE_YSIZE=21 \
	-DCONIO_ADM3A \
	-DNO_COLOR \
	-DACK \
	-D__LINUX68K__ \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DALT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv linux68k.exe $(BUILD_PATH)/FULL_linux68k_turn_based.exe
	rm $(SOURCE_PATH)/../*.o
	

# buffered
linux386:
	$(_ACK) $(_ACK_OPTS) -mlinux386 \
	-DFORCE_XSIZE=78 \
	-DFORCE_YSIZE=21 \
	-DCONIO_ADM3A \
	-DNO_COLOR \
	-DACK \
	-D__LINUX386__ \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DALT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv linux386.exe $(BUILD_PATH)/FULL_linux386_turn_based.exe
	rm $(SOURCE_PATH)/../*.o


# buffered
linuxmips:
	$(_ACK) $(_ACK_OPTS) -mlinuxmips \
	-DFORCE_XSIZE=78 \
	-DFORCE_YSIZE=21 \
	-DCONIO_ADM3A \
	-DNO_COLOR \
	-DACK \
	-D__LINUXPPC__ \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DALT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv linuxppc.exe $(BUILD_PATH)/FULL_linuxmips_turn_based.exe
	rm $(SOURCE_PATH)/../*.o	


# buffered
pdp11v7:
	$(_ACK) $(_ACK_OPTS) -mpdpv7 \
	-DFORCE_XSIZE=78 \
	-DFORCE_YSIZE=22 \
	-DCONIO_ADM3A \
	-DNO_COLOR \
	-DACK \
	-D__PDP11V7__ \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DNO_PRINT \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DALT_CLEAR_SCREEN \
	$(FULL_FILES)
	mv a.out $(BUILD_PATH)/FULL_pdp11v7_turn_based.bin
	rm $(SOURCE_PATH)/../*.o
	

# no ncurses in ACK
dos:
	$(_ACK) $(_ACK_OPTS) -pc86 \
	-D__DOS__ \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DTURN_BASED \
	-DCONIO  \
	-DALT_PRINT \
	$(CROSS_LIB_PATH)/display/alt_print/ncurses_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/ncurses/ncurses_init_graphics.c \
	$(FULL_FILES) \
	-lncurses
	# mv a.exe $(BUILD_PATH)/FULL_dos_turn_based.exe

# ------------------------------------


ncurses_turn_based:
	$(_CC) $(_CC_OPTS) -D__NCURSES__ \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DTURN_BASED \
	-DCONIO  \
	-DALT_PRINT \
	$(CROSS_LIB_PATH)/display/alt_print/ncurses_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/ncurses/ncurses_init_graphics.c \
	$(FULL_FILES) \
	-lncurses
	mv a$(COMPILEDEXT) $(BUILD_PATH)/FULL_ncurses_turn_based$(COMPILEDEXT)
	
ncurses:
	$(_CC) $(_CC_OPTS) -D__NCURSES__ \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO  \
	-DALT_PRINT \
	-DNO_TITLE_LINE \
	$(CROSS_LIB_PATH)/display/alt_print/ncurses_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/ncurses/ncurses_init_graphics.c \
	$(FULL_FILES) \
	-lncurses
	mv a$(COMPILEDEXT) $(BUILD_PATH)/FULL_ncurses$(COMPILEDEXT)

# cannot run in Mame (Kaypro 2x)
# 	--max-allocs-per-node200000 -SO3 --opt-code-size 
#	-compiler=sdcc
# MAX: 79, 21 
cpm_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	$(SCCZ80_OPTS) \
	-DSTDLIB \
	-DFORCE_XSIZE=24 \
	-DFORCE_YSIZE=16 \
	-DNO_COLOR \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-create-app
	rm a.bin
	mv A.COM $(BUILD_PATH)/FULL_CPM_BUFFERED.COM
	# cp $(BUILD_PATH)/FULL_CPM_BUFFERED.COM $(BUILD_PATH)/CPM.COM
	
	
gcc_buffered:
	$(_CC) $(_CC_OPTS) \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DNO_COLOR \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv a$(COMPILEDEXT) $(BUILD_PATH)/FULL_gcc_buffered$(COMPILEDEXT)


.PHONY: mtx vic20exp_8k vic20exp_16k  atari_color atari_no_color atari_no_color_16k atari5200 atmos c128_40col c128_80col c16_16k c16_32k c64 pet cbm510 cbm610 nes apple2 apple2enh

# KO:
#  
# OK: 
#
# No. of systems: 19
# ------------

cc65_targets: \
	atari_targets \
	oric_targets \
	cbm510_targets \
	cbm610_targets \
	apple2_targets \
	apple2enh_targets \
	c64_targets \
	c128_8502_targets \
	pce_targets \
	atari5200_targets \
	nes_targets \
	creativision_targets \
	osic1p_targets \
	pet_targets \
	vic20_targets \
	c264_targets \
	atari_lynx_targets \
	gamate_targets	\
	supervision_targets	
	
# KO: 	
# m5: out of memory
# sg1000: interrupt

# OK:

# to_fix:
	# einstein_targets \
	# sc3000_targets

# Number of systems: 41 - 1 (c128_z80) = 40


zsdcc_test: \
	aquarius_zsdcc_test \
	spectrum_zsdcc_test \
	vz200_zsdcc_test \
	vg5k_zsdcc_test \
	mc1000_zsdcc_test \
	zx81_zsdcc_test \
	svi_zsdcc_test \
	c128_z80_zsdcc_test

sccz80_test: \
	einstein_test \
	sc3000_test \
	sg1000_test \
	m5_test \
	ace_test \
	pc6001_test \
 	eg2k_test \
	pps_test \
	pv2000_test \
	srr_test \
	ti82_test \
	ti83_test \
	ti85_test \
	z1013_test \
	x1_test \
	px4_test \
	px8_test \
	kc_test \
	trs80_test \
	cpm_test \
	nascom_test \
	z9001_test \
	vg5k_test \
	cpc_test \
	mc1000_test \
	mz_test \
	mtx_test \
	abc80_test \
	p2000_test \
	svi_test \
	msx_test \
	aquarius_test \
	vz200_test \
	microbee_test \
	gal_test \
	zx80_test \
	zx81_test \
	spectrum_test \
	samcoupe_test \
	lambda_test \
	nc100_test \
	c128_z80_test

z88dk_test: \
	sccz80_test \
	zsdcc_test	
	


# missing: abc800, xo7, c7420, rex
z88dk_targets: \
	abc80_targets \
	ace_targets \
	alphatro_targets \
	aquarius_targets \
	attache_targets \
	aussie_targets \
	camputers_lynx_targets \
	c128_z80_targets \
	coleco_targets \
	cpc_targets \
	cpm_targets \
 	eg2k_targets \
	einstein_targets \
	enterprise_targets \
	fp1100_targets \
	g800_targets \
	gal_targets \
	kaypro_targets \
	kc_targets \
	lambda_targets \
	laser500_targets \
	m5_targets \
	mc1000_targets \
	microbee_targets \
	msx_targets \
	mtx_targets \
	multi8_targets \
	mz_targets \
	mz2500_targets \
	nascom_targets \
	nc100_targets \
	nc200_targets \
	newbrain_targets \
	osborne1_targets \
	osca_targets \
	oz_targets \
	p2000_targets \
	pc6001_targets \
	pps_targets \
	pv1000_targets \
	pv2000_targets \
	px4_targets \
	px8_targets \
	rx78_targets \
	samcoupe_targets \
	sc3000_targets \
	sms_targets \
	sos_targets \
	spc1000_targets \
	spectrum_targets \
	srr_targets \
	svi_targets \
	ti82_targets \
	ti83_targets \
	ti85_targets \
	ti86_targets \
	ti8x_targets \
	tiki100_targets \
	trs80_targets \
	ts2068_targets \
	vg5k_targets \
	vz200_targets \
	x1_targets \
	z1013_targets \
	z88_targets \
	z9001_targets \
	zx80_targets \
	zx81_targets \

cmoc_targets: \
	coco vectrex
	
ack_targets: \
	pdp11v7 cpm8080_buffered pc8086 linux386 linux68k linuxppc  
	
all: cc65_targets z88dk_targets cmoc_targets ncurses_targets ack_targets

clean_chase_obj:
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(SOURCE_PATH)/wincmoc_main.c
	rm -rf $(SOURCE_PATH)/wincmoc_main.asm
	rm -rf $(SOURCE_PATH)/wincmoc_main.coco.bin

clean_lib_obj:
	rm -rf $(CROSS_LIB_PATH)/*.o
	rm -rf $(CROSS_LIB_PATH)/display/*.o
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/cc65/apple2enh/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/cc65/atari/*.o
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/cc65/atari_lynx/*.o
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/cc65/atmos/*.o
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/cc65/c64/*.o
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/cc65/c264/*.o
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/cc65/c128/*.o
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/*.o
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/ncurses/*.o
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/z88dk/aquarius/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/z88dk/cpc/*.o		
	rm -rf $(CROSS_LIB_PATH)/display/init_graphics/z88dk/msx/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/alt_print/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/graphics_data/cc65/*.o	
	rm -rf $(CROSS_LIB_PATH)/input/*.o
	rm -rf $(CROSS_LIB_PATH)/sleep/*.o
	rm -rf $(CROSS_LIB_PATH)/sound/*.o
	rm -rf $(CROSS_LIB_PATH)/sound/cc65/pokey/*.o
	rm -rf $(CROSS_LIB_PATH)/sound/cc65/atmos/*.o
	rm -rf $(CROSS_LIB_PATH)/sound/cc65/c264/*.o
	rm -rf $(CROSS_LIB_PATH)/sound/cc65/sid/*.o
	rm -rf $(CROSS_LIB_PATH)/sound/cc65/vic20/*.o	
	rm -rf $(CROSS_LIB_PATH)/sound/z88dk/bit_bang/*.o
	rm -rf $(CROSS_LIB_PATH)/sound/z88dk/psg/*.o	
	rm -rf $(CROSS_LIB_PATH)/text/*.o	

clean_obj: \
	clean_chase_obj \
	clean_lib_obj

clean_build:
	rm -rf $(BUILD_PATH)/*


clean: \
	clean_obj \
	clean_build



help:
	cat docs/BUILD.txt
	cat docs/TARGETS.txt

list:
	cat docs/TARGETS.txt
	

#
#

aussie_targets: \
	aussie_buffered

eg2k_targets: \
	eg2k_udg

pps_targets: \
	pps

pv2000_targets: \
	pv2000_udg

srr_targets: \
	srr_udg

m5_targets: \
	m5_tiny m5_udg

ti82_targets: \
	ti82

ti83_targets: \
	ti83

ti8x_targets: \
	ti8x
	
ti85_targets: \
	ti85
	
ti86_targets: \
	ti86_light
	
tiki100_targets: \
	tiki100

enterprise_targets: \
	enterprise_buffered
	
mz2500_targets: \
	mz2500_tiny_buffered
	
einstein_targets: \
	einstein
	
newbrain_targets: \
	newbrain

kaypro_targets: \
	kaypro
	
tiki100_targets: \
	tiki100

osca_targets: \
	osca
	
camputers_lynx_targets: \
	camputers_lynx camputers_lynx_tiny
	
attache_targets: \
	attache attache_chars
	
osborne1_targets: \
	osborne1 osborne1_chars
	
z1013_targets: \
	z1013
	
x1_targets: \
	x1

px4_targets: \
	px4 

px8_targets: \
	px8 px8_chars
	
kc_targets: \
	kc_tiny_udg kc_light_udg kc_udg
	
trs80_targets: \
	trs80_chars trs80_tiny trs80 trs80_cmd

cpm_targets: \
	cpm_adm3a cpm_vt52 cpm_vt100 

nascom_targets: \
	nascom_16k nascom_32k 
	
pc6001_targets: \
	pc6001_16k pc6001_32k_udg 	
	
z9001_targets: \
	z9001_16k z9001_32k

vg5k_targets: \
	vg5k vg5k_exp_16k_udg
	
sc3000_targets: \
	sc3000_16k sc3000_32k_udg sg1000_udg
	
ace_targets: \
	ace_exp_16k_udg

cpc_targets: \
	cpc
	
mc1000_targets: \
 	mc1000_16k_full mc1000_48k_udg mc1000_48k_udg_joystick
	
mz_targets: \
 	mz
	
mtx_targets: \
	mtx_udg
	
abc80_targets: \
	abc80_light abc80
	
p2000_targets: \
	p2000_16k p2000_32k

svi_targets: \
	svi_318 svi_328_udg_cursor

msx_targets: \
	msx_32k_rom msx_32k

c128_z80_targets: \
	c128_z80_40col c128_z80_80col
	
aquarius_targets: \
	aquarius_exp_4k aquarius_exp_16k
	
vz200_targets: \
	vz200_8k vz200_18k vz200_sprites_18k
	
microbee_targets: \
	microbee_16k microbee_32k
	
gal_targets: \
	gal_6k gal_22k
	
zx80_targets: \
	zx80_16k_turn_based
	
zx81_targets: \
	zx81_16k zx81_16k_turn_based
	
spectrum_targets: \
	spectrum_16k spectrum_48k

ts2068_targets: \
	ts2068
	
g800_targets: \
	g800_turn_based
	
rx78_targets: \
	rx78_udg
	
pv1000_targets: \
	pv1000_light pv1000
	
coleco_targets: \
	coleco_udg
	
multi8_targets: \
	multi8_udg
	
spc1000_targets: \
	spc1000_udg
	
alphatro_targets: \
	alphatro
	
fp1100_targets: \
	fp1100

samcoupe_targets: \
	samcoupe
	
lambda_targets: \
	lambda_16k
	
nc100_targets: \
	nc100
	
nc200_targets: \
	nc200

laser500_targets: \
	laser500
	
oz_targets: \
	oz_tiny_buffered
	
z88_targets: \
	z88_tiny_buffered
	
sos_targets: \
	sos_buffered sos_tiny_buffered

sms_targets: \
	sms_tiny_buffered
	
##

supervision_targets: \
	supervision

vic20_targets: \
	vic20_unexpanded vic20_exp_3k vic20_exp_8k vic20_exp_8k_full vic20_exp_16k

atari_targets: \
	atari_color atari_no_color
	
oric_targets: \
	atmos oric1_16k

c264_targets: \
	c16_16k c16_16k_full c16_32k
	
pet_targets: \
	pet_8k pet_16k
	
cbm510_targets: \
	cbm510
	
cbm610_targets: \
	cbm610
	
apple2_targets: \
	apple2

apple2enh_targets: \
	apple2enh apple2enh_80col

c64_targets: \
	c64 c64_8k_cart
	
c128_8502_targets: \
	c128_40col c128_80col
	
pce_targets: \
	pce_8k pce_16k 
	
atari5200_targets: \
	atari5200

nes_targets: \
	nes_fx
	
creativision_targets: \
	creativision_8k_tiny creativision_16k
	
gamate_targets: \
	gamate
	
atari_lynx_targets: \
	atari_lynx
	
osic1p_targets: \
	osic1p_8k osic1p_32k
	
c128_targets: \
	c128_8502_targets c128_z80_targets


##

ncurses_targets: \
	ncurses ncurses_turn_based
	
####################################################################################################################

# TESTS

FAST_TEST_OPTS ?= \
-O0 \
-DTINY_GAME -DLESS_TEXT \
-DNO_BLINKING -DNO_CHASE \
-DNO_INITIAL_SCREEN -DNO_SET_SCREEN_COLORS \
-DNO_DEAD_GHOSTS \
-DNO_RANDOM_LEVEL -DFLAT_ENEMIES -DFORCE_GHOSTS_NUMBER=8 \
-DNO_STATS


einstein_test:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -leinstein  -v \
	-D__EINSTEIN__ \
	-DFORCE_CONIO_LIB \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-clib=ansi \
	-create-app -o$(BUILD_PATH)/TEST_einstein.bin \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_einstein.bin

sc3000_test:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/TEST_sc3000_16k.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_sc3000_16k.prg
	rm $(BUILD_PATH)/TEST_sc3000_16k.tap	

	
sg1000_test:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 -subtype=rom \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/TEST_sg1000.prg \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_sg1000.prg	
	
m5_test:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-lm -create-app -Cz--audio -subtype=tape \
	-D__M5__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	$(FAST_TEST_OPTS) \
	-o$(BUILD_PATH)/TEST_m5.bin \
	$(TEST_FILES)
	
ace_test:
	$(Z88DK_PATH)$(MYZ88DK) +ace \
	-D__ACE__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-clib=ansi -o test -Cz--audio -create-app \
	$(TEST_FILES)
	cp test.wav $(BUILD_PATH)/TEST_ace_exp_16k.wav
	rm test.wav
	rm test.tap
	rm test	
	
pc6001_test:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 -Cz--audio -clib=ansi -subtype=32k \
	-D__PC6001__ -v \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o $(BUILD_PATH)/TEST_pc6001_32k.prg \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_pc6001_32k.prg
	rm $(BUILD_PATH)/TEST_pc6001_32k.wav
	mv $(BUILD_PATH)/TEST_pc6001_32k.cas $(BUILD_PATH)/TEST_pc6001_32k.cp6	
	
eg2k_test:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 \
	-D__EG2K__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-subtype=eg2000disk \
	-lndos \
	-lm \
	-D__TRS80__ \
	$(FAST_TEST_OPTS) \
	-create-app \
	-DCONIO_VT52 \
	$(TEST_FILES)
	mv a.cmd $(BUILD_PATH)/TEST_eg2k.cmd	
	rm a.bin

pps_test:	
	$(Z88DK_PATH)$(MYZ88DK) +pps -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__PPS__ -vn \
	-DCONIO_VT52 \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-DNO_WAIT \
	-lndos \
	$(TEST_FILES) 
	mv a.bin $(BUILD_PATH)/TEST_pps.exe		
	
pv2000_test:
	$(Z88DK_PATH)$(MYZ88DK) +pv2000 -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__SRR__ -vn \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-DSOUNDS \
	-DNO_WAIT \
	-DCONIO_VT52 \
	-lndos \
	$(FAST_TEST_OPTS) \
	-create-app \
	$(TEST_FILES)
	mv a.rom $(BUILD_PATH)/TEST_pv2000.rom
	rm a.bin
	rm a_BSS.bin
	rm a_DATA.bin

srr_test:
	$(Z88DK_PATH)$(MYZ88DK) +srr -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__SRR__ -vn \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-DSOUNDS \
	-DNO_WAIT \
	-DCONIO_VT52 \
	$(FAST_TEST_OPTS) \
	-lndos \
	-create-app \
	$(TEST_FILES)
	rm a.srr
	rm a.bin
	mv a.wav $(BUILD_PATH)/TEST_srr.wav
	
ti82_test:
	$(Z88DK_PATH)$(MYZ88DK) +ti82 \
	$(FAST_TEST_OPTS) -D__TI82__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DTURN_BASED -DNO_WAIT \
	-lndos \
	-create-app \
	$(TEST_FILES)
	mv $(SOURCE_PATH)/../a.82p $(BUILD_PATH)/TEST_ti82.82p
	rm $(SOURCE_PATH)/../a.bin
	
ti83_test:
	$(Z88DK_PATH)$(MYZ88DK) +ti83 \
	$(FAST_TEST_OPTS) -D__TI83__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DTURN_BASED -DNO_WAIT \
	-lndos \
	-create-app \
	$(TEST_FILES)
	mv $(SOURCE_PATH)/../a.83p $(BUILD_PATH)/TEST_ti83.83p
	rm $(SOURCE_PATH)/../a.bin

ti85_test:
	$(Z88DK_PATH)$(MYZ88DK) +ti85 \
	$(FAST_TEST_OPTS) -D__TI85__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DTURN_BASED -DNO_WAIT \
	-lndos \
	-create-app  \
	$(TEST_FILES)
	mv $(SOURCE_PATH)/../a.85s $(BUILD_PATH)/TEST_ti85.85s
	rm $(SOURCE_PATH)/../a.bin	
	
z1013_test:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 -clib=ansi \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-vn -lndos \
	-D__Z1013__ \
	$(FAST_TEST_OPTS) \
	$(TEST_FILES) \
	-create-app
	mv $(SOURCE_PATH)/../A.Z80 $(BUILD_PATH)/TEST_z1013.z80
	rm $(SOURCE_PATH)/../a.bin		
	
x1_test:
	$(Z88DK_PATH)$(MYZ88DK) +x1 \
	-D__X1__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-DNO_SLEEP \
	$(FAST_TEST_OPTS) \
	-create-app -o $(BUILD_PATH)/TEST_x1.bin -vn -lndos \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_x1.bin
	
px4_test:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-pragma-define:ansicolumns=40 \
	-pragma-define:ansipixels=240 -pragma-define:ansicolumns=60 \
 	-subtype=px4ansi \
	-D__PX4__ \
	-DNO_WAIT \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-DZ88DK_PUTC4X6 \
	-DALT_PRINT \
	-Cz–-32k \
	-create-app -o $(BUILD_PATH)/TEST_px4.bin \
	-vn -lndos \
	$(CROSS_LIB_PATH)/display/alt_print/z88dk_putc4x6_alt_print.c \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_px4.bin
	
px8_test:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px32k \
	-D__PX8__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-DCONIO_ADM3A \
	$(FAST_TEST_OPTS) \
	-DNO_SLEEP \
	-DNO_WAIT \
	-create-app -o$(BUILD_PATH)/TEST_px8.bin \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_px8.bin
	
kc_test:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap \
	-D__KC__ \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-DZ88DK_SPRITES \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DANIMATE_PLAYER \
	-create-app -o$(BUILD_PATH)/TEST_kc_sprites.bin \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_kc_sprites.bin	
	
trs80_test:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	-lm -create-app \
	-D__TRS80__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-DNO_SLEEP \
	-DCONIO_VT52 \
	-o$(BUILD_PATH)/TEST_trs80.bin \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_trs80.bin	

cpm_test:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-DCONIO_ADM3A \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-D__CPM_80X24__ \
	-DNO_SLEEP -DNO_WAIT \
	$(FAST_TEST_OPTS) \
	-create-app -o$(BUILD_PATH)/TEST_cpm.bin \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_cpm.bin
	
nascom_test:
	$(Z88DK_PATH)$(MYZ88DK) +nascom -clib=ansi -vn -lndos \
	-D__NASCOM__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-DSOUNDS \
	$(FAST_TEST_OPTS) \
	-create-app -o $(BUILD_PATH)/TEST_nascom.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_nascom.prg
	
z9001_test:
	$(Z88DK_PATH)$(MYZ88DK) +z9001 -clib=ansi \
	-D__Z9001__ -vn   \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o $(BUILD_PATH)/TEST_z9001.z80 \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_z9001.z80	
	
vg5k_test:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-DSOUNDS -vn -D__VG5K__ -DASM_DISPLAY \
	-lndos -create-app -o $(BUILD_PATH)/TEST_vg5k.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_vg5k.prg	

cpc_test:	
	$(Z88DK_PATH)$(MYZ88DK) +cpc -DANIMATE_PLAYER -vn  -clib=ansi \
	-D__CPC__ -DSOUNDS \
	$(FAST_TEST_OPTS) \
	-DCPCRSLIB \
	-pragma-define:REGISTER_SP=-1 \
	-lndos -create-app -o 	$(BUILD_PATH)/TEST_cpc.prg \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/cpc_Chars8.asm \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/cpc/cpc_cpcrslib_init_graphics.c \
	$(TEST_FILES)
	$(TOOLS_PATH)/z88dk/cpc/2cdt.exe -n -r cross_chase $(BUILD_PATH)/TEST_cpc.cpc  $(BUILD_PATH)/TEST_cpc.cdt
	# $(TOOLS_PATH)/z88dk/cpc/cpcxfsw -nd TEST_cpc.dsk
	# $(TOOLS_PATH)/z88dk/cpc/cpcxfsw TEST_cpc.dsk -p $(BUILD_PATH)/TEST_cpc.cpc xchase
	# mv TEST_cpc.dsk $(BUILD_PATH)/
	# rm $(BUILD_PATH)/TEST_cpc.cpc 
	# rm $(BUILD_PATH)/TEST_cpc.prg	

mc1000_test:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 \
	-subtype=gaming -pragma-define:ansicolumns=32 \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-clib=ansi \
	-D__MC1000__ -DSOUNDS \
	-vn  -lndos -create-app -Cz--audio \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(TEST_FILES)
	mv a.wav $(BUILD_PATH)/TEST_mc1000.wav
	rm a.bin
	rm a.cas	

mz_test:	
	$(Z88DK_PATH)$(MYZ88DK) +mz \
	-D__MZ__ -clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DSOUNDS \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o $(BUILD_PATH)/TEST_mz.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_mz.prg
	mv $(BUILD_PATH)/TEST_mz.mzt $(BUILD_PATH)/TEST_mz.mzf

mtx_test:
	$(Z88DK_PATH)$(MYZ88DK) +mtx -startup=2 \
	-D__MTX__ -clib=ansi -pragma-define:ansicolumns=32 -create-app -o TEST_mtx.bin -vn \
	-DSOUNDS \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-lndos \
	$(TEST_FILES)
	rm TEST_mtx.bin
	mv TEST_mtx.wav $(BUILD_PATH)/TEST_mtx.wav
	mv TEST_mtx $(BUILD_PATH)/TEST_mtx.mtx

abc80_test: 	
	$(Z88DK_PATH)$(MYZ88DK) +abc80 -lm -subtype=hex -zorg=49200 \
	-D__ABC80__ -clib=ansi -vn -DSOUNDS  -lndos \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-create-app -o a \
	$(TEST_FILES)
	rm a
	mv a.ihx $(BUILD_PATH)/TEST_abc80.ihx 	
	

p2000_test:
	$(Z88DK_PATH)$(MYZ88DK) +p2000 -clib=ansi -D__P2000__ -vn \
	-DSOUNDS  \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o $(BUILD_PATH)/TEST_p2000.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_p2000.prg

svi_test:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	-clib=ansi -pragma-define:ansicolumns=32 -vn -lndos \
	-DSOUNDS \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-D__SVI__ \
	$(FAST_TEST_OPTS) \
	-create-app \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c  \
	$(TEST_FILES)
	mv a.cas $(BUILD_PATH)/TEST_svi.cas
	rm a.bin

msx_test:
	$(Z88DK_PATH)$(MYZ88DK) +msx -zorg=49200 \
	-DSOUNDS -DANIMATE_PLAYER -create-app -vn -DMSX_VPOKE -D__MSX__ -lndos \
	$(FAST_TEST_OPTS) \
	-DMEMORY_MAPPED \
	-create-app -o $(BUILD_PATH)/TEST_msx.prg \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/msx/msx_redefined_chars_init_graphics.c \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_msx.prg 	
	
aquarius_test: 
	$(Z88DK_PATH)$(MYZ88DK) +aquarius -clib=ansi -vn \
	-DSOUNDS -D__AQUARIUS__  -DTINY_GAME \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-lndos \
	-o TEST_aquarius -create-app \
	$(TEST_FILES)
	rm $(SOURCE_PATH)/../TEST_aquarius
	mv $(SOURCE_PATH)/../TEST_aquarius.caq $(BUILD_PATH)
	mv $(SOURCE_PATH)/../_TEST_aquarius.caq $(BUILD_PATH)	
	
vz200_test:
	$(Z88DK_PATH)$(MYZ88DK) +vz -vn \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc \
	-D__VZ__ -clib=ansi \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-lndos \
	-create-app -o $(BUILD_PATH)/TEST_vz200.vz \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_vz200.cas	
	
	
microbee_test:
	$(Z88DK_PATH)$(MYZ88DK) +bee \
	-D__BEE__ -clib=ansi -vn -DSOUNDS  \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o $(BUILD_PATH)/TEST_microbee.prg  \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_microbee.prg


gal_test:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-pragma-need=ansiterminal \
	-D__GAL__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-vn -lndos -create-app -o  $(BUILD_PATH)/TEST_galaksija.prg \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_galaksija.prg	
	rm $(BUILD_PATH)/TEST_galaksija.wav
	
zx80_test:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 -vn \
	-D__ZX80__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-DTURN_BASED \
	$(FAST_TEST_OPTS) \
	-DALT_SLEEP \
	-lndos \
	-create-app -o  $(BUILD_PATH)/TEST_zx80.prg \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_zx80.prg
	
zx81_test:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -vn \
	-D__ZX81__ \
	-DTURN_BASED \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-DALT_SLEEP \
	-lndos \
	-create-app -o  $(BUILD_PATH)/TEST_zx81.prg \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_zx81.prg
		
spectrum_test:
	$(Z88DK_PATH)$(MYZ88DK) +zx -clib=ansi -vn  \
	-pragma-redirect:ansifont=_udg -pragma-define:ansifont_is_packed=0 -pragma-define:ansicolumns=32 \
	-DANIMATE_PLAYER -DSOUNDS -DCLIB_ANSI -D__SPECTRUM__ \
	-DCONIO \
	-lndos -create-app \
	$(FAST_TEST_OPTS) \
	-o $(BUILD_PATH)/TEST_spectrum.prg \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/spectrum/udg.asm \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/spectrum/spectrum_redefined_chars_init_graphics.c \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_spectrum.prg
	rm $(BUILD_PATH)/TEST_spectrum_BANK_7.bin	
	
samcoupe_test:
	$(Z88DK_PATH)$(MYZ88DK) +sam \
	-D__SAM__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	-o $(BUILD_PATH)/FULL_samcoupe.bin -lndos \
	$(TEST_FILES)
	cp $(TOOLS_PATH)/z88dk/samcoupe/samdos2_empty $(TOOLS_PATH)/z88dk/samcoupe/samdos2
	$(TOOLS_PATH)/z88dk/samcoupe/pyz80.py -I $(TOOLS_PATH)/z88dk/samcoupe/samdos2 $(TOOLS_PATH)/z88dk/samcoupe/sam_wrapper.asm
	mv $(TOOLS_PATH)/z88dk/samcoupe/sam_wrapper.dsk $(BUILD_PATH)/TEST_samcoupe.dsk
	rm $(BUILD_PATH)/FULL_samcoupe.bin
	
lambda_test:
	$(Z88DK_PATH)$(MYZ88DK) +lambda \
	-vn -D__LAMBDA__ \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o  $(BUILD_PATH)/TEST_lambda.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_lambda.prg		
	
nc100_test:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc100 \
	-DNO_INIT_GRAPHICS \
	-D__NC100__ \
	-DNO_PRINT \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DANIMATE_PLAYER \
	$(FAST_TEST_OPTS) \
	-create-app -o$(BUILD_PATH)/TEST_nc100.bin \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_nc100.bin	

c128_z80_test:	
	$(Z88DK_PATH)$(MYZ88DK) +c128 \
	-lndos -subtype=disk \
	-D__C128_Z80__ -DFORCE_XSIZE=40 \
	$(FAST_TEST_OPTS) \
	-DFORCE_CONIO_LIB \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(TEST_FILES) \
	-create-app
	$(TOOLS_PATH)/generic/c1541 -format "crosschase,0" d64 TEST_c128_z80.d64
	$(TOOLS_PATH)/generic/c1541 -attach TEST_c128_z80.d64 -write a.ldr
	$(TOOLS_PATH)/generic/c1541 -attach TEST_c128_z80.d64 -write a
	mv TEST_c128_z80.d64 $(BUILD_PATH)/
	rm A.LDR
	rm A
	rm a.bin
	
###############################################################################


zx81_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-compiler=sdcc \
	-vn \
	-D__ZX81__ -DTINY_GAME \
	-DALT_SLEEP \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-lndos \
	-create-app -o  $(BUILD_PATH)/TEST_ZSDCC_zx81.prg \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_ZSDCC_zx81.prg
	

aquarius_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +aquarius \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc \
	-compiler=sdcc \
	$(FAST_TEST_OPTS) \
	-vn \
	-DALT_PRINT -D__AQUARIUS__ -DTINY_GAME \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-lndos -o TEST_ZSDCC_aquarius -create-app \
	-DMEMORY_MAPPED \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/aquarius/aquarius_init_graphics.c \
	$(TEST_FILES) 
	rm $(SOURCE_PATH)/../TEST_ZSDCC_aquarius
	mv $(SOURCE_PATH)/../TEST_ZSDCC_aquarius.caq $(BUILD_PATH)
	mv $(SOURCE_PATH)/../_TEST_ZSDCC_aquarius.caq $(BUILD_PATH)	


vz200_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +vz -vn \
	-DTINY_GAME \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc \
	-compiler=sdcc \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-D__VZ__ -clib=ansi \
	-DLESS_TEXT \
	-DNO_BLINKING \
	-DNO_RANDOM_LEVEL \
	-DNO_DEAD_GHOSTS \
	-DNO_SET_SCREEN_COLORS \
	-DNO_STATS \
	-DNO_INITIAL_SCREEN \
	-DNO_MESSAGE \
	-lndos \
	-create-app -o $(BUILD_PATH)/TEST_ZSDCC_vz200.vz \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_ZSDCC_vz200.cas


spectrum_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +zx -startup=1 -zorg=24055 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc -clib=sdcc_iy \
	-DNO_SLEEP -DLESS_TEXT -DTINY_GAME -vn  -D__SPECTRUM__ \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-create-app -o $(BUILD_PATH)/TEST_ZSDCC_spectrum.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_ZSDCC_spectrum_CODE.bin 
	rm $(BUILD_PATH)/TEST_ZSDCC_spectrum_UNASSIGNED.bin


svi_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	-compiler=sdcc \
	-zorg=49152 \
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-vn -lndos \
	-D__SVI__ \
	-DSOUNDS \
	-DALT_SLEEP \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-create-app \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(TEST_FILES) 
	mv a.cas $(BUILD_PATH)/TEST_ZSDCC_svi.cas
	rm a.bin
	
	
vg5k_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-compiler=sdcc \
	--reserve-regs-iy \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-DNO_BLINKING \
	-vn -D__VG5K__ \
	-DLESS_TEXT \
	-DSOUNDS \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-create-app -o $(BUILD_PATH)/TEST_ZSDCC_vg5k.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_ZSDCC_vg5k.prg


mc1000_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 -compiler=sdcc \
	-subtype=gaming -pragma-define:ansicolumns=32 \
	-DLESS_TEXT \
	-DNO_BLINKING \
	-DNO_HINTS \
	-clib=ansi \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-D__MC1000__ -DSOUNDS \
	-DALT_SLEEP \
	-vn  -lndos -create-app -Cz--audio \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(TEST_FILES) 
	mv a.wav $(BUILD_PATH)/TEST_ZSDCC_mc1000.wav
	rm a.bin
	rm a.cas


c128_z80_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +c128 \
	-compiler=sdcc \
	-lndos -subtype=disk \
	-D__C128_Z80__ -DFORCE_XSIZE=40 \
	-DFORCE_CONIO_LIB \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	$(TEST_FILES) \
	-create-app
	$(TOOLS_PATH)/generic/c1541 -format "crosschase,0" d64 TEST_c128_z80.d64
	$(TOOLS_PATH)/generic/c1541 -attach TEST_c128_z80.d64 -write a.ldr
	$(TOOLS_PATH)/generic/c1541 -attach TEST_c128_z80.d64 -write a
	mv TEST_c128_z80.d64 $(BUILD_PATH)/
	rm A.LDR
	rm A
	rm a.bin

	
####################################################################################################################
	
# DEBUG	

	
oric1_16k_full: 
	$(CC65_PATH)$(MYCC65) -Or -Cl \
	-DANIMATE_PLAYER -DFULL_GAME \
	-DLESS_TEXT -DNO_SLEEP -DNO_BORDER -DSIMPLE_STRATEGY -DNO_BLINKING -DNO_HINTS -DNO_STATS -DFORCE_GHOSTS_NUMBER=6 \
	-DFLAT_ENEMIES -DNO_DEAD_GHOSTS \
	-t atmos \
	-DMEMORY_MAPPED \
	-DFORCE_NARROW \
	--config $(CFG_PATH)/cc65/atmos_better_tap.cfg \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atmos/atmos_redefined_characters_init_graphics.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_oric1_16k_less_text.tap	
	

pet_light:
	$(CC65_PATH)$(MYCC65) -Or -t pet -Cl \
	-DLESS_TEXT -DNO_MESSAGE -DNO_BLINKING -DNO_DEAD_GHOSTS -DNO_SET_SCREEN_COLORS -DNO_RANDOM_LEVEL -DFORCE_GHOSTS_NUMBER=8 \
	$(LIGHT_FILES) \
	-o $(BUILD_PATH)/LIGHT_pet.prg

# TODO: Adapt code to work with -compiler=sdcc
# $(ZSDCC_OPTS)
# $(SCCZ80_OPTS) -zorg=18941 -vn 
#  -DNO_RANDOM_LEVEL
# -DLESS_TEXT -DNO_SLEEP
vg5k_light: 
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	$(SCCZ80_OPTS) \
	-vn \
	-D__VG5K__ -DSOUNDS  \
	-DASM_DISPLAY \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_vg5k.prg \
	$(CROSS_LIB_PATH)/z88dk/vg5k/vg5k_graphics.c  \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_vg5k.prg	
	
	
cpc_no_udg:
	$(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) -DANIMATE_PLAYER -vn  -clib=ansi \
	-D__CPC__ -DSOUNDS -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-pragma-define:REGISTER_SP=-1 \
	-lndos -create-app -o $(BUILD_PATH)/FULL_cpc_no_udg.prg \
	$(SOURCE_PATH)/z88dk/cpc/cpc_graphics.c  \
	$(SOURCE_PATH)/z88dk/psg/psg_sounds.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_no_udg.cpc  $(BUILD_PATH)/FULL_cpc_no_udg.cdt
	rm $(BUILD_PATH)/FULL_cpc_no_udg.cpc 
	rm $(BUILD_PATH)/FULL_cpc_no_udg.prg	


zx80_8k:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 $(SCCZ80_OPTS) -vn \
	-D__ZX80__ -DROUND_ENEMIES -DTINY_GAME \
	-DTURN_BASED \
	-lndos -create-app -o  $(BUILD_PATH)/TINY_zx80_8k.prg \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_zx80_8k.prg


zx81_8k:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-vn \
	-D__ZX81__ -DTINY_GAME -DROUND_ENEMIES \
	-DALT_SLEEP \
	-DMACRO_SLEEP \
	-lndos \
	-create-app -o  $(BUILD_PATH)/TINY_zx81_8k.prg \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_zx81_8k.prg


msx_conio_32k:
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) \
	-DSOUNDS -DANIMATE_PLAYER \
	-create-app -vn -DFULL_GAME -D__MSX__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-clib=ansi \
	-create-app -o $(BUILD_PATH)/FULL_msx_conio_32k.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_msx_conio_32k.prg 	

# g800:
	# $(Z88DK_PATH)$(MYZ88DK) +g800 $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	# -D__SRR__ -vn \
	# -DFULL_GAME -DSOUNDS \
	# -DEND_SCREEN -DBETWEEN_LEVEL -DNO_WAIT \
	# -DCONIO_VT52 \
	# -lndos \
	# -clib=g850b \
	# -create-app \
	# $(SOURCE_PATH)/end_screen.c \
	# $(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	# $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	# $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	# $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	# $(SOURCE_PATH)/main.c
	# mv a.rom $(BUILD_PATH)/FULL_g800.rom
		

atari_no_color_16k_full: 
	$(CC65_PATH)$(MYCC65) -O -Cl -t atari \
	-DFULL_GAME \
	-DNO_HINTS \
	-DNO_BLINKING \
	-DLESS_TEXT \
	-DFLAT_ENEMIES \
	-DALT_SLEEP \
	-DNO_RANDOM_LEVEL \
	-DNO_MESSAGE \
	-DNO_PRINT \
	$(SOURCE_PATH)/sleep_macros.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_atari_no_color_16k.xex	
	
atari_no_color_16k: 
	$(CC65_PATH)$(MYCC65) -O -Cl -t atari \
	-DLESS_TEXT \
	-DNO_BLINKING \
	-DNO_RANDOM_LEVEL \
	-DNO_PRINT \
	-DNO_MESSAGE \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c  -o \
	$(BUILD_PATH)/LIGHT_atari_no_color_16k.xex

# pv1000:
	# $(Z88DK_PATH)$(MYZ88DK) +pv1000 $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	# -D__SRR__ -vn \
	# -DZ88DK_JOYSTICK \
	# -DFULL_GAME -DSOUNDS \
	# -DEND_SCREEN -DBETWEEN_LEVEL -DNO_WAIT \
	# -DCONIO_VT52 \
	# -lndos \
	# -create-app \
	# $(SOURCE_PATH)/end_screen.c \
	# $(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	# $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	# $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	# $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	# $(SOURCE_PATH)/main.c
	# mv a.rom $(BUILD_PATH)/FULL_pv1000.rom


nc100_sprites_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc100 \
	-D__NC100__ \
	-DZ88DK_SPRITES \
	-DTINY_GAME \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DANIMATE_PLAYER \
	-create-app -o$(BUILD_PATH)/TINY_nc100_sprites.bin \
	$(SOURCE_PATH)/z88dk_sprites/z88dk_sprites_graphics.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_nc100_sprites.bin
	cp $(BUILD_PATH)/TINY_nc100_sprites.rom /cygwin/c/Emulation/nc100em-1.2-2014/nc100.card

	
einstein_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -leinstein \
	-D__EINSTEIN__ \
	-DFORCE_CONIO_LIB \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-clib=ansi \
	-create-app -o$(BUILD_PATH)/TINY_einstein.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_einstein.bin
	

	


mc1000_16k_light:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 $(SCCZ80_OPTS) \
	-pragma-define:ansicolumns=32 -subtype=gaming -clib=ansi -D__MC1000__ -DSOUNDS -vn \
	 \
	-lndos -create-app -Cz--audio \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
	$(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv a.wav $(BUILD_PATH)/LIGHT_mc1000_16k.wav
	rm a.bin
	rm a.cas


# nc200_sprites_tiny:
	# $(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc200 \
	# -D__NC200__ \
	# -DNO_PRINT \
	# -DZ88DK_SPRITES \
	# -DTINY_GAME \
	# -DLESS_TEXT \
	# -DNO_SLEEP \
	# -DNO_WAIT \
	# -DANIMATE_PLAYER \
	# -create-app -o$(BUILD_PATH)/TINY_nc200_sprites.bin \
	# $(SOURCE_PATH)/z88dk_sprites/z88dk_sprites_graphics.c \
	# $(SOURCE_PATH)/display_macros.c \
	# $(SOURCE_PATH)/enemy.c \
	# $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	# $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	# $(SOURCE_PATH)/main.c
	# rm $(BUILD_PATH)/TINY_nc200_sprites.bin


eg2k_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 \
	-subtype=eg2000disk \
	-lndos \
	-lm \
	-D__TRS80__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-DALT_PRINT \
	-create-app \
	$(SOURCE_PATH)/trs80/trs80_graphics.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	mv a.cmd $(BUILD_PATH)/TINY_eg2k.cmd	

	
kc_tiny_putc4x6:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap \
	-v \
	-D__KC__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-DZ88DK_PUTC4X6 \
	-DCONIO \
	-DALT_PRINT \
	-DNO_INIT_GRAPHICS \
	-create-app -o$(BUILD_PATH)/TINY_kc_putc4x6_.bin \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/z88dk_putc4x6_alt_print.c
	rm $(BUILD_PATH)/TINY_kc_putc4x6_.bin
	
	
	
kc_tiny_sprites:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap \
	-D__KC__ \
	-DZ88DK_SPRITES \
	-DTINY_GAME \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DNO_MESSAGE \
	-DNO_STATS \
	-DANIMATE_PLAYER \
	-create-app -o$(BUILD_PATH)/TINY_kc_sprites.bin \
	-DNO_INIT_GRAPHICS \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_kc_sprites.bin	
	
	
kc_tiny_udg:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap $(SCCZ80_OPTS) \
	-D__KC__ \
	-DTINY_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(TINY_FILES) \
	-create-app -o$(BUILD_PATH)/TINY_kc.bin
	rm $(BUILD_PATH)/TINY_kc.bin

kc_light_udg:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap $(SCCZ80_OPTS) \
	-D__KC__ \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(LIGHT_FILES) \
	-create-app -o$(BUILD_PATH)/LIGHT_kc.bin
	rm $(BUILD_PATH)/LIGHT_kc.bin
	
	
kc_udg:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap $(SCCZ80_OPTS) \
	-D__KC__ \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_kc.bin
	rm $(BUILD_PATH)/FULL_kc.bin

rx78_udg:
	$(Z88DK_PATH)$(MYZ88DK) +rx78 $(SCCZ80_OPTS) \
	-D__RX78__ \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_rx78.bin
	rm $(BUILD_PATH)/FULL_rx78.bin
	rm $(BUILD_PATH)/FULL_rx78_DATA.bin
	rm $(BUILD_PATH)/FULL_rx78_BSS.bin
	

# UDG can be done but differently
pv1000_light:
	$(Z88DK_PATH)$(MYZ88DK) +pv1000 $(SCCZ80_OPTS) \
	-D__PV1000__ \
	-DZ88DK_JOYSTICK \
	-DCONIO_VT52 \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-lndos \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(LIGHT_FILES) \
	-create-app -o$(BUILD_PATH)/LIGHT_pv1000.bin
	rm $(BUILD_PATH)/LIGHT_pv1000.bin	
	rm $(BUILD_PATH)/LIGHT_pv1000_DATA.bin	
	rm $(BUILD_PATH)/LIGHT_pv1000_BSS.bin	
	
# UDG can be done but differently
pv1000:
	$(Z88DK_PATH)$(MYZ88DK) +pv1000 \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-D__PV1000__ \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-lndos \
	-DLESS_TEXT \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_pv1000.bin
	rm $(BUILD_PATH)/FULL_pv1000.bin
	rm $(BUILD_PATH)/FULL_pv1000_DATA.bin
	rm $(BUILD_PATH)/FULL_pv1000_BSS.bin


pv2000_udg:
	$(Z88DK_PATH)$(MYZ88DK) +pv2000 \
	$(SCCZ80_OPTS) \
	-D__PV1000__ \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-DLESS_TEXT \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_pv2000.bin
	rm $(BUILD_PATH)/FULL_pv2000.bin
	rm $(BUILD_PATH)/FULL_pv2000_DATA.bin
	rm $(BUILD_PATH)/FULL_pv2000_BSS.bin

	
	
	
coleco_udg:
	$(Z88DK_PATH)$(MYZ88DK) +coleco $(SCCZ80_OPTS) \
	-D__COLECO__ \
	-DZ88DK_JOYSTICK \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_coleco.bin
	rm $(BUILD_PATH)/FULL_coleco.bin
	rm $(BUILD_PATH)/FULL_coleco_DATA.bin	
	rm $(BUILD_PATH)/FULL_coleco_BSS.bin	


	
multi8_tiny_udg:
	$(Z88DK_PATH)$(MYZ88DK) +multi8 $(SCCZ80_OPTS) \
	-D__MULTI8__ \
	-DTINY_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(TINY_FILES) \
	-create-app -o$(BUILD_PATH)/TINY_multi8.bin
	rm $(BUILD_PATH)/TINY_multi8.bin		


multi8_light_udg:
	$(Z88DK_PATH)$(MYZ88DK) +multi8 $(SCCZ80_OPTS) \
	-D__MULTI8__ \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(LIGHT_FILES) \
	-create-app -o$(BUILD_PATH)/LIGHT_multi8.bin
	rm $(BUILD_PATH)/LIGHT_multi8.bin		
	
multi8_udg:
	$(Z88DK_PATH)$(MYZ88DK) +multi8 \
	$(SCCZ80_OPTS) \
	$(CROSS_LIB_PATH)/boot/multi8/bootstrap.c \
	-create-app -o bootstrap
	rm bootstrap
	$(Z88DK_PATH)$(MYZ88DK) +multi8 -subtype=64k \
	$(SCCZ80_OPTS) \
	-D__MULTI8__ \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-DNO_HINTS \
	-DSIMPLE_STRATEGY \
	-DLESS_TEXT \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app -o code
	rm code
	cat bootstrap.cas code.cas > $(BUILD_PATH)/FULL_multi8.cas
	rm bootstrap.cas
	rm code.cas
	
spc1000_udg:
	$(Z88DK_PATH)$(MYZ88DK) +spc1000 $(SCCZ80_OPTS) \
	-D__SPC1000__ \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DANIMATE_PLAYER \
	-DUDG_GRAPHICS \
	-clib=default -pragma-redirect:fputc_cons=fputc_cons_generic \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_spc1000.bin
	rm $(BUILD_PATH)/FULL_spc1000.bin
	

	
# -o$(BUILD_PATH)/TINY_abc800.bin 	
abc800_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +abc800 -zorg=40000 \
	-D__ABC800__ \
	-DCONIO_ADM3A \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-o$(BUILD_PATH)/TINY_abc800.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c


# cpc_tiny:
	# $(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) -DANIMATE_PLAYER -vn  -clib=ansi \
	# -D__CPC__ 	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	# -DCPCRSLIB \
	# -pragma-define:REGISTER_SP=-1 \
	# -lndos -create-app -o 	$(BUILD_PATH)/TINY_cpc.prg \
	# $(TOOLS_PATH)/cpcrslib/cpc_Chars.asm \
	# $(TOOLS_PATH)/cpcrslib/cpc_Chars8.asm \
	# $(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c \
	# $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	# $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	# $(SOURCE_PATH)/main.c
	# $(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/TINY_cpc.cpc  $(BUILD_PATH)/TINY_cpc.cdt
	# rm $(BUILD_PATH)/TINY_cpc.cpc 
	# rm $(BUILD_PATH)/TINY_cpc.prg			


cpc_cpcrslib:
	$(Z88DK_PATH)$(MYZ88DKASM) -v \
	-x$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lib \
	@$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib.lst	
	$(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) \
	-pragma-define:REGISTER_SP=-1 \
	-DANIMATE_PLAYER -DSOUNDS -DFULL_GAME -clib=ansi -D__CPC__ -DCPCRSLIB -DBETWEEN_LEVEL -DEND_SCREEN \
	-l$(SOURCE_PATH)/../tools/cpcrslib/cpcrslib -lndos \
	-create-app -o $(BUILD_PATH)/FULL_cpc_cpcrslib.prg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/psg/psg_sounds.c \
	$(SOURCE_PATH)/cpc/cpc_cpcrslib_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_cpcrslib.cpc  $(BUILD_PATH)/FULL_cpc_cpcrslib.cdt
	rm $(BUILD_PATH)/FULL_cpc_cpcrslib.cpc 
	rm $(BUILD_PATH)/FULL_cpc_cpcrslib.prg
	

ncurses_turn_based_tiny:
	$(_CC) -D__NCURSES__ \
	-DTINY_GAME \
	-DTURN_BASED \
	-DALT_PRINT \
	$(TINY_FILES) \
	-lncurses
	mv a.exe $(BUILD_PATH)/TINY_ncurses.exe
	
sms_libctest:
	$(Z88DK_PATH)$(MYZ88DK) +sms \
	-vn -lndos \
	-create-app \
	-o $(BUILD_PATH)/sms_libctest.sms \
	$(SOURCE_PATH)/../experiments/libctest.c

m5_sounds:
	$(Z88DK_PATH)$(MYZ88DK) +m5 -create-app \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/sounds.c
	rm -f a.bin
	rm -f A_BSS.bin
	rm -f A_DATA.bin
	mv a.rom $(BUILD_PATH)/m5_sounds.rom

mz2500_getchar:
	$(Z88DK_PATH)$(MYZ88DK) +mz2500 -create-app \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/getchar_test.c
	rm -f a.bin
	mv a.2D $(BUILD_PATH)/mz2500_getchar.2D
	
spectrum_sounds:
	$(Z88DK_PATH)$(MYZ88DK) +zx -create-app \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/sounds.c
	rm -f a.bin
	rm -f a_BANK_7.bin
	mv a.tap $(BUILD_PATH)/spectrum_sounds.tap
	
	
sms_hello:
	$(Z88DK_PATH)$(MYZ88DK) +sms -startup=1 -clib=new -O3 \
	-vn -pragma-include:$(CFG_PATH)/z88dk/zpragma_sms.inc \
	-create-app	\
	$(SOURCE_PATH)/../experiments/z88dk_hello.c 
	rm a_BSS.bin
	rm a_CODE.bin
	rm a_DATA.bin
	rm a_UNASSIGNED.bin
	mv a.sms $(BUILD_PATH)


abc80_hello:
	$(Z88DK_PATH)$(MYZ88DK) +abc80 \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/hello.c
	perl $(TOOLS_PATH)/z88dk/abc80/bin2bac.pl $(SOURCE_PATH)/../a.bin > a.bac	
	# $(TOOLS_PATH)/z88dk/abc80/bin2bac2$(COMPILEDEXT) a.bin $(BUILD_PATH)/abc80_hello.bac 49200
	rm a.bin	
	
abc800_hello:
	$(Z88DK_PATH)$(MYZ88DK) +abc800 -Ca --reloc-info \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/hello.c
	perl $(TOOLS_PATH)/z88dk/abc80/bin2bac.pl -800 $(SOURCE_PATH)/../a.bin $(SOURCE_PATH)/../a.reloc > a.bac
	mv a.bac $(BUILD_PATH)/abc800_hello.bac
	rm a.reloc
	rm a.bin
	
rex_hello:
	$(Z88DK_PATH)$(MYZ88DK) +rex \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/hello.c \
	-create-app	
	
mtx_hello:
	$(Z88DK_PATH)$(MYZ88DK) +mtx \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/hello.c \
	-create-app	
	mv a.wav $(BUILD_PATH)/hello.wav
	mv a $(BUILD_PATH)/hello.mtx
	
	
c128_hello:
	$(Z88DK_PATH)$(MYZ88DK) +c128 \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/hello.c \
	-create-app

# -pragma-redirect:getk=getk_inkey
trs80_test_getch:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	-lm -create-app \
	-pragma-redirect:getk=getk_inkey \
	-vn -lndos \
	$(SOURCE_PATH)/../../notes/tests/trs80_test.c

vg5k_hello:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k -compiler=sdcc \
	-vn -lndos \
	$(SOURCE_PATH)/../experiments/vg5k_hello.c \
	-create-app

sms_chicken:
	$(Z88DK_PATH)$(MYZ88DK) +sms \
	-vn -lndos \
	-create-app \
	-o $(BUILD_PATH)/sms_chicken.sms \
	$(SOURCE_PATH)/../experiments/chicken/chicken.c \
	$(SOURCE_PATH)/../experiments/chicken/chicken_graphics.asm	

sms_joy_test:
	$(Z88DK_PATH)$(MYZ88DK) +sms -clib=sdcc_iy -startup=17 -v \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_sms.inc \
	-create-app \
	$(SOURCE_PATH)/../experiments/sms_joy_test.c \
	-o $(BUILD_PATH)/sms_joy_test.bin
	rm $(BUILD_PATH)/sms_joy_test_CODE.bin
	rm $(BUILD_PATH)/sms_joy_test_DATA.bin
	rm $(BUILD_PATH)/sms_joy_test_BSS.bin
	rm $(BUILD_PATH)/sms_joy_test_UNASSIGNED.bin	
	
sms_tiny_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +sms -clib=sdcc_iy -startup=17 -v \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_sms.inc \
	-D__SMS__ \
	-DNO_COLOR \
	-DTINY_GAME \
	-DSTDLIB \
	-DLESS_TEXT \
	-create-app \
	-DBUFFERED \
	-DTURN_BASED \
	-DALT_PRINT \
	-DALT_CLEAR_SCREEN \
	-DNO_SLEEP \
	-DNO_WAIT \
	-o $(BUILD_PATH)/TINY_sms_buffered.bin \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/sms/sms_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_sms_buffered_CODE.bin
	rm $(BUILD_PATH)/TINY_sms_buffered_DATA.bin
	rm $(BUILD_PATH)/TINY_sms_buffered_BSS.bin
	rm $(BUILD_PATH)/TINY_sms_buffered_UNASSIGNED.bin
	

# 	-pragma-need=ansiterminal -pragma-define:ansipixels=240 -pragma-define:ansicolumns=40 
# -DNO_BORDER


creativision_32k:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	-DNO_SLEEP -DLESS_TEXT -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(SOURCE_PATH)/../cfg/cc65/creativision-32k.cfg \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	-o $(BUILD_PATH)/FULL_creativision_32k.bin

zx81_light:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-vn \
	-D__ZX81__ -DNO_SLEEP -DLESS_TEXT \
	-lndos \
	-create-app -o  $(BUILD_PATH)/LIGHT_zx81_8k.prg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/skull.c $(SOURCE_PATH)/bullet.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_zx81_8k.prg

osca_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +osca \
	-D__OSCA__ -DNO_SLEEP -DLESS_TEXT \
	-DTINY_GAME \
	-o $(BUILD_PATH)/TINY_osca.bin -vn -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
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
	
# ts2068:
	# $(Z88DK_PATH)$(MYZ88DK) +ts2068 $(SCCZ80_OPTS) \
	# -D__TS2068__ -DEND_SCREEN -DBETWEEN_LEVEL \
	# -clib=ansi -pragma-define:ansicolumns=32 -vn \
	# -DFULL_GAME  -lndos \
	# $(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	# $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c \
	# $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c \
	# $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \
	# -o $(BUILD_PATH)/FULL_ts2068.bin



# ts2068_tiny:
	# $(Z88DK_PATH)$(MYZ88DK) +ts2068 \
	# -D__TS2068__ \
	# -DTINY_GAME \
	# -clib=ansi -pragma-define:ansicolumns=32 -vn \
	# -o $(BUILD_PATH)/FULL_ts2068.bin -lndos \
	# $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	# $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	# $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	
z1013_light:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 $(SCCZ80_OPTS) -clib=ansi \
	-vn -lndos \
	-D__Z1013__  \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \
	-create-app -o
	mv $(BUILD_PATH)/../A.Z80 $(BUILD_PATH)/LIGHT_z1013.z80
	rm $(BUILD_PATH)/../a.bin


z1013_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 $(SCCZ80_OPTS) -clib=ansi \
	-D__Z1013__ -vn   -DTINY_GAME -DNO_SLEEP \
	-lndos \
	$(SOURCE_PATH)/sleep_macros.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \
	-create-app -o
	mv $(BUILD_PATH)/../A.Z80 $(BUILD_PATH)/TINY_z1013.z80
	rm $(BUILD_PATH)/../a.bin
	
	
# import as data into ram at 32768 - call 32768
samcoupe_light:
	$(Z88DK_PATH)$(MYZ88DK) +sam -O0 \
	-D__SAM__  \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	 -o $(BUILD_PATH)/LIGHT_samcoupe.bin -lndos \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	
	
samcoupe_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +sam -O0 \
	-D__SAM__ -DTINY_GAME \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	 -o $(BUILD_PATH)/TINY_samcoupe.bin -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	
	
zx81_16k_sccz80:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	$(SCCZ80_OPTS) \
	-DTURN_BASED \
	-vn \
	-D__ZX81__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -create-app -o  $(BUILD_PATH)/FULL_zx81_16k_sccz80.prg \
	$(FULL_FILES) \
	$(SOURCE_PATH)/end_screen.c
	rm $(BUILD_PATH)/FULL_zx81_16k_sccz80.prg


supervision_tiny:
	$(CC65_PATH)$(MYCC65) -t supervision \
	-DTINY_GAME -DLESS_TEXT -DNO_WAIT -DNO_SLEEP -DALT_PRINT -DNO_CHASE \
	-o $(BUILD_PATH)/TINY_supervision.sv \
	$(SOURCE_PATH)/sleep_macros.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/supervision/supervision_graphics.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	
supervision_test:
	$(CC65_PATH)$(MYCC65) -t supervision \
	$(SOURCE_PATH)/../experiments/supervision_test.c \
	-o $(BUILD_PATH)/supervision_test.sv

sound_test:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	-clib=ansi -pragma-define:ansicolumns=32 -vn -lndos \
	-create-app -o $(BUILD_PATH)/sound_test \
	experiments/sound_test.c


sc3000_tiny: 
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	$(SCCZ80_OPTS) \
	-pragma-need=ansiterminal \
	-DTINY_GAME -DNO_SLEEP -DLESS_TEXT -D__GAL__ -DNO_RANDOM_LEVEL -DALT_PRINT -DNO_MESSAGE -DNO_STATS \
	-clib=ansi \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/TINY_sc3000.prg \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	#rm $(BUILD_PATH)/TINY_sc3000.prg


pointerToFunction:
	$(CC65_PATH)$(MYCC65) -t pce \
	$(SOURCE_PATH)/../experiments/pointerToFunction.c \
	-o $(BUILD_PATH)/pointerToFunctions.pce
	

# -DNO_MESSAGE \
# 	-compiler=sdcc \
#	$(ZSDCC_OPTS) \
# -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL
# -DALT_PRINT
# -pragma-include:$(SOURCE_PATH)/../cfg/z88dk/zpragma.inc

gal_6k_sccz80: 
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	$(SCCZ80_OPTS) \
	-pragma-need=ansiterminal \
	-DTINY_GAME -DNO_SLEEP -DLESS_TEXT -D__GAL__ -DNO_RANDOM_LEVEL -DALT_PRINT -DNO_MESSAGE -DNO_STATS \
	-vn -lndos -create-app -Cz--audio \
	-o  $(BUILD_PATH)/TINY_galaksija_6k_sccz80.prg \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_galaksija_6k_sccz80.prg
	#rm $(BUILD_PATH)/TINY_galaksija_6k_sccz80.wav
	

	
# 	--opt-code-size 
gal_6k:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-pragma-need=ansiterminal \
	--reserve-regs-iy \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc \
	-DTINY_GAME \
	-DNO_SLEEP -DLESS_TEXT \
	-D__GAL__ \
	-DALT_MOVE \
	-DNO_RANDOM_LEVEL \
	-DNO_SET_SCREEN_COLORS \
	-DNO_STATS \
	-DNO_INITIAL_SCREEN \
	-DNO_PRINT \
	-DALT_PRINT \
	-DNO_MESSAGE \
	-DFORCE_BOMBS_NUMBER=4 \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DNO_DEAD_GHOSTS \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-v -lndos -create-app -Cz--audio -o  $(BUILD_PATH)/TINY_galaksija_6k.prg \
	$(TINY_FILES)
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
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
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
# hello_vec:
	# cmoc -o $(BUILD_PATH)/hello_vec.bin $(SOURCE_PATH)/../experiments/hello_vec.c
	
pet_8k_LIGHT:
	$(CC65_PATH)$(MYCC65) -O -t pet -DLESS_TEXT -DNO_SLEEP $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_pet_8k.prg


c16_16k_no_udg:
	$(CC65_PATH)$(MYCC65) -O -t c16 -Cl -DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DSOUNDS --config $(SOURCE_PATH)/../cfg/cc65/c16-16k_plus.cfg $(SOURCE_PATH)/c264/c264_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_c16_16k_no_udg.prg



	
lambda_8k:
	$(Z88DK_PATH)$(MYZ88DK) +lambda $(SCCZ80_OPTS) -vn -D__LAMBDA__ -DTINY_GAME -DNO_SET_SCREEN_COLORS \
	-DLESS_TEXT -DNO_SLEEP -lndos \
	-create-app -o  $(BUILD_PATH)/TINY_lambda_8k.prg \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_lambda_8k.prg	
	
# -----------------------------------------------------------------------------------------------
	

atari5200_light:
	$(CC65_PATH)$(MYCC65) -O -t atari5200 $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_atari5200.rom	
	

vg5k_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k -O0 -vn -DTINY_GAME -D__VG5K__ -lndos -create-app -o $(BUILD_PATH)/TINY_vg5k.prg \
	$(SOURCE_PATH)/vg5k/vg5k_graphics.c $(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
	$(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_vg5k.prg


spectrum_clib_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +zx $(SCCZ80_OPTS) -clib=ansi -pragma-define:ansicolumns=32 -vn                           -DCLIB_ANSI -DNO_SLEEP -DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL -DLESS_TEXT -DTINY_GAME -D__SPECTRUM__ -lndos -create-app -o $(BUILD_PATH)/TINY_spectrum_clib.prg  $(SOURCE_PATH)/spectrum/spectrum_graphics.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_spectrum_clib.prg
	rm $(BUILD_PATH)/TINY_spectrum_clib_BANK_7.bin	
	
	
conio_nascom:
	$(Z88DK_PATH)$(MYZ88DK) +nascom experiments/coniotest.c -create-app -o  $(BUILD_PATH)/conio.nas
			
conio:
	$(CC65_PATH)$(MYCC65) -O -t gamate experiments/conio.c -o  $(BUILD_PATH)/conio.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/conio.bin

c64_joy-test:
	$(CC65_PATH)$(MYCC65) -O -t c64 -DJOYSTICK_DRIVER=c64_hitjoy_joy experiments/joy-test.c -o $(BUILD_PATH)/joy-test.prg
	
	
joy-test:
	$(CC65_PATH)$(MYCC65) -O -t gamate experiments/joy-test.c -o  $(BUILD_PATH)/joy-test.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/joy-test.bin




# -subtype=gaming
mc1000_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 -DDEBUG -DTINY_GAME $(SCCZ80_OPTS) -pragma-define:ansicolumns=32  -clib=ansi -D__MC1000__ -vn  -lndos -create-app -Cz--audio $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv a.wav $(BUILD_PATH)/TINY_mc1000.wav
	rm a.bin
	rm a.cas	

# -DFULL_GAME
vic20_exp_3k_NO_GFX: 
	$(CC65_PATH)$(MYCC65) -O -Cl -t vic20 -DNO_SLEEP -DLESS_TEXT -DNO_SET_SCREEN_COLORS   -DTINY_GAME --config $(SOURCE_PATH)/../cfg/cc65/vic20-3k.cfg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_vic20_exp_3k.prg
	
creativision_16k_2:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	-DNO_SLEEP -DLESS_TEXT \
	--config $(SOURCE_PATH)/../cfg/cc65/creativision-16k_2.cfg \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c \
	--code-name CODE2 \
	$(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	-o $(BUILD_PATH)/LIGHT_creativision_16k.bin	
	dd if=$(BUILD_PATH)/LIGHT_creativision_16k.bin ibs=1 count=8192 of=$(BUILD_PATH)/LIGHT_creativision_16k_LOW.bin
	dd if=$(BUILD_PATH)/LIGHT_creativision_16k.bin ibs=1 skip=8192 of=$(BUILD_PATH)/LIGHT_creativision_16k_HIGH.bin
	rm $(BUILD_PATH)/LIGHT_creativision_16k.bin 
	cat $(BUILD_PATH)/LIGHT_creativision_16k_HIGH.bin $(BUILD_PATH)/LIGHT_creativision_16k_LOW.bin > $(BUILD_PATH)/LIGHT_creativision_16k.bin
	rm $(BUILD_PATH)/LIGHT_creativision_16k_LOW.bin
	rm $(BUILD_PATH)/LIGHT_creativision_16k_HIGH.bin
			

pce_light:
	$(CC65_PATH)$(MYCC65) -O -t pce -Cl \
	--config $(CFG_PATH)cc65/pce_8k_less_stack.cfg \
	-DNO_SLEEP -DLESS_TEXT -DNO_COLOR -DNO_RANDOM_LEVEL \
	-DNO_MESSAGE -DNO_BLINKING -DNO_INITIAL_SCREEN -DNO_SET_SCREEN_COLORS -DNO_STATS \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(LIGHT_FILES) \
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
	$(CC65_PATH)cc65 -D__NES__ $(SOURCE_PATH)/game_text.c 
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
	#$(CC65_PATH)$(MYCC65) -O -t nes -DTINY_GAME --config $(SOURCE_PATH)/nes/nes.cfg -DNES_COLOR $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/nes/reset.s $(SOURCE_PATH)/nes/nes_graphics.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_nes_color.nes
	# -C $(SOURCE_PATH)/nes/nes.cfg
	
nes_16k:
	$(CC65_PATH)$(MYCC65) -O -t nes $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_nes.nes

osca:
	$(Z88DK_PATH)$(MYZ88DK) +osca $(SCCZ80_OPTS) \
	-clib=ansi -D__OSCA__ -vn \
	-DFULL_GAME \
	-DSOUNDS  -lndos \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app  \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm a.bin
	mv A.EXE $(BUILD_PATH)/FULL_OSCA.EXE
	
	




# ti86:
	# $(Z88DK_PATH)$(MYZ88DK) +ti86 \
	# $(SCCZ80_OPTS) -D__TI86__ \
	# -clib=ansi -pragma-define:ansicolumns=32 \
	# -vn \
	# -DFULL_GAME  \
	# -lndos \
	# -create-app -o $(BUILD_PATH)/FULL_ti86.bin  \
	# $(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	# $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	# $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	# $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	# $(SOURCE_PATH)/main.c
	

# ti86_tiny:
	# $(Z88DK_PATH)$(MYZ88DK) +ti86 \
	# $(SCCZ80_OPTS) -D__TI86__ \
	# -clib=ansi -pragma-define:ansicolumns=32 \
	# -vn \
	# -DTINY_GAME -DLESS_TEXT  \
	# -lndos \
	# -create-app -o $(BUILD_PATH)/TINY_ti86.bin  \
	# $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c \
	# $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	# $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	# $(SOURCE_PATH)/main.c



# it may work
mtx_16k:
	$(Z88DK_PATH)$(MYZ88DK) +mtx -startup=2 $(SCCZ80_OPTS) -D__MTX__ -clib=ansi -pragma-define:ansicolumns=32 -vn   -lndos -create-app -o LIGHT.bin $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv LIGHT $(BUILD_PATH)
	mv LIGHT.bin $(BUILD_PATH)
	mv LIGHT.wav $(BUILD_PATH)
	
	
lambda_light:
	$(Z88DK_PATH)$(MYZ88DK) +lambda -vn -D__LAMBDA__ -lndos -create-app -o  $(BUILD_PATH)/LIGHT_lambda.prg \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_lambda.prg	
	
msx_no_color_16k:
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS)  -zorg=49200 -DSOUNDS -create-app -vn -D__MSX__ -lndos -create-app -o $(BUILD_PATH)/LIGHT_msx_no_color_16k.prg $(SOURCE_PATH)/msx/msx_graphics.c $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/LIGHT_msx_no_color_16k.prg 	


	
creativision_hello:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	$(SOURCE_PATH)/../experiments/hello.c \
	-o $(BUILD_PATH)/creativision_hello.bin
	
gal_hello:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-pragma-need=ansiterminal \
	-pragma-include:$(SOURCE_PATH)/../cfg/z88dk/zpragma.inc \
	-vn -lndos -create-app -Cz--audio \
	$(SOURCE_PATH)/../experiments/hello.c \
	-o  $(BUILD_PATH)/hello.prg
	rm $(BUILD_PATH)/hello.prg
	
	
ti85_hello:
	$(Z88DK_PATH)$(MYZ88DK) +ti85 $(SOURCE_PATH)/../experiments/hello.c \
	-lndos \
	-create-app -o $(BUILD_PATH)/ti85_hello.bin
	rm $(BUILD_PATH)/ti85_hello.bin
	
cpc_hello:
	$(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) $(SOURCE_PATH)/../experiments/hello.c \
	-lndos \
	-create-app -o $(BUILD_PATH)/hello.prg
	$(SOURCE_PATH)/../tools/2cdt.exe -n -r cross_chase $(BUILD_PATH)/hello.cpc $(BUILD_PATH)/hello.cdt
	rm $(BUILD_PATH)/hello.cpc 
	rm $(BUILD_PATH)/hello.prg	

pps_vt52_test:
	$(Z88DK_PATH)$(MYZ88DK) +pps $(SCCZ80_OPTS) \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	$(SOURCE_PATH)/../experiments/vt52_test.c \
	-lndos -vn
	
g800_vt52_test:
	$(Z88DK_PATH)$(MYZ88DK) +g800 $(SCCZ80_OPTS) \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	$(SOURCE_PATH)/../experiments/vt52_test.c \
	-lndos -vn  \
	-create-app -o	
	
srr_vt52_test:
	$(Z88DK_PATH)$(MYZ88DK) +srr $(SCCZ80_OPTS) \
	-pragma-redirect:fputc_cons=fputc_cons_generic \
	$(SOURCE_PATH)/../experiments/vt52_test.c \
	-lndos -vn  \
	-create-app -o
	rm a.srr 
	mv a.wav $(BUILD_PATH)/srr_vt52_test.wav

eg2k_hello:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -subtype=eg2000disk -create-app $(SCCZ80_OPTS) \
	$(SOURCE_PATH)/../experiments/hello.c
	mv a.cmd $(BUILD_PATH)/eg2k_hello.cmd
	
z1013_hello:
	$(Z88DK_PATH)$(MYZ88DK) +z1013 $(SCCZ80_OPTS) $(SOURCE_PATH)/../experiments/hello.c \
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
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) -DSOUNDS -DANIMATE_PLAYER -vn -DMSX_MODE1 -DFULL_GAME -D__MSX__ -lndos -subtype=msxdos -o $(BUILD_PATH)/FULL_msx_color_32k.com $(SOURCE_PATH)/msx/msx_graphics.c $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	
