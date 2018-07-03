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
# --max-allocs-per-node200000
#-SO3 --max-allocs-per-node200000

TOOLS_PATH ?= ../tools

CFG_PATH ?= $(SOURCE_PATH)/../../cfg/

NO_CHASE_FILES ?= \
	$(CROSS_LIB_PATH)/display/display_macros.c \
	$(CROSS_LIB_PATH)/display/init_images.c \
	$(CROSS_LIB_PATH)/display/graphics.c \
	$(CROSS_LIB_PATH)/input/input_macros.c \
	$(CROSS_LIB_PATH)/text/text_macros.c \
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


COCO_OPTS_TINY  ?= -O0 -D__CMOC__ -DTINY_GAME

COCO_OPTS_LIGHT ?= -O0 -D__CMOC__

COCO_OPTS       ?= -O0 -DALT_CLEAR_SCREEN -D__CMOC__ -DMEMORY_MAPPED -DNO_INIT_GRAPHICS -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL -DCMOC_RAND_FIX


# -DNO_SLEEP

# ------------------------------------------------------------------------------------------
#CC65
# 

# 	$(CROSS_LIB_PATH)/display/cc65/atari/atari_mode1_redefined_chars_init_graphics.c 

atari_color: 
	$(CC65_PATH)$(MYCC65) -O -t atari \
	-DREDEFINED_CHARS -DFULL_GAME -DATARI_MODE1 -DSOUNDS -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(CFG_PATH)/cc65/atari_mode1_redefined_chars.cfg \
	-DMEMORY_MAPPED \
	$(CROSS_LIB_PATH)/display/cc65/atari/disable_setcursor.s \
	$(CROSS_LIB_PATH)/display/cc65/atari/atari_mode1_redefined_chars_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/atari/atari_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_atari_color.xex

atari_no_color: 
	$(CC65_PATH)$(MYCC65) -O -t atari \
	-DFULL_GAME -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(CROSS_LIB_PATH)/sound/cc65/atari/atari_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_atari_no_color.xex


# --config $(SOURCE_PATH)/../cfg/cc65/atari5200_less_stack.cfg
atari5200: 
	$(CC65_PATH)$(MYCC65) -O -t atari5200 \
	-DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_atari5200.rom


atmos: 
	$(CC65_PATH)$(MYCC65)  -O  -DSOUNDS -DREDEFINED_CHARS -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-t atmos \
	-DALT_MOVE \
	--config $(CFG_PATH)/cc65/atmos_better_tap.cfg \
	-DMEMORY_MAPPED \
	$(CROSS_LIB_PATH)/display/cc65/atmos/atmos_redefined_characters_init_graphics.c \
	$(FULL_FILES) \
	$(SOURCE_PATH)/end_screen.c \
	-o $(BUILD_PATH)/FULL_atmos_and_oric1_48k.tap


oric1_16k: 
	$(CC65_PATH)$(MYCC65)  -O -D__ORIC1__ -DSOUNDS -DREDEFINED_CHARS \
	-t atmos \
	--config $(CFG_PATH)/cc65/atmos_better_tap.cfg \
	-DMEMORY_MAPPED \
	-DALT_MOVE \
	$(CROSS_LIB_PATH)/display/cc65/atmos/atmos_redefined_characters_init_graphics.c \
	$(LIGHT_FILES) \
	-o $(BUILD_PATH)/LIGHT_oric1_16k.tap

	
# 	$(CC65_PATH)$(MYCC65) -O -Cl -t vic20 -m 746e3d1.map -Wl -vm 
	
vic20_unexpanded:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 \
	-DVIC20_UNEXPANDED \
	-DNO_PRINT \
	-DNO_SLEEP \
	-DLESS_TEXT \
	-DNO_SET_SCREEN_COLORS \
	-DTINY_GAME \
	-DNO_RANDOM_LEVEL \
	-DNO_MESSAGE \
	-DNO_STATS \
	-DNO_INITIAL_SCREEN \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DNO_DEAD_GHOSTS \
	-DFORCE_BOMBS_NUMBER=4 \
	--config $(CFG_PATH)/cc65/vic20_unexpanded.cfg  \
	-DMEMORY_MAPPED \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/cc65/vic20/vic20_init_graphics.c \
	-o $(BUILD_PATH)/TINY_vic20_unexpanded.prg
	
vic20_exp_3k:
	$(CC65_PATH)$(MYCC65) -Or  -t vic20 \
	-DALT_PRINT -DREDEFINED_CHARS -DLESS_TEXT -DNO_SET_SCREEN_COLORS -DNO_DEAD_GHOSTS -DFORCE_GHOSTS_NUMBER=8 -DROUND_ENEMIES -DTINY_GAME -DSOUNDS \
	--config $(CFG_PATH)/cc65/vic20-3k_GFX.cfg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds_3k.c \
	$(CROSS_LIB_PATH)/display/cc65/vic20/udc_3k.s  \
	$(CROSS_LIB_PATH)/display/cc65/vic20/vic20_graphics_3k.c \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_vic20_exp_3k.prg


vic20_exp_8k_full:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 --config $(CFG_PATH)/cc65/vic20-8k.cfg \
	-DFULL_GAME \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DSIMPLE_STRATEGY \
	-DNO_BLINKING \
	-DLESS_TEXT	\
	-DALT_PRINT \
	-DNO_HINTS \
	-DSOUNDS \
	-DALT_SLEEP \
	-DREDEFINED_CHARS \
	-DMEMORY_MAPPED \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds.c \
	$(CROSS_LIB_PATH)/display/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/display/memory_mapped/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_vic20_exp_8k_NO_GFX.prg		


vic20_exp_8k: 
	$(CC65_PATH)$(MYCC65) -Or  -t vic20  -DSOUNDS -DREDEFINED_CHARS --config $(CFG_PATH)/cc65/vic20-8k_GFX.cfg \
	-DCONIO -DVIC20_EXP_8K \
	$(CROSS_LIB_PATH)/display/cc65/vic20/udc.s \
	$(CROSS_LIB_PATH)/display/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c  $(CROSS_LIB_PATH)/text/text_macros.c \
	$(SOURCE_PATH)/skull.c $(SOURCE_PATH)/ghost.c $(CROSS_LIB_PATH)/input/input_macros.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/bullet.c \
	$(SOURCE_PATH)/move_player.c \
	$(CROSS_LIB_PATH)/display/init_images.c \
	$(CROSS_LIB_PATH)/display/display_macros.c  \
	--code-name CODE2 \
	$(CROSS_LIB_PATH)/display/graphics.c \
	$(SOURCE_PATH)/item.c $(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/LIGHT_vic20_exp_8k.prg

	
vic20_exp_16k: 
	$(CC65_PATH)$(MYCC65) -Or -t vic20 -DREDEFINED_CHARS -DFULL_GAME -DSOUNDS -DEND_SCREEN -DBETWEEN_LEVEL \
	--config $(CFG_PATH)/cc65/vic20-16k_GFX.cfg \
	-DCONIO -DVIC20_EXP_16K \
	$(CROSS_LIB_PATH)/display/cc65/vic20/udc.s \
	$(CROSS_LIB_PATH)/display/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_vic20_exp_16k.prg

	
c64: 
	$(CC65_PATH)$(MYCC65) -O -t c64 \
	-DFULL_GAME -DREDEFINED_CHARS -DSOUNDS -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(CFG_PATH)/cc65/c64_GFXat0xC000.cfg  \
	-DCBM_SCREEN_CODES \
	-DMEMORY_MAPPED \
	$(CROSS_LIB_PATH)/display/cc65/c64/graphics.s \
	$(CROSS_LIB_PATH)/display/cc65/c64/c64_init_graphics.c \
	$(CROSS_LIB_PATH)/display/memory_mapped/memory_mapped_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c64.prg
	$(TOOLS_PATH)/generic/exomizer sfx basic $(BUILD_PATH)/FULL_c64.prg -o $(BUILD_PATH)/FULL_c64_exomized.prg
	python $(TOOLS_PATH)/cc65/c64/prg2crt.py $(BUILD_PATH)/FULL_c64_exomized.prg  $(BUILD_PATH)/FULL_c64_exomized.crt
	rm $(BUILD_PATH)/FULL_c64.prg


c64_8k_cart: 
	$(CC65_PATH)$(MYCC65) -O -t c64 \
	-DFULL_GAME -DREDEFINED_CHARS -DSOUNDS \
	-DLESS_TEXT \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DEND_SCREEN \
	-DALT_PRINT \
	-DBETWEEN_LEVEL \
	-DFLAT_ENEMIES \
	-DCBM_SCREEN_CODES \
	-DMEMORY_MAPPED \
	--config $(CFG_PATH)/cc65/c64_GFXat0xC000.cfg \
	$(CROSS_LIB_PATH)/display/cc65/c64/graphics.s \
	$(CROSS_LIB_PATH)/display/cc65/c64/c64_init_graphics.c \
	$(CROSS_LIB_PATH)/display/memory_mapped/memory_mapped_alt_print.c \
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
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c128_40col.prg

c128_80col: 
	$(CC65_PATH)$(MYCC65) -O -t c128  \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-DFORCE_GHOSTS_NUMBER=9 \
	$(CROSS_LIB_PATH)/display/cc65/c128/c128_80col_init_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c128_80col.prg

# -DNO_SLEEP -DLESS_TEXT -DNO_RANDOM_LEVEL
# -DSOUNDS $(SOURCE_PATH)/c264/c264_sounds.c
# -Cl 
c16_16k: 
	$(CC65_PATH)$(MYCC65) -Or -t c16 \
	--config $(CFG_PATH)/cc65/c16-16k.cfg \
	-DREDEFINED_CHARS  -DSOUNDS \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(CROSS_LIB_PATH)/display/cc65/c264/c264_graphics.c  \
	$(LIGHT_FILES) \
	-o $(BUILD_PATH)/LIGHT_c16_16k.prg
	
c16_32k: 
	$(CC65_PATH)$(MYCC65) -Or -t c16 --config $(CFG_PATH)/cc65/c16-32k.cfg \
	-DREDEFINED_CHARS -DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/cc65/c264/c264_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c16_32k.prg

	
c16_16k_full: 
	$(CC65_PATH)$(MYCC65) -Or -Cl -t c16 \
	--config $(CFG_PATH)/cc65/c16_16k_less_stack.cfg \
	-DFULL_GAME \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DLESS_TEXT	\
	-DSOUNDS \
	-DALT_PRINT \
	-DREDEFINED_CHARS \
	-DMEMORY_MAPPED \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(CROSS_LIB_PATH)/display/cc65/c264/c264_init_graphics.c \
	$(CROSS_LIB_PATH)/display/memory_mapped/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c16_16k_NO_GFX.prg	

	
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
	$(CROSS_LIB_PATH)/display/cc65/apple2enh/apple2enh_80col_init_graphics.c \
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
	$(CC65_PATH)$(MYCC65) -O -t gamate --config $(CFG_PATH)/cc65/gamate_reduced_stack.cfg \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_gamate.bin
	$(TOOLS_PATH)/cc65/gamate/gamate-fixcart $(BUILD_PATH)/FULL_gamate.bin


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
	-DLESS_TEXT -DNO_SLEEP \
	-DNO_SET_SCREEN_COLORS \
	-DFLAT_ENEMIES \
	-DNO_BLINKING \
	-DNO_RANDOM_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	--config $(CFG_PATH)/cc65/creativision-8k_less_stack.cfg \
	$(LIGHT_FILES) \
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
	-DCONIO \
	-DNO_INIT_GRAPHICS \
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
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/cc65/supervision/supervision_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_supervision.sv
	
# ------------------------------------------------------------------------------------------
#Z88DK
# 	-DNO_INITIAL_SCREEN -DNO_RANDOM_LEVEL \
# 	$(ZSDCC_OPTS) \

aquarius_exp_4k:
	$(Z88DK_PATH)$(MYZ88DK) +aquarius \
	-clib=ansi \
	-pragma-include:$(SOURCE_PATH)/../cfg/z88dk/zpragma.inc \
	-compiler=sdcc \
	-opt-code-size \
	$(ZSDCC_OPTS) \
	-vn \
	-D__AQUARIUS__ -DTINY_GAME \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-lndos -o TINY_aquarius_exp_4k -create-app \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES)
	rm $(SOURCE_PATH)/../TINY_aquarius_exp_4k
	mv $(SOURCE_PATH)/../TINY_aquarius_exp_4k.caq $(BUILD_PATH)
	mv $(SOURCE_PATH)/../_TINY_aquarius_exp_4k.caq $(BUILD_PATH)


aquarius_exp_16k: 
	$(Z88DK_PATH)$(MYZ88DK) +aquarius -clib=ansi $(SCCZ80_OPTS) -vn \
	-DSOUNDS -D__AQUARIUS__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-create-app	\
	-o FULL_aquarius_exp_16k
	rm $(SOURCE_PATH)/../FULL_aquarius_exp_16k
	mv $(SOURCE_PATH)/../FULL_aquarius_exp_16k.caq $(BUILD_PATH)
	mv $(SOURCE_PATH)/../_FULL_aquarius_exp_16k.caq $(BUILD_PATH)


vz200_8k:
	$(Z88DK_PATH)$(MYZ88DK) +vz -v \
	-DTINY_GAME \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc \
	-compiler=sdcc \
	-opt-code-size \
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

	
vz200_24k: 
	$(Z88DK_PATH)$(MYZ88DK) +vz $(SCCZ80_OPTS) -vn \
	-DSOUNDS -D__VZ__ -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	-clib=ansi -lndos -create-app -o  $(BUILD_PATH)/FULL_vz200_24k.vz \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_vz200_24k.cas



# 	$(ZSDCC_OPTS) 
#   -opt-code-size 
#	-DSOUNDS
#	-DNO_INITIAL_SCREEN
# 	-DNO_MESSAGE
# 	-DFORCE_NARROW
vg5k:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-opt-code-size \
	--reserve-regs-iy \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-DNO_BLINKING \
	-vn -DFULL_GAME -D__VG5K__ \
	-DLESS_TEXT \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DFORCE_BOMBS_NUMBER=4 \
	-DSOUNDS \
	-create-app -o $(BUILD_PATH)/FULL_vg5k_less_text.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/vg5k/vg5k_graphics.c \
	$(FULL_FILES) 
	rm $(BUILD_PATH)/FULL_vg5k_less_text.prg	


# $(SCCZ80_OPTS) -zorg=18941 -vn
vg5k_exp_16k:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	$(SCCZ80_OPTS) \
	-DSOUNDS -vn -DFULL_GAME -D__VG5K__ -DBETWEEN_LEVEL -DEND_SCREEN -DASM_DISPLAY \
	-lndos -create-app -o $(BUILD_PATH)/FULL_vg5k_exp_16k.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/vg5k/vg5k_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) 
	rm $(BUILD_PATH)/FULL_vg5k_exp_16k.prg	


ace_exp_16k:
	$(Z88DK_PATH)$(MYZ88DK) +ace $(SCCZ80_OPTS) \
	-D__ACE__ -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	-clib=ansi -o full -Cz--audio -create-app \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	cp full.wav $(BUILD_PATH)/FULL_ace_exp_16k.wav
	rm full.wav
	rm full.tap
	rm full


zx80_16k_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 $(SCCZ80_OPTS) -vn \
	-D__ZX80__ -DFULL_GAME \
	-DEND_SCREEN \
	-DTURN_BASED \
	-DALT_SLEEP \
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

	
# 	$(ZSDCC_OPTS) 
	
zx81_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	$(ZSDCC_OPTS) \
	-compiler=sdcc \
	-vn \
	-D__ZX81__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_16k.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_zx81_16k.prg
	
	
lambda_16k:
	$(Z88DK_PATH)$(MYZ88DK) +lambda $(SCCZ80_OPTS) \
	-vn -D__LAMBDA__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o  $(BUILD_PATH)/FULL_lambda_16k.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_lambda_16k.prg		
	


cpc:
	$(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) -DREDEFINED_CHARS -v -clib=ansi \
	-D__CPC__ -DSOUNDS -DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCPCRSLIB \
	-pragma-define:REGISTER_SP=-1 \
	-lndos -create-app -o 	$(BUILD_PATH)/FULL_cpc.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/cpc/cpc_Chars.asm \
	$(CROSS_LIB_PATH)/display/z88dk/cpc/cpc_Chars8.asm \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/display/z88dk/cpc/cpc_cpcrslib_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	$(TOOLS_PATH)/z88dk/cpc/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc.cpc  $(BUILD_PATH)/FULL_cpc.cdt
	$(TOOLS_PATH)/z88dk/cpc/cpcxfsw -nd FULL_cpc.dsk
	$(TOOLS_PATH)/z88dk/cpc/cpcxfsw FULL_cpc.dsk -p $(BUILD_PATH)/FULL_cpc.cpc xchase
	mv FULL_cpc.dsk $(BUILD_PATH)/
	rm $(BUILD_PATH)/FULL_cpc.cpc 
	rm $(BUILD_PATH)/FULL_cpc.prg			


cpc_joystick:
	$(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) -DREDEFINED_CHARS -vn  -clib=ansi \
	-D__CPC__ \
	-D__CPC_JOYSTICK__ \
	-DSOUNDS -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCPCRSLIB \
	-pragma-define:REGISTER_SP=-1 \
	-lndos -create-app -o 	$(BUILD_PATH)/FULL_cpc_joystick.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/cpc/cpc_Chars.asm \
	$(CROSS_LIB_PATH)/display/z88dk/cpc/cpc_Chars8.asm \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/display/z88dk/cpc/cpc_cpcrslib_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	$(TOOLS_PATH)/z88dk/cpc/2cdt.exe -n -r cross_chase $(BUILD_PATH)/FULL_cpc_joystick.cpc  $(BUILD_PATH)/FULL_cpc_joystick.cdt
	rm -rf FULL_cpc_joystick.dsk
	$(TOOLS_PATH)/z88dk/cpc/cpcxfsw -nd FULL_cpc_joystick.dsk
	$(TOOLS_PATH)/z88dk/cpc/cpcxfsw FULL_cpc_joystick.dsk -p $(BUILD_PATH)/FULL_cpc_joystick.cpc xchase
	mv FULL_cpc_joystick.dsk $(BUILD_PATH)/
	cp $(TOOLS_PATH)/z88dk/cpc/nocart/*.rom .
	$(TOOLS_PATH)/z88dk/cpc/nocart/nocart.exe  $(BUILD_PATH)/FULL_cpc_joystick.dsk  $(BUILD_PATH)/FULL_gx4000.cpr   -c 'run"xchase'
	rm os.rom
	rm amsdos.rom
	rm basic.rom
	rm $(BUILD_PATH)/FULL_cpc_joystick.cpc 
	rm $(BUILD_PATH)/FULL_cpc_joystick.prg		

		
msx_16k:
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) -zorg=49200 \
	-DSOUNDS -DREDEFINED_CHARS -create-app -vn -DMSX_VPOKE -D__MSX__ -lndos \
	-create-app -o $(BUILD_PATH)/LIGHT_msx_16k.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/msx/msx_graphics.c $(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_msx_16k.prg 	


msx_32k:
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) \
	-DSOUNDS -DREDEFINED_CHARS \
	-create-app -vn -DMSX_VPOKE -DFULL_GAME -D__MSX__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_msx_32k.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/msx/msx_graphics.c	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_msx_32k.prg 


msx_32k_rom:
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) \
	-DSOUNDS -DREDEFINED_CHARS -vn -DMSX_VPOKE -DFULL_GAME -D__MSX__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -subtype=rom \
	-o $(BUILD_PATH)/FULL_msx_32k.rom \
	$(SOURCE_PATH)/end_screen.c \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/msx/msx_graphics.c $(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_msx_32k_BSS.bin
	rm $(BUILD_PATH)/FULL_msx_32k_DATA.bin


svi_318_mode0:
	$(Z88DK_PATH)$(MYZ88DK) +svi $(SCCZ80_OPTS) -zorg=49200 -vn -lndos \
	-D__SVI__ -DMSX_MODE0 \
	-DSOUNDS \
	-create-app \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/display/z88dk/svi/svi_graphics.c \
	$(LIGHT_FILES)
	mv a.cas $(BUILD_PATH)/LIGHT_svi_318_mode0.cas
	rm a.bin

#
svi_318:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	$(ZSDCC_OPTS) \
	-compiler=sdcc \
	-zorg=49152 \
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-vn -lndos \
	-D__SVI__ \
	-DSOUNDS \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c	\
	$(LIGHT_FILES)
	mv a.cas $(BUILD_PATH)/LIGHT_svi_318.cas
	rm a.bin
	

sc3000_16k:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	$(SCCZ80_OPTS) \
	-clib=ansi \
	-DCONIO -DNO_INIT_GRAPHICS \
	-pragma-define:ansicolumns=32 \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/LIGHT_sc3000_16k.prg \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_sc3000_16k.prg
	rm $(BUILD_PATH)/LIGHT_sc3000_16k.tap


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

	
# It hangs if compiled with sdcc.
# syntax error if compiled with sccz80
# $(SOURCE_PATH)/svi/svi_graphics.c	
svi_328:
	$(Z88DK_PATH)$(MYZ88DK) +svi $(SCCZ80_OPTS) \
	-clib=ansi -pragma-define:ansicolumns=32 -vn -lndos \
	-DSOUNDS \
	-DFULL_GAME -D__SVI__ -DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app \	
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c  \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.cas $(BUILD_PATH)/FULL_svi_328.cas
	rm a.bin

sharp_mz:
	$(Z88DK_PATH)$(MYZ88DK) +mz $(SCCZ80_OPTS) \
	-D__MZ__ -clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_sharp_mz.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_sharp_mz.prg
	mv $(BUILD_PATH)/FULL_sharp_mz.mzt $(BUILD_PATH)/FULL_sharp_mz.mzf
	
microbee_16k:
	$(Z88DK_PATH)$(MYZ88DK) +bee $(SCCZ80_OPTS) \
	-D__BEE__ -clib=ansi -vn -DSOUNDS  \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/LIGHT_microbee_16k.prg  \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_microbee_16k.prg
	
microbee_32k:
	$(Z88DK_PATH)$(MYZ88DK) +bee $(SCCZ80_OPTS) \
	-D__BEE__ -clib=ansi -vn -DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
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


mtx:
	$(Z88DK_PATH)$(MYZ88DK) +mtx -startup=2 $(SCCZ80_OPTS) \
	-D__MTX__ -clib=ansi -pragma-define:ansicolumns=32 -create-app -o FULL.bin -vn \
	-DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm FULL.bin
	mv FULL.wav $(BUILD_PATH)/FULL_mtx.wav
	mv FULL $(BUILD_PATH)/FULL_mtx.mtx
	
abc80_16k:
	$(Z88DK_PATH)$(MYZ88DK) +abc80 -lm -subtype=hex -zorg=49200 $(SCCZ80_OPTS) \
	-DCONIO -DNO_INIT_GRAPHICS \
	-D__ABC80__ -clib=ansi -vn -DSOUNDS  -lndos -create-app -o a \
	$(LIGHT_FILES)
	rm a
	mv a.ihx $(BUILD_PATH)/LIGHT_abc80.ihx 
	
abc80_32k:
	$(Z88DK_PATH)$(MYZ88DK) +abc80 -lm -subtype=hex -zorg=49200 $(SCCZ80_OPTS) \
	-D__ABC80__ -clib=ansi -vn -DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o a \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm a
	mv a.ihx $(BUILD_PATH)/FULL_abc80.ihx 

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
spectrum_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx --opt-code-size  -startup=1 -zorg=24055 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc -clib=sdcc_iy \
	-DNO_SLEEP -DLESS_TEXT -DTINY_GAME -v  -D__SPECTRUM__ \
	-create-app -o $(BUILD_PATH)/TINY_spectrum_16k.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/spectrum/spectrum_graphics.c \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_spectrum_16k_CODE.bin 
	rm $(BUILD_PATH)/TINY_spectrum_16k_UNASSIGNED.bin
	
	
# -pragma-redirect:ansifont=_font_8x8_zx_system -pragma-define:ansifont_is_packed=0
spectrum_48k:
	$(Z88DK_PATH)$(MYZ88DK) +zx $(SCCZ80_OPTS) -clib=ansi -v  \
	-pragma-redirect:ansifont=_udg -pragma-define:ansifont_is_packed=0 -pragma-define:ansicolumns=32 \
	-DFULL_GAME -DREDEFINED_CHARS -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL -D__SPECTRUM__ \
	-DALT_MOVE \
	-lndos -create-app \
	-DEXT_GRAPHICS \
	-o $(BUILD_PATH)/FULL_spectrum_48k.prg \
	$(CROSS_LIB_PATH)/display/z88dk/spectrum/udg.asm $(CROSS_LIB_PATH)/display/z88dk/spectrum/spectrum_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_spectrum_48k.prg
	rm $(BUILD_PATH)/FULL_spectrum_48k_BANK_7.bin	

	
spectrum_48k_putc4x6:
	$(Z88DK_PATH)$(MYZ88DK) +zx $(SCCZ80_OPTS) -clib=ansi -vn  \
	-pragma-redirect:ansifont=_udg -pragma-define:ansifont_is_packed=0 -pragma-define:ansicolumns=32 \
	-DFULL_GAME -DREDEFINED_CHARS -DSOUNDS -DCLIB_ANSI -DEND_SCREEN -DBETWEEN_LEVEL -D__SPECTRUM__ \
	-DZ88DK_PUTC4X6 \
	-DNO_SLEEP -DLESS_TEXT -DNO_WAIT \
	-lndos -create-app \
	-o $(BUILD_PATH)/FULL_spectrum_48k_putc4x6.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/spectrum/udg.asm $(CROSS_LIB_PATH)/display/z88dk/spectrum/spectrum_graphics.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	rm $(BUILD_PATH)/FULL_spectrum_48k_putc4x6.prg
	rm $(BUILD_PATH)/FULL_spectrum_48k_putc4x6_BANK_7.bin	
	

pc6001_16k:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 $(SCCZ80_OPTS) -Cz--audio -clib=ansi -subtype=32k \
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
pc6001_32k:
	$(Z88DK_PATH)$(MYZ88DK) +pc6001 $(SCCZ80_OPTS) -Cz--audio -clib=ansi -subtype=32k \
	-D__PC6001__ -v -DFULL_GAME -DSOUNDS  -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o $(BUILD_PATH)/FULL_pc6001_32k.prg \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_pc6001_32k.prg
	rm $(BUILD_PATH)/FULL_pc6001_32k.wav
	mv $(BUILD_PATH)/FULL_pc6001_32k.cas $(BUILD_PATH)/FULL_pc6001_32k.cp6

	
# kbhit KO
# Everything displayed on the same line
nascom_32k:
	$(Z88DK_PATH)$(MYZ88DK) +nascom $(SCCZ80_OPTS) -clib=ansi -vn -lndos \
	-D__NASCOM__ \
	-DSOUNDS \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DNO_WAIT \
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
	-DNO_WAIT \
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
	-DNO_WAIT \
	-DCONIO_ADM3A \
	-DTINY_GAME -DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o$(BUILD_PATH)/TINY_px8.bin \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_px8.bin


px8:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px32k \
	-D__PX8__ \
	-DCONIO_ADM3A \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o$(BUILD_PATH)/FULL_px8.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
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
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_px4_putc4x6.bin

	
px4_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-D__PX4__ \
	-DTINY_GAME \
	-subtype=px4ansi -Cz–-32k \
	-pragma-define:ansicolumns=40 \
	-create-app -o $(BUILD_PATH)/TINY_px4.bin \
	-vn -lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES)
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
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
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
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_px4_putc4x6.bin


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
	-lm -create-app \
	-D__TRS80__ \
	-DCONIO_VT52 \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-DCONIO -DNO_INIT_GRAPHICS \
	-o$(BUILD_PATH)/TINY_trs80.bin \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_trs80.bin
	

trs80:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	-lm -create-app \
	-D__TRS80__ \
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
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/z88dk_sprites/z88dk_sprites_graphics.c \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_kc_sprites.bin	


nc100_sprites_light:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc100 \
	-D__NC100__ \
	-DNO_PRINT \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DREDEFINED_CHARS \
	-create-app -o$(BUILD_PATH)/LIGHT_nc100_sprites.bin \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/z88dk_sprites/z88dk_sprites_graphics.c \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_nc100_sprites.bin


cpm_adm3a_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-DCONIO_ADM3A \
	-D__CPM_80X24__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o$(BUILD_PATH)/TINY_cpm_adm3a.bin \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_cpm_adm3a.bin

	
cpm_vt52_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-DCONIO_VT52 \
	-D__CPM_80X24__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o$(BUILD_PATH)/TINY_cpm_vt52.bin \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_cpm_vt52.bin	
	

cpm_vt100_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-DCONIO_VT100 \
	-D__CPM_80X24__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o$(BUILD_PATH)/TINY_cpm_vt100.bin \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_cpm_vt100.bin	
	

c128_z80_40col:
	$(Z88DK_PATH)$(MYZ88DK) +c128 \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-lndos -subtype=disk \
	-D__C128_Z80__ -DFORCE_XSIZE=40 \
	-DFULL_GAME -DEND_SCREEN -DNO_BLINKING \
	-DFORCE_CONIO \
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
	
	
einstein:
	$(Z88DK_PATH)$(MYZ88DK) +cpm $(SCCZ80_OPTS) -leinstein \
	-D__EINSTEIN__ \
	-DFORCE_CONIO \
	-DFULL_GAME -DLESS_TEXT -DNO_SLEEP -DNO_WAIT \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-clib=ansi \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o$(BUILD_PATH)/FULL_einstein.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_einstein.bin


ti82_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ti82 \
	$(SCCZ80_OPTS) -D__TI82__ \
	-DTURN_BASED -DNO_WAIT \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-DLESS_TEXT -DSIMPLE_STRATEGY -DNO_HINTS -DNO_BLINKING \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app  \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../a.82p $(BUILD_PATH)/FULL_ti82_turn_based.82p
	rm $(SOURCE_PATH)/../a.bin

	
ti82_tiny_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ti82 \
	$(SCCZ80_OPTS) -D__TI82__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DTINY_GAME  \
	-DTURN_BASED -DNO_WAIT \
	-DLESS_TEXT -DSIMPLE_STRATEGY -DNO_HINTS -DNO_BLINKING \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES) \
	-create-app 	
	mv $(SOURCE_PATH)/../a.82p $(BUILD_PATH)/TINY_ti82_turn_based.82p
	rm $(SOURCE_PATH)/../a.bin


ti83_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ti83 \
	$(SCCZ80_OPTS) -D__TI83__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-DTURN_BASED \
	-DNO_WAIT \
	-DLESS_TEXT -DNO_HINTS -DNO_BLINKING -DNO_COLOR \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(FULL_FILES)	
	-create-app \
	mv $(SOURCE_PATH)/../a.83p $(BUILD_PATH)/FULL_ti83_turn_based.83p
	rm $(SOURCE_PATH)/../a.bin

	
ti83_tiny_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ti83 \
	$(SCCZ80_OPTS) -D__TI83__ \
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

	
ti85:
	$(Z88DK_PATH)$(MYZ88DK) +ti85 \
	$(SCCZ80_OPTS) -D__TI85__ \
	-DFORCE_XSIZE=32 \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app  \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../a.85s $(BUILD_PATH)/TINY_ti85.85s	
	rm $(SOURCE_PATH)/../a.bin


ti85_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ti85 \
	$(SCCZ80_OPTS) -D__TI85__ \
	-DFORCE_XSIZE=32 \
	-DTURN_BASED \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app \
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../a.85s $(BUILD_PATH)/FULL_ti85_turn_based.85s	
	rm $(SOURCE_PATH)/../a.bin


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

m5:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	-lm -create-app -Cz--audio -subtype=tape \
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
	

pps:
	$(Z88DK_PATH)$(MYZ88DK) +pps $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__PPS__ -vn \
	-DCONIO_VT52 \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL -DNO_WAIT \
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
	-DEND_SCREEN -DBETWEEN_LEVEL -DNO_WAIT \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	mv a.bin $(BUILD_PATH)/FULL_pps_turn_based.exe

	
# ------------------------------------

	# $(CROSS_LIB_PATH)/display/init_images.c \
	# $(CROSS_LIB_PATH)/display/cc65/c64/c64_init_graphics.c \
	# $(CROSS_LIB_PATH)/display/memory_mapped/memory_mapped_graphics.c \
	# $(CROSS_LIB_PATH)/display/memory_mapped/memory_mapped_alt_print.c \


coco:
	cmoc \
	$(COCO_OPTS) \
	-c $(CROSS_LIB_PATH)/display/display_macros.c
	cmoc \
	$(COCO_OPTS) \
	-c $(CROSS_LIB_PATH)/display/init_images.c	
	cmoc \
	$(COCO_OPTS) \
	-c $(CROSS_LIB_PATH)/display/graphics.c
	cmoc \
	$(COCO_OPTS) \
	-c $(CROSS_LIB_PATH)/display/memory_mapped/memory_mapped_alt_print.c
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
	graphics.o \
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
	graphics.o \
	memory_mapped_alt_print.o \
	ghost.o \
	move_player.o \
	level.o character.o \
	game_text.o text_macros.o \
	strategy.o \
	input_macros.o \
	item.o bullet.o skull.o \
	rocket.o horizontal_missile.o end_screen.o
		

# ------------------------------------

ncurses_turn_based:
	$(_CC) -D__NCURSES__ \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DTURN_BASED \
	-DCONIO  \	
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-lncurses
	mv a.exe $(BUILD_PATH)/FULL_ncurses_turn_based.exe
	
ncurses:
	$(_CC) -D__NCURSES__ \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO  \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-lncurses
	mv a.exe $(BUILD_PATH)/FULL_ncurses.exe
		
		

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
	gamate_targets \
	osic1p_targets \
	pet_targets \
	atari_lynx_targets \
	supervision_targets \
	vic20_targets \
	c264_targets
	
# KO: 	
# m5: out of memory
# sg1000: interrupt
# einstein: 'msx_attr' not defined 

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
	sharp_mz_test \
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
	spectrum_test

totest: \
	samcoupe_test \
	lambda_test \
	nc100_test \
	c128_z80_test

z88dk_test: \
	sccz80_test \
	zsdcc_test	
	
z88dk_targets: \
	pc6001_targets \
	zx81_targets \
	einstein_targets \
	sc3000_targets \
	m5_targets \
 	eg2k_targets \
	pps_targets \
	pv2000_targets \
	srr_targets \
	ti82_targets \
	ti83_targets \
	ti85_targets \
	z1013_targets \
	x1_targets \
	px4_targets \
	px8_targets \
	kc_targets \
	trs80_targets \
	cpm_targets \
	nascom_targets \
	z9001_targets \
	sharp_mz_targets \
	mtx_targets \
	abc80_targets \
	p2000_targets \
	svi_targets \
	msx_targets \
	aquarius_targets \
	vz200_targets \
	microbee_targets \
	gal_targets \
	zx80_targets \
	ace_targets \
	samcoupe_targets \
	lambda_targets \
	nc100_targets \
	c128_z80_targets \
	mc1000_targets \
	vg5k_targets \
	cpc_targets \
	spectrum_targets


cmoc_targets: \
	coco
	
all: cc65_targets z88dk_targets cmoc_targets

clean_chase_obj:
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(SOURCE_PATH)/wincmoc_main.c
	rm -rf $(SOURCE_PATH)/wincmoc_main.asm
	rm -rf $(SOURCE_PATH)/wincmoc_main.coco.bin

clean_lib_obj:
	rm -rf $(CROSS_LIB_PATH)/*.o
	rm -rf $(CROSS_LIB_PATH)/display/*.o
	rm -rf $(CROSS_LIB_PATH)/display/cc65/atari/*.o
	rm -rf $(CROSS_LIB_PATH)/display/cc65/atari_lynx/*.o
	rm -rf $(CROSS_LIB_PATH)/display/cc65/atmos/*.o
	rm -rf $(CROSS_LIB_PATH)/display/cc65/c64/*.o
	rm -rf $(CROSS_LIB_PATH)/display/cc65/c264/*.o
	rm -rf $(CROSS_LIB_PATH)/display/cc65/gamate/*.o
	rm -rf $(CROSS_LIB_PATH)/display/cc65/supervision/*.o
	rm -rf $(CROSS_LIB_PATH)/display/cc65/vic20/*.o
	rm -rf $(CROSS_LIB_PATH)/display/cmoc/*.o
	rm -rf $(CROSS_LIB_PATH)/display/memory_mapped/*.o
	rm -rf $(CROSS_LIB_PATH)/display/wincmoc/*.o
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/aquarius/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/cpc/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/kc/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/msx/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/spectrum/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/svi/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/trs80/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/vg5k/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/z88dk_putc4x6/*.o	
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/z88dk_sprites/*.o
	rm -rf $(CROSS_LIB_PATH)/display/z88dk/zx81/*.o	
	rm -rf $(CROSS_LIB_PATH)/input/*.o
	rm -rf $(CROSS_LIB_PATH)/sleep/*.o
	rm -rf $(CROSS_LIB_PATH)/sound/*.o
	rm -rf $(CROSS_LIB_PATH)/sound/cc65/atari/*.o
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

eg2k_targets: \
	eg2k

pps_targets: \
	pps

pv2000_targets: \
	pv2000

srr_targets: \
	srr

m5_targets: \
	m5_tiny m5

ti82_targets: \
	ti82_turn_based ti82_tiny_turn_based 

ti83_targets: \
	ti83_turn_based ti83_tiny_turn_based

ti8x_targets: \
	ti8x_turn_based ti8x
	
ti85_targets: \
	ti85 ti85_turn_based

einstein_targets: \
	einstein

z1013_targets: \
	z1013
	
x1_targets: \
	x1_tiny x1

px4_targets: \
	px4_tiny px4 px4_putc4x6

px8_targets: \
	px8_tiny px8
	
kc_targets: \
	kc_sprites_tiny
	
trs80_targets: \
	trs80_tiny trs80 eg2k

cpm_targets: \
	cpm_adm3a_tiny cpm_vt52_tiny cpm_vt100_tiny 

nascom_targets: \
	nascom_16k nascom_32k 
	
pc6001_targets: \
	pc6001_16k pc6001_32k 	
	
z9001_targets: \
	z9001_16k z9001_32k

vg5k_targets: \
	vg5k vg5k_exp_16k
	
sc3000_targets: \
	sc3000_16k sc3000_32k sg1000
	
ace_targets: \
	ace_exp_16k

cpc_targets: \
	cpc cpc_joystick
	
mc1000_targets: \
 	mc1000_16k_full mc1000_48k
	
sharp_mz_targets: \
 	sharp_mz
	
mtx_targets: \
	mtx 
	
abc80_targets: \
	abc80_16k abc80_32k
	
p2000_targets: \
	p2000_16k p2000_32k

svi_targets: \
	svi_318 svi_318_mode0 svi_328

msx_targets: \
	msx_16k msx_32k_rom msx_32k

c128_z80_targets: \
	c128_z80_40col c128_z80_40col_turn_based
	
aquarius_targets: \
	aquarius_exp_4k aquarius_exp_16k
	
vz200_targets: \
	vz200_8k vz200_18k
	
microbee_targets: \
	microbee_16k microbee_32k
	
gal_targets: \
	gal_22k
	
zx80_targets: \
	zx80_16k_turn_based
	
zx81_targets: \
	zx81_16k zx81_16k_turn_based
	
spectrum_targets: \
	spectrum_16k spectrum_48k
	
samcoupe_targets: \
	samcoupe
	
lambda_targets: \
	lambda_16k
	
nc100_targets: \
	nc100_sprites_light	
	
	
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
	nes
	
creativision_targets: \
	creativision_8k_tiny creativision_8k_light creativision_16k
	
gamate_targets: \
	gamate
	
atari_lynx_targets: \
	atari_lynx
	
osic1p_targets: \
	osic1p_8k osic1p_32k
	


c128_targets: \
	c128_8502_targets c128_z80_targets

	
	
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
	-DFORCE_CONIO \
	$(FAST_TEST_OPTS) \
	-clib=ansi \
	-create-app -o$(BUILD_PATH)/TEST_einstein.bin \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_einstein.bin

sc3000_test:
	$(Z88DK_PATH)$(MYZ88DK) +sc3000 \
	$(FAST_TEST_OPTS) \
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
	-clib=ansi \
	-pragma-define:ansicolumns=32 \
	-vn -lndos -create-app -Cz--audio \
	-o $(BUILD_PATH)/TEST_sg1000.prg \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_sg1000.prg	
	
m5_test:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	-lm -create-app -Cz--audio -subtype=tape \
	-D__M5__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	$(FAST_TEST_OPTS) \
	-o$(BUILD_PATH)/TEST_m5.bin \
	$(TEST_FILES)
	
ace_test:
	$(Z88DK_PATH)$(MYZ88DK) +ace \
	-D__ACE__ \
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
	$(FAST_TEST_OPTS) \
	-DNO_WAIT \
	-lndos \
	$(TEST_FILES) 
	mv a.bin $(BUILD_PATH)/TEST_pps.exe		
	
pv2000_test:
	$(Z88DK_PATH)$(MYZ88DK) +pv2000 -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__SRR__ -vn \
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
	-DNO_SLEEP \
	$(FAST_TEST_OPTS) \
	-create-app -o $(BUILD_PATH)/TEST_x1.bin -vn -lndos \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_x1.bin
	
px4_test:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	-pragma-define:ansicolumns=40 \
	-pragma-define:ansipixels=240 -pragma-define:ansicolumns=60 \
 	-subtype=px4ansi \
	-D__PX4__ \
	-DNO_WAIT \
	$(FAST_TEST_OPTS) \
	-DZ88DK_PUTC4X6 \
	-DALT_PRINT \
	-Cz–-32k \
	-create-app -o $(BUILD_PATH)/TEST_px4.bin \
	-vn -lndos \
	$(CROSS_LIB_PATH)/display/z88dk/z88dk_putc4x6/z88dk_putc4x6_alt_print.c \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_px4.bin
	
px8_test:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px32k \
	-D__PX8__ \
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
	$(FAST_TEST_OPTS) \
	-DZ88DK_SPRITES \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DREDEFINED_CHARS \
	-create-app -o$(BUILD_PATH)/TEST_kc_sprites.bin \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/z88dk_sprites/z88dk_sprites_graphics.c \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_kc_sprites.bin	
	
trs80_test:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	-lm -create-app \
	-D__TRS80__ \
	$(FAST_TEST_OPTS) \
	-DNO_SLEEP \
	-DCONIO_VT52 \
	-o$(BUILD_PATH)/TEST_trs80.bin \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_trs80.bin	

cpm_test:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	-DCONIO_ADM3A \
	-D__CPM_80X24__ \
	-DNO_SLEEP -DNO_WAIT \
	$(FAST_TEST_OPTS) \
	-create-app -o$(BUILD_PATH)/TEST_cpm.bin \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_cpm.bin
	
nascom_test:
	$(Z88DK_PATH)$(MYZ88DK) +nascom -clib=ansi -vn -lndos \
	-D__NASCOM__ \
	-DSOUNDS \
	$(FAST_TEST_OPTS) \
	-create-app -o $(BUILD_PATH)/TEST_nascom.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_nascom.prg
	
z9001_test:
	$(Z88DK_PATH)$(MYZ88DK) +z9001 -clib=ansi \
	-D__Z9001__ -vn   \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o $(BUILD_PATH)/TEST_z9001.z80 \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_z9001.z80	
	
vg5k_test:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	$(FAST_TEST_OPTS) \
	-DSOUNDS -vn -D__VG5K__ -DASM_DISPLAY \
	-lndos -create-app -o $(BUILD_PATH)/TEST_vg5k.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/vg5k/vg5k_graphics.c \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_vg5k.prg	

cpc_test:	
	$(Z88DK_PATH)$(MYZ88DK) +cpc -DREDEFINED_CHARS -vn  -clib=ansi \
	-D__CPC__ -DSOUNDS \
	$(FAST_TEST_OPTS) \
	-DCPCRSLIB \
	-pragma-define:REGISTER_SP=-1 \
	-DEXT_GRAPHICS \
	-lndos -create-app -o 	$(BUILD_PATH)/TEST_cpc.prg \
	$(CROSS_LIB_PATH)/display/z88dk/cpc/cpc_Chars.asm \
	$(CROSS_LIB_PATH)/display/z88dk/cpc/cpc_Chars8.asm \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(CROSS_LIB_PATH)/display/z88dk/cpc/cpc_cpcrslib_graphics.c \
	$(TEST_FILES)
	$(TOOLS_PATH)/z88dk/cpc/2cdt.exe -n -r cross_chase $(BUILD_PATH)/TEST_cpc.cpc  $(BUILD_PATH)/TEST_cpc.cdt
	$(TOOLS_PATH)/z88dk/cpc/cpcxfsw -nd TEST_cpc.dsk
	$(TOOLS_PATH)/z88dk/cpc/cpcxfsw TEST_cpc.dsk -p $(BUILD_PATH)/TEST_cpc.cpc xchase
	mv TEST_cpc.dsk $(BUILD_PATH)/
	rm $(BUILD_PATH)/TEST_cpc.cpc 
	rm $(BUILD_PATH)/TEST_cpc.prg	

mc1000_test:	
	$(Z88DK_PATH)$(MYZ88DK) +mc1000 \
	-subtype=gaming -pragma-define:ansicolumns=32 \
	$(FAST_TEST_OPTS) \
	-clib=ansi \
	-D__MC1000__ -DSOUNDS \
	-vn  -lndos -create-app -Cz--audio \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(TEST_FILES)
	mv a.wav $(BUILD_PATH)/TEST_mc1000.wav
	rm a.bin
	rm a.cas	

sharp_mz_test:	
	$(Z88DK_PATH)$(MYZ88DK) +mz \
	-D__MZ__ -clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DSOUNDS \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o $(BUILD_PATH)/TEST_sharp_mz.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_sharp_mz.prg
	mv $(BUILD_PATH)/TEST_sharp_mz.mzt $(BUILD_PATH)/TEST_sharp_mz.mzf

mtx_test:
	$(Z88DK_PATH)$(MYZ88DK) +mtx -startup=2 \
	-D__MTX__ -clib=ansi -pragma-define:ansicolumns=32 -create-app -o TEST_mtx.bin -vn \
	-DSOUNDS \
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
	-create-app -o a \
	$(TEST_FILES)
	rm a
	mv a.ihx $(BUILD_PATH)/TEST_abc80.ihx 	
	

p2000_test:
	$(Z88DK_PATH)$(MYZ88DK) +p2000 -clib=ansi -D__P2000__ -vn \
	-DSOUNDS  \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o $(BUILD_PATH)/TEST_p2000.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_p2000.prg	

svi_test:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	-clib=ansi -pragma-define:ansicolumns=32 -vn -lndos \
	-DSOUNDS \
	-D__SVI__ \
	$(FAST_TEST_OPTS) \
	-create-app \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c  \
	$(TEST_FILES)
	mv a.cas $(BUILD_PATH)/TEST_svi.cas
	rm a.bin

msx_test:
	$(Z88DK_PATH)$(MYZ88DK) +msx -zorg=49200 \
	-DSOUNDS -DREDEFINED_CHARS -create-app -vn -DMSX_VPOKE -D__MSX__ -lndos \
	$(FAST_TEST_OPTS) \
	-create-app -o $(BUILD_PATH)/TEST_msx.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/msx/msx_graphics.c \
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_msx.prg 	
	
aquarius_test: 
	$(Z88DK_PATH)$(MYZ88DK) +aquarius -clib=ansi -vn \
	-DSOUNDS -D__AQUARIUS__  -DTINY_GAME \
	$(FAST_TEST_OPTS) \
	-lndos \
	-DEXT_GRAPHICS \
	-o TEST_aquarius -create-app \
	$(CROSS_LIB_PATH)/display/z88dk/aquarius/aquarius_graphics.c \
	$(TEST_FILES)
	rm $(SOURCE_PATH)/../TEST_aquarius
	mv $(SOURCE_PATH)/../TEST_aquarius.caq $(BUILD_PATH)
	mv $(SOURCE_PATH)/../_TEST_aquarius.caq $(BUILD_PATH)	
	
vz200_test:
	$(Z88DK_PATH)$(MYZ88DK) +vz -vn \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc \
	-D__VZ__ -clib=ansi \
	$(FAST_TEST_OPTS) \
	-lndos \
	-create-app -o $(BUILD_PATH)/TEST_vz200.vz \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_vz200.cas	
	
	
microbee_test:
	$(Z88DK_PATH)$(MYZ88DK) +bee \
	-D__BEE__ -clib=ansi -vn -DSOUNDS  \
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o $(BUILD_PATH)/TEST_microbee.prg  \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_microbee.prg


gal_test:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-pragma-need=ansiterminal \
	-D__GAL__ \
	$(FAST_TEST_OPTS) \
	-vn -lndos -create-app -o  $(BUILD_PATH)/TEST_galaksija.prg \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_galaksija.prg	
	rm $(BUILD_PATH)/TEST_galaksija.wav
	
zx80_test:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 -vn \
	-D__ZX80__ \
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
	-DREDEFINED_CHARS -DSOUNDS -DCLIB_ANSI -D__SPECTRUM__ \
	-lndos -create-app \
	$(FAST_TEST_OPTS) \
	-o $(BUILD_PATH)/TEST_spectrum.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/spectrum/udg.asm \
	$(CROSS_LIB_PATH)/display/z88dk/spectrum/spectrum_graphics.c \
	$(TEST_FILES) 
	rm $(BUILD_PATH)/TEST_spectrum.prg
	rm $(BUILD_PATH)/TEST_spectrum_BANK_7.bin	
	
samcoupe_test:
	$(Z88DK_PATH)$(MYZ88DK) +sam \
	-D__SAM__ \
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
	$(FAST_TEST_OPTS) \
	-lndos -create-app -o  $(BUILD_PATH)/TEST_lambda.prg \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_lambda.prg		
	
nc100_test:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc100 \
	-D__NC100__ \
	-DNO_PRINT \
	-DZ88DK_SPRITES \
	-DLESS_TEXT \
	-DNO_SLEEP \
	-DNO_WAIT \
	-DREDEFINED_CHARS \
	$(FAST_TEST_OPTS) \
	-create-app -o$(BUILD_PATH)/TEST_nc100.bin \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/z88dk_sprites/z88dk_sprites_graphics.c \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_nc100.bin	

c128_z80_test:	
	$(Z88DK_PATH)$(MYZ88DK) +c128 \
	-lndos -subtype=disk \
	-D__C128_Z80__ -DFORCE_XSIZE=40 \
	$(FAST_TEST_OPTS) \
	-DFORCE_CONIO \
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
	-DALT_PRINT -D__AQUARIUS__ -DTINY_GAME -DEXT_GRAPHICS \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-lndos -o TEST_ZSDCC_aquarius -create-app \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/aquarius/aquarius_graphics.c \
	$(TEST_FILES) 
	rm $(SOURCE_PATH)/../TEST_ZSDCC_aquarius
	mv $(SOURCE_PATH)/../TEST_ZSDCC_aquarius.caq $(BUILD_PATH)
	mv $(SOURCE_PATH)/../_TEST_ZSDCC_aquarius.caq $(BUILD_PATH)	


vz200_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +vz -vn \
	-DTINY_GAME \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc \
	-compiler=sdcc \
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
	-create-app -o $(BUILD_PATH)/TEST_ZSDCC_spectrum.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/spectrum/spectrum_graphics.c \
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
	-DEXT_GRAPHICS \
	$(FAST_TEST_OPTS) \
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
	$(FAST_TEST_OPTS) \
	-create-app -o $(BUILD_PATH)/TEST_ZSDCC_vg5k.prg \
	-DEXT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/z88dk/vg5k/vg5k_graphics.c \
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
	-DFORCE_CONIO \
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

	
spectrum_16k_light:
	$(Z88DK_PATH)$(MYZ88DK) +zx --opt-code-size  -startup=1 -zorg=24055 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma.inc -clib=sdcc_iy \
	-DNO_SLEEP -DLESS_TEXT -vn  -D__SPECTRUM__ \
	-create-app -o $(BUILD_PATH)/LIGHT_spectrum_16k.prg \
	$(CROSS_LIB_PATH)/z88dk/spectrum/spectrum_graphics.c \
	$(LIGHT_FILES)
	rm $(BUILD_PATH)/LIGHT_spectrum_16k_CODE.bin 
	rm $(BUILD_PATH)/LIGHT_spectrum_16k_UNASSIGNED.bin
	
	
oric1_16k_full: 
	$(CC65_PATH)$(MYCC65)  -Or -Cl \
	-DREDEFINED_CHARS -DFULL_GAME \
	-DLESS_TEXT -DNO_MESSAGE -DNO_SLEEP -DSIMPLE_STRATEGY -DNO_BLINKING -DNO_HINTS -DNO_STATS -DFORCE_GHOSTS_NUMBER=8 \
	-t atmos \
	--config $(CFG_PATH)/cc65/atmos_better_tap.cfg \
	$(CROSS_LIB_PATH)/cc65/atmos/atmos_redefined_characters.c \
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
	$(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) -DREDEFINED_CHARS -vn  -clib=ansi \
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
	-DSOUNDS -DREDEFINED_CHARS \
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

g800:
	$(Z88DK_PATH)$(MYZ88DK) +g800 $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__SRR__ -vn \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL -DNO_WAIT \
	-DCONIO_VT52 \
	-lndos \
	-clib=g850b \
	-create-app \
	$(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
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

pv1000:
	$(Z88DK_PATH)$(MYZ88DK) +pv1000 $(SCCZ80_OPTS) -pragma-redirect:fputc_cons=fputc_cons_generic \
	-D__SRR__ -vn \
	-DFULL_GAME -DSOUNDS \
	-DEND_SCREEN -DBETWEEN_LEVEL -DNO_WAIT \
	-DCONIO_VT52 \
	-lndos \
	-create-app \
	$(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	mv a.rom $(BUILD_PATH)/FULL_pv1000.rom


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
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_nc100_sprites.bin
	
einstein_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -leinstein \
	-D__EINSTEIN__ \
	-DFORCE_CONIO \
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


nc200_sprites_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc200 \
	-D__NC200__ \
	-DNO_PRINT \
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
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_nc200_sprites.bin


kc_putc4x6_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap \
	-D__KC__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-DZ88DK_PUTC4X6 \
	-create-app -o$(BUILD_PATH)/TINY_kc_putc4x6_.bin \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/z88dk/z88dk_putc4x6/z88dk_putc4x6_alt_print.c 
	rm $(BUILD_PATH)/TINY_kc_putc4x6_.bin
	

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

kc_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +kc -subtype=tap \
	-D__KC__ \
	-DTINY_GAME -DLESS_TEXT -DNO_SLEEP \
	-create-app -o$(BUILD_PATH)/TINY_kc.bin \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/TINY_kc.bin

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
	# $(Z88DK_PATH)$(MYZ88DK) +cpc $(SCCZ80_OPTS) -DREDEFINED_CHARS -vn  -clib=ansi \
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
	-DREDEFINED_CHARS -DSOUNDS -DFULL_GAME -clib=ansi -D__CPC__ -DCPCRSLIB -DBETWEEN_LEVEL -DEND_SCREEN \
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



enterprise_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +enterprise \
	-create-app -o $(BUILD_PATH)/TINY_enterprise.app \
	-lm -vn -lndos \
	-DTINY_GAME \
	-DNO_PRINT \
	-DNO_INPUT \
	-DNO_SLEEP -DLESS_TEXT -DALT_PRINT \
	$(SOURCE_PATH)/enterprise/enterprise_graphics.c \
	$(SOURCE_PATH)/display_macros.c \
	$(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c


ncurses_turn_based_tiny:
	$(_CC) -D__NCURSES__ \
	-DTINY_GAME \
	-DTURN_BASED \
	$(TINY_FILES) \
	-lncurses
	mv a.exe $(BUILD_PATH)/TINY_ncurses.exe
	
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
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	mv $(BUILD_PATH)/TINY_sms.bin $(BUILD_PATH)/TINY_sms.sms

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


z88_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +z88 \
	-D__Z88__ \
	-DTINY_GAME \
	-subtype=app -create-app \
	-o $(BUILD_PATH)/TINY_z88.bin \
	-vn -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c


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
	
ts2068:
	$(Z88DK_PATH)$(MYZ88DK) +ts2068 $(SCCZ80_OPTS) \
	-D__TS2068__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	-DFULL_GAME  -lndos \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c $(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c \
	$(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c \
	$(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c \
	-o $(BUILD_PATH)/FULL_ts2068.bin



ts2068_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +ts2068 \
	-D__TS2068__ \
	-DTINY_GAME \
	-clib=ansi -pragma-define:ansicolumns=32 -vn \
	-o $(BUILD_PATH)/FULL_ts2068.bin -lndos \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	
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
	

gal_6k:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	--opt-code-size \
	-pragma-need=ansiterminal \
	--reserve-regs-iy \
	-pragma-include:$(SOURCE_PATH)/../cfg/z88dk/zpragma.inc \
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
	-DFORCE_BOMBS_NUMBER=2 \
	-DFORCE_GHOSTS_NUMBER=4 \
	-DNO_DEAD_GHOSTS \
	-vn -lndos -create-app -Cz--audio -o  $(BUILD_PATH)/TINY_galaksija_6k.prg \
	$(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
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

	
# cmoc_link:
	# cmoc display_macros.o cmoc_graphics.o \
	# enemy.o \
	# level.o character.o text.o \
	# strategy.o \
	# input_macros.o \
	# main.o

# lwlink_link:
	# lwlink display_macros.o cmoc_graphics.o \
	# enemy.o \
	# level.o character.o text.o \
	# strategy.o \
	# input_macros.o \
	# main.o	

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

joy-test:
	$(CC65_PATH)$(MYCC65) -O -t gamate experiments/joy-test.c -o  $(BUILD_PATH)/joy-test.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/joy-test.bin

		
gamate_tiny:
	$(CC65_PATH)$(MYCC65) -O -t gamate -DTINY_GAME $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/gamate/gamate_graphics.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/TINY_gamate.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/TINY_gamate.bin
	
gamate_light:
	$(CC65_PATH)$(MYCC65) -O -t gamate $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/gamate/gamate_graphics.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_gamate.bin
	$(TOOLS_PATH)/gamate-fixcart $(BUILD_PATH)/LIGHT_gamate.bin

	# TODO: Reduce size in order to compile	


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
	-create-app -o $(BUILD_PATH)/FULL_osca.exe \
	$(SOURCE_PATH)/end_screen.c \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	


#All of these may work
# ti86s:
	# $(Z88DK_PATH)$(MYZ88DK) +ti86s $(SCCZ80_OPTS) -D__TI86S__ -clib=ansi -pragma-define:ansicolumns=32 -vn -DFULL_GAME -DSOUNDS   -lndos -create-app -o $(BUILD_PATH)/FULL_ti86_mz.prg  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c


ti86:
	$(Z88DK_PATH)$(MYZ88DK) +ti86 \
	$(SCCZ80_OPTS) -D__TI86__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_ti86.bin  \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	

ti86_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +ti86 \
	$(SCCZ80_OPTS) -D__TI86__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DTINY_GAME -DLESS_TEXT  \
	-lndos \
	-create-app -o $(BUILD_PATH)/TINY_ti86.bin  \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c


	
ti8x:
	$(Z88DK_PATH)$(MYZ88DK) +ti8x \
	$(SCCZ80_OPTS) -D__TI8X__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-DLESS_TEXT -DSIMPLE_STRATEGY -DNO_HINTS -DNO_BLINKING \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_ti8x.bin  \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_ti8x.bin
	
	
ti8x_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +ti8x \
	$(SCCZ80_OPTS) -D__TI8X__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-vn \
	-DFULL_GAME  \
	-DLESS_TEXT -DSIMPLE_STRATEGY -DNO_HINTS -DNO_BLINKING \
	-DTURN_BASED \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_ti8x_turn_based.bin  \
	$(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c $(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c \
	$(SOURCE_PATH)/main.c
	rm $(BUILD_PATH)/FULL_ti8x_turn_based.bin

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
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) -DSOUNDS -DREDEFINED_CHARS -vn -DMSX_MODE1 -DFULL_GAME -D__MSX__ -lndos -subtype=msxdos -o $(BUILD_PATH)/FULL_msx_color_32k.com $(SOURCE_PATH)/msx/msx_graphics.c $(SOURCE_PATH)/psg/psg_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c $(SOURCE_PATH)/bullet.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c	
	
