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

WINCMOC_DIR ?= /cygdrive/c/cmoc-win32/bin/
LWAR ?= $(WINCMOC_DIR)lwar.exe
WINCMOC ?= $(WINCMOC_DIR)cmoc.exe 
WINCMOC2 ?= $(WINCMOC_DIR)cmoc2.exe 
WINCMOC2_OPTS ?= -c -o combo.o -t coco --org 10240 

WINCMOC_OPTS ?= \
	--emit-uncalled \
	-c -O1 -D__WINCMOC__ -D__CMOC__=0.1.32 -D__6809__= -D__COCO__= -Dnullptr=0 "-Dbool=unsigned char" \
	-Dfalse=0 -Dtrue=1 "-Dbyte=unsigned char" -Dsbyte=char -Dword=unsigned -Dsword=int -Dconst= \
	-D__fastcall__= -D__cdecl= -Drestrict= -DNO_OLDNAMES= 
	

CC65_PATH ?= /home/fabri/CC65_FIX/cc65/bin/
#/home/fabri/CC65_FIX/cc65/bin/
#/cygdrive/c/cc65-snapshot-win32/bin/
Z88DK_PATH ?= /cygdrive/c/z88dk/bin/
Z88DK_PATH_20171210 ?= /cygdrive/c/z88dk_20171210/bin/
Z88DK_INCLUDE ?= /cygdrive/c/z88dk/include
BUILD_PATH ?= ../build

DEBUG_MACROS ?= \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_RANDOM_LEVEL \
	-DNO_WAIT \
	-DNO_SET_SCREEN_COLORS \
	-DNO_INPUT \
	-DNO_PRINT \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DNO_CHASE \
	-DTINY_GAME \
	-DTURN_BASED \
	-DNO_GRAPHICS \
	-DNO_INIT_GRAPHICS \
	-DDEFAULT_CLEAR_SCREEN 

MACROS_NO_GRAPHICS ?= \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DNO_CHASE \
	-DFULL_GAME \
	-DTURN_BASED \
	-DNO_GRAPHICS \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN

INCLUDE_OPTS ?= -I$(CROSS_LIB_PATH) -I$(CROSS_LIB_PATH)/sleep \
-I$(CROSS_LIB_PATH)/display \
-I$(CROSS_LIB_PATH)/display/graphics_data \
-I$(CROSS_LIB_PATH)/display/graphics_data/cc65 \
-I$(CROSS_LIB_PATH)/display/graphics_data/z88dk \
-I$(CROSS_LIB_PATH)/display/graphics_mode \
-I$(CROSS_LIB_PATH)/display/alt_print \
-I$(CROSS_LIB_PATH)/display/image_settings \
-I$(CROSS_LIB_PATH)/display/redefine_characters \
-I$(CROSS_LIB_PATH)/include \
-I$(CROSS_LIB_PATH)/sound \
-I$(CROSS_LIB_PATH)/sound/cc65/atmos \
-I$(CROSS_LIB_PATH)/sound/cc65/c264 \
-I$(CROSS_LIB_PATH)/sound/cc65/pokey \
-I$(CROSS_LIB_PATH)/sound/cc65/sid \
-I$(CROSS_LIB_PATH)/sound/cc65/vic20 \
-I$(CROSS_LIB_PATH)/sound/z88dk/bit_bang \
-I$(CROSS_LIB_PATH)/sound/z88dk/psg \
-I$(CROSS_LIB_PATH)/text \
-I$(CROSS_LIB_PATH)/input 


XTC68 ?= qcc
XTC68_OPTS = -D__XTC68__ $(INCLUDE_OPTS) -I$(SOURCE_PATH) \


MYCC65 ?= cl65$(EXEEXT) $(INCLUDE_OPTS) 
MYZ88DK ?= zcc$(EXEEXT)
MYZ88DKASM ?= z80asm$(EXEEXT)

SCCZ80_OPTS ?= $(INCLUDE_OPTS) 
#-O3

ZSDCC_OPTS ?= $(INCLUDE_OPTS)
#-SO3 --max-allocs-per-node200000

Z8K-PCOS-GCC ?= /opt/z8kgcc-jan-19-2009/bin/z8k-pcos-gcc
Z8K-PCOS-GCC_OPTS ?= -Wno-cpg $(INCLUDE_OPTS)

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

INCLUDE_ALL ?= $(INCLUDE_OPTS) -I$(SOURCE_PATH)

# For cygwin posix build: use gcc
# For windows 32 non-posix build: x86_64-w64-mingw32-gcc
_CC ?= gcc
_CC_OPTS ?= $(INCLUDE_OPTS)

TI99_CC ?= tms9900-gcc
TI99_OPTS ?= $(INCLUDE_OPTS) -nostdlib -DNO_RANDOM_LEVEL

GCC ?= gcc
GCC_OPTS ?= -Wpedantic -Wall $(INCLUDE_OPTS)

ATARI_ST_CC ?= m68k-atari-mint-gcc
ATARI_ST_OPTS ?= $(INCLUDE_OPTS)

AMIGA_CC ?= m68k-amigaos-gcc
AMIGA_OPTS ?= $(INCLUDE_OPTS)

EM_CC ?= emcc
EM_OPTS = $(INCLUDE_OPTS)

LCC1802 ?= /cygdrive/c/lcc42/bin/lcc.exe
LC1802_OPTS ?= $(INCLUDE_ALL)

COCO_OPTS_TINY  ?= -O0 -Wno-const  -D__COCO__ -DTINY_GAME $(INCLUDE_OPTS)

COCO_OPTS_LIGHT ?= -O0 -Wno-const -D__COCO__

COCO_OPTS       ?= -O0 -Wno-const  $(INCLUDE_OPTS) -DDEFAULT_CLEAR_SCREEN -D__CMOC__ -D__COCO__ -DMEMORY_MAPPED -DNO_INIT_GRAPHICS -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL

DRAGON_OPTS     ?= --dragon $(COCO_OPTS)

VECTREX_OPTS    ?= -O0 --vectrex $(INCLUDE_OPTS) -DNO_SLEEP -DNO_WAIT -DNO_INPUT -DNO_PRINT -Wno-const -DDEFAULT_CLEAR_SCREEN -D__CMOC__ -D__VECTREX__ -DVECTORIAL -DNO_INIT_GRAPHICS -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL

MO5_OPTS        ?= -O2 $(INCLUDE_OPTS) -DANIMATE_PLAYER -DFORCE_NO_CONIO_LIB -DNO_SLEEP -DNO_WAIT -Wno-const -DDEFAULT_CLEAR_SCREEN -D__CMOC__ -D__MO5__ -DBIT_MAPPED -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL

TO7_OPTS        ?= -O1 $(INCLUDE_OPTS) -DFORCE_NO_CONIO_LIB -DNO_INPUT -DNO_SLEEP -DNO_WAIT -DNO_PRINT -Wno-const -DDEFAULT_CLEAR_SCREEN -D__CMOC__ -D__TO7__ -DBIT_MAPPED -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL

_ACK ?= ack
_ACK_OPTS ?= -O6 $(INCLUDE_OPTS)
# -O 
# -ansi


# -DNO_SLEEP

# ------------------------------------------------------------------------------------------
# CC65
# 

cx16: 
	$(CC65_PATH)$(MYCC65) -O -t cx16 \
	-D__CX16__ \
	-DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_cx16.prg


atari_color: 
	$(CC65_PATH)$(MYCC65) -O -t atari \
	-DREDEFINED_CHARS \
	-DANIMATE_PLAYER -DFULL_GAME -DATARI_MODE1 -DSOUNDS -DBETWEEN_LEVEL -DEND_SCREEN \
	--config $(CFG_PATH)/cc65/atari_mode1_redefined_chars.cfg \
	-DMEMORY_MAPPED \
	-DDEFAULT_CLEAR_SCREEN \
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

atari2600_graphics_test:
	$(CC65_PATH)$(MYCC65) -O -t atari2600 \
	-D__ATARI2600__ \
	$(DEBUG_MACROS) \
	-DTINY_GAME \
	$(SOURCE_PATH)/../test/atari2600_graphics_test.c \
	-o $(BUILD_PATH)/atari2600_graphics_test.rom	

atari2600_tiny:
	$(CC65_PATH)$(MYCC65) -O -t atari2600 \
	-D__ATARI2600__ \
	$(DEBUG_MACROS) \
	-DTINY_GAME \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_atari2600.rom

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


vic20_unexpanded_no_gfx:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 \
	-DSOUNDS \
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
	-DUSE_TINY_MEMORY_BUFFERS \
	--config $(CFG_PATH)/cc65/vic20_unexpanded.cfg  \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/memory/vic20_memory.s \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds_3k.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/simple_alt_stats.c \
	-o $(BUILD_PATH)/TINY_vic20_unexpanded_no_gfx.prg

vic20_unexpanded:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 \
	-DSOUNDS \
	-DCC65_GOTO \
	-DANIMATE_PLAYER \
	-DREDEFINED_CHARS \
	-DNO_SPACE_TEXT \
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
	-DALT_HIGHSCORE \
	-DALT_DISPLAY_STATS \
	-DDEFAULT_CLEAR_SCREEN \
	-DUSE_TINY_MEMORY_BUFFERS \
	-DROUND_ENEMIES \
	-DSIMPLE_RANDOM_LEVEL \
	--config $(CFG_PATH)/cc65/vic20_unexpanded_gfx.cfg  \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds_3k.c \
	$(CROSS_LIB_PATH)/memory/vic20_memory.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/simple_alt_stats.c \
	$(CROSS_LIB_PATH)/display/graphics_data/cc65/udc_6chars.s \
	-o $(BUILD_PATH)/TINY_vic20_unexpanded.prg


vic20_unexpanded_turn_based:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 \
	-DSOUNDS \
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
	-DUSE_TINY_MEMORY_BUFFERS \
	--config $(CFG_PATH)/cc65/vic20_unexpanded.cfg  \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/memory/vic20_memory.s \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds_3k.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/simple_alt_stats.c \
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
	-DUSE_TINY_MEMORY_BUFFERS \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds_3k.c \
	$(CROSS_LIB_PATH)/display/graphics_data/cc65/udc_3k.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/memory/vic20_memory.s \
	-o $(BUILD_PATH)/TINY_vic20_exp_3k.prg

	
vic20_exp_3k_light:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 \
	--config $(CFG_PATH)/cc65/vic20-3k.cfg  \
	-DANIMATE_PLAYER -DLESS_TEXT -DNO_SET_SCREEN_COLORS -DNO_DEAD_GHOSTS -DFORCE_GHOSTS_NUMBER=8 \
	-DROUND_ENEMIES \
	-DSOUNDS \
	-DVIC20_EXP_3K \
	-DMEMORY_MAPPED \
	-DNO_PRINT \
	-DCBM_SCREEN_CODES \
	-DUSE_MEMORY_BUFFERS \
	-DLESS_TEXT \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_BLINKING \
	-DNO_RANDOM_LEVEL \
	-DALT_SLEEP \
	-DNO_INIT_GRAPHICS \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c  $(CROSS_LIB_PATH)/text/text_macros.c \
	$(SOURCE_PATH)/skull.c $(SOURCE_PATH)/ghost.c $(CROSS_LIB_PATH)/input/input_macros.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/bullet.c \
	$(SOURCE_PATH)/move_player.c \
	$(CROSS_LIB_PATH)/display/init_images.c \
	$(SOURCE_PATH)/item.c	\
	$(CROSS_LIB_PATH)/display/display_macros.c  \
	$(SOURCE_PATH)/main.c \
	$(CROSS_LIB_PATH)/memory/vic20_memory.s \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	-o $(BUILD_PATH)/LIGHT_vic20_exp_3k.prg
	
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
	-DEND_SCREEN \
	-DUSE_MEMORY_BUFFERS \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/memory/vic20_memory.s \
	$(SOURCE_PATH)/end_screen.c \
	-o $(BUILD_PATH)/FULL_vic20_exp_8k_NO_GFX.prg		

vic20_exp_8k: 
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20 \
	--config $(CFG_PATH)/cc65/vic20-8k_GFX_3.cfg \
	-DSOUNDS -DANIMATE_PLAYER \
	-DCC65_GOTO \
	-DMEMORY_MAPPED -D__VIC20__ -DVIC20_EXP_8K \
	-DFULL_GAME \
	-DSIMPLE_STRATEGY \
	-DREDEFINED_CHARS \
	-DALT_PRINT \
	-DNO_HINTS \
	-DLESS_TEXT \
	-DDEFAULT_CLEAR_SCREEN \
	-DALT_SLEEP \
	-DUSE_MEMORY_BUFFERS \
	-DDANCE -DEND_SCREEN \
	$(CROSS_LIB_PATH)/display/graphics_data/cc65/udc_vic20.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/vic20/vic20_init_graphics.c \
	$(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c \
	$(SOURCE_PATH)/game_text.c  $(CROSS_LIB_PATH)/text/text_macros.c \
	$(SOURCE_PATH)/ghost.c $(CROSS_LIB_PATH)/input/input_macros.c  \
	$(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/bullet.c \
	$(SOURCE_PATH)/move_player.c \
	$(SOURCE_PATH)/item.c \
	$(SOURCE_PATH)/skull.c \
	$(SOURCE_PATH)/rocket.c \
	$(CROSS_LIB_PATH)/display/display_macros.c  \
	$(SOURCE_PATH)/horizontal_missile.c \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	--code-name CODE2 \
	$(CROSS_LIB_PATH)/display/init_images.c \
	$(CROSS_LIB_PATH)/sound/cc65/vic20/vic20_sounds.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(SOURCE_PATH)/main.c \
	$(CROSS_LIB_PATH)/memory/vic20_memory.s \
	$(SOURCE_PATH)/end_screen.c \
	-o $(BUILD_PATH)/FULL_vic20_exp_8k.prg


vic20_exp_8k_old: 
	$(CC65_PATH)$(MYCC65) -Or -Cl -t vic20  -DSOUNDS -DANIMATE_PLAYER --config $(CFG_PATH)/cc65/vic20-8k_GFX.cfg \
	-DCONIO -DVIC20_EXP_8K \
	-DREDEFINED_CHARS \
	-DALT_SLEEP \
	-DSIMPLE_STRATEGY \
	-DNO_HINTS \
	-DUSE_MEMORY_BUFFERS \
	-DLESS_TEXT \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_BLINKING \
	-DNO_RANDOM_LEVEL \
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
	$(CROSS_LIB_PATH)/memory/vic20_memory.s \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
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
	-o $(BUILD_PATH)/FULL_vic20_exp_16k.prg

c64_bench: 
	$(CC65_PATH)$(MYCC65) -O -t c64 \
	-DBENCHMARK -DBENCHMARK_MAX=40 \
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
	-DFORCE_GHOSTS_NUMBER=9 \
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
	-DFORCE_XSIZE=80 \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-DFORCE_GHOSTS_NUMBER=9 \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c128/c128_80col_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/sid/sid_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_c128_80col.prg


c16_16k: 
	$(CC65_PATH)$(MYCC65) -Or -t c16 \
	--config $(CFG_PATH)/cc65/c16-16k_GFX.cfg \
	-DANIMATE_PLAYER  -DSOUNDS \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DREDEFINED_CHARS \
	-DCONIO \
	-DUSE_MEMORY_BUFFERS \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c264/c16_32k_redefined_chars_init_graphics.c  \
	$(LIGHT_FILES) \
	$(CROSS_LIB_PATH)/memory/c16_memory.s \
	-o $(BUILD_PATH)/LIGHT_c16_16k.prg
	
c16_32k: 
	$(CC65_PATH)$(MYCC65) -Or -t c16 --config $(CFG_PATH)/cc65/c16-32k.cfg \
	-DANIMATE_PLAYER -DFULL_GAME -DSOUNDS \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DREDEFINED_CHARS \
	-DCONIO \
	-DUSE_MEMORY_BUFFERS \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c264/c16_32k_redefined_chars_init_graphics.c  \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/memory/c16_memory.s \
	-o $(BUILD_PATH)/FULL_c16_32k.prg


c16_16k_gfx:
	$(CC65_PATH)$(MYCC65) -Or -Cl -t c16 \
	--config $(CFG_PATH)/cc65/c16-16k_GFX_2.cfg \
	-DFORCE_NO_CONIO_LIB \
	-DFULL_GAME \
	-DREDEFINED_CHARS \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DLESS_TEXT	\
	-DSOUNDS \
	-DNO_HINTS \
	-DALT_PRINT \
	-DANIMATE_PLAYER \
	-DMEMORY_MAPPED \
	-DC16_UNEXPANDED \
	-DDEFAULT_CLEAR_SCREEN \
	-DALT_SLEEP \
	-DDANCE -DEND_SCREEN \
	-DUSE_MEMORY_BUFFERS \
	$(CROSS_LIB_PATH)/memory/c16_memory.s \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c264/c16_16k_redefined_chars_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(FULL_FILES) \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/graphics_data/cc65/udc_63chars.s \
	-o $(BUILD_PATH)/FULL_c16_16k.prg	
	
c16_16k_full: 
	$(CC65_PATH)$(MYCC65) -Or -Cl -t c16 \
	--config $(CFG_PATH)/cc65/c16-16k.cfg \
	-DFULL_GAME \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DLESS_TEXT	\
	-DSOUNDS \
	-DALT_PRINT \
	-DANIMATE_PLAYER \
	-DMEMORY_MAPPED \
	-DEND_SCREEN \
	-DUSE_MEMORY_BUFFERS \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/c264/c264_init_graphics.c \
	$(CROSS_LIB_PATH)/sound/cc65/c264/c264_sounds.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/memory/c16_memory.s \
	-o $(BUILD_PATH)/FULL_c16_16k_NO_GFX.prg	

	
pet_8k_buffered: 
	$(CC65_PATH)$(MYCC65) -O -t pet -Cl \
	-DTINY_GAME -DNO_INIT_GRAPHICS \
	-DBUFFERED \
	-DALT_PRINT \
	-DUSE_TINY_MEMORY_BUFFERS \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/memory/pet_memory.s \
	-o $(BUILD_PATH)/TINY_pet_8k_buffered.prg	
	
pet_8k: 
	$(CC65_PATH)$(MYCC65) -O -t pet -Cl \
	-DTINY_GAME -DNO_INIT_GRAPHICS -DCONIO \
	-DUSE_TINY_MEMORY_BUFFERS \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/memory/pet_memory.s \
	-o $(BUILD_PATH)/TINY_pet_8k.prg
	
pet_8k_light_experimental:
	$(CC65_PATH)$(MYCC65) -Or -t pet -Cl \
	-DNO_INIT_GRAPHICS -DCONIO -DLESS_TEXT -DNO_HINTS -DNO_BLINKING -DNO_RANDOM_LEVEL \
	-DUSE_LIGHT_MEMORY_BUFFERS \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DFORCE_NARROW \
	-DNO_SET_SCREEN_COLORS \
	-DNO_STATS \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_DEAD_GHOSTS \
	$(LIGHT_FILES) \
	$(CROSS_LIB_PATH)/memory/pet_memory.s \
	-o $(BUILD_PATH)/LIGHT_pet_8k_experimental.prg
	
pet_8k_light:
	$(CC65_PATH)$(MYCC65) -Or -t pet -Cl \
	-DLESS_TEXT \
	-DNO_MESSAGE -DNO_BLINKING \
	-DNO_CONTROL_INSTRUCTIONS -DNO_INITIAL_SCREEN \
	-DNO_DEAD_GHOSTS -DNO_SET_SCREEN_COLORS -DNO_RANDOM_LEVEL \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DFORCE_NARROW \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DUSE_LIGHT_MEMORY_BUFFERS \
	-DNO_STATS \
	$(LIGHT_FILES) \
	$(CROSS_LIB_PATH)/memory/pet_memory.s \
	-o $(BUILD_PATH)/LIGHT_pet_8k_less_text.prg

pet_16k: 
	$(CC65_PATH)$(MYCC65) -O -t pet \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_pet_16k.prg

telestrat: 
	$(CC65_PATH)$(MYCC65) -O -t telestrat \
	-DFORCE_CONIO_LIB \
	-DNO_WAIT \
	-DNO_SET_SCREEN_COLORS \
	-DALT_SLEEP \
	-DFULL_GAME \
	-DNO_COLOR \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_telestrat.prg	
	
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
	-DDEFAULT_CLEAR_SCREEN \
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
	-DFORCE_GHOSTS_NUMBER=8 \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	-o $(BUILD_PATH)/FULL_osic1p_32k.lod
	$(TOOLS_PATH)/cc65/osic1p/srec_cat $(BUILD_PATH)/FULL_osic1p_32k.lod -binary -offset 0x200 -o $(BUILD_PATH)/FULL_osic1p_32k.c1p -Ohio_Scientific -execution-start-address=0x200	
	rm $(BUILD_PATH)/FULL_osic1p_32k.lod
	mv $(BUILD_PATH)/FULL_osic1p_32k.c1p $(BUILD_PATH)/FULL_osic1p_32k.lod

	
osic1p_32k_turn_based: 
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
	-o $(BUILD_PATH)/FULL_osic1p_32k_turn_based.lod
	$(TOOLS_PATH)/cc65/osic1p/srec_cat $(BUILD_PATH)/FULL_osic1p_32k_turn_based.lod -binary -offset 0x200 -o $(BUILD_PATH)/FULL_osic1p_32k_turn_based.c1p -Ohio_Scientific -execution-start-address=0x200	
	rm $(BUILD_PATH)/FULL_osic1p_32k_turn_based.lod
	mv $(BUILD_PATH)/FULL_osic1p_32k_turn_based.c1p $(BUILD_PATH)/FULL_osic1p_32k_turn_based.lod


osic1p_8k: 
	$(CC65_PATH)$(MYCC65) -Cl --start-addr 0x200 -Wl -D,__HIMEM__=0x2000 -O --config $(CFG_PATH)/cc65/osic1p_less_stack.cfg -t osic1p \
	-DROUND_ENEMIES -DNO_SLEEP  -DNO_RANDOM_LEVEL -DLESS_TEXT -DNO_SET_SCREEN_COLORS -DTINY_GAME \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_osic1p_8k.lod	
	$(TOOLS_PATH)/cc65/osic1p/srec_cat $(BUILD_PATH)/TINY_osic1p_8k.lod -binary -offset 0x200 -o $(BUILD_PATH)/TINY_osic1p_8k.c1p -Ohio_Scientific -execution-start-address=0x200	
	rm $(BUILD_PATH)/TINY_osic1p_8k.lod
	mv $(BUILD_PATH)/TINY_osic1p_8k.c1p $(BUILD_PATH)/TINY_osic1p_8k.lod


osic1p_8k_turn_based: 
	$(CC65_PATH)$(MYCC65) -Cl --start-addr 0x200 -Wl -D,__HIMEM__=0x2000 -O --config $(CFG_PATH)/cc65/osic1p_less_stack.cfg -t osic1p \
	-DROUND_ENEMIES -DNO_SLEEP  -DNO_RANDOM_LEVEL -DLESS_TEXT -DNO_SET_SCREEN_COLORS -DTINY_GAME \
	-DTURN_BASED \
	-DFORCE_GHOSTS_NUMBER=9 \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_osic1p_8k_turn_based.lod	
	$(TOOLS_PATH)/cc65/osic1p/srec_cat $(BUILD_PATH)/TINY_osic1p_8k_turn_based.lod -binary -offset 0x200 -o $(BUILD_PATH)/TINY_osic1p_8k_turn_based.c1p -Ohio_Scientific -execution-start-address=0x200	
	rm $(BUILD_PATH)/TINY_osic1p_8k_turn_based.lod
	mv $(BUILD_PATH)/TINY_osic1p_8k_turn_based.c1p $(BUILD_PATH)/TINY_osic1p_8k_turn_based.lod
	
	
gamate: 
	$(CC65_PATH)$(MYCC65) -O -t gamate   --config $(CFG_PATH)/cc65/gamate_reduced_stack.cfg \
	-DFULL_GAME \
	-DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DSIMPLE_STRATEGY \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_gamate.bin
	$(TOOLS_PATH)/cc65/gamate/gamate-fixcart $(BUILD_PATH)/FULL_gamate.bin

gamate_test:
	$(CC65_PATH)$(MYCC65) -O -t gamate \
	$(SOURCE_PATH)/../test/gamate_test.c \
	-o $(BUILD_PATH)/gamate_test.bin 
	$(TOOLS_PATH)/cc65/gamate/gamate-fixcart $(BUILD_PATH)/gamate_test.bin

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
	-DNO_SET_SCREEN_COLORS \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(TINY_FILES) \
	-o $(BUILD_PATH)/TINY_creativision_8k.bin

creativision_8k_light: 
	$(CC65_PATH)$(MYCC65) -Or -Cl -t creativision \
	-DNO_SET_SCREEN_COLORS \
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
	

creativision_16k_x:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	-DNO_SET_SCREEN_COLORS \
	-DFULL_GAME -DALT_SLEEP \
	-DSIMPLE_STRATEGY \
	-DCONIO \
	-DANIMATE_PLAYER \
	--config $(CFG_PATH)/cc65/creativision-16k.cfg \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/creativision/creativision_init_graphics.c \
	-o $(BUILD_PATH)/FULL_creativision_16k.bin	
	dd if=$(BUILD_PATH)/FULL_creativision_16k.bin ibs=1 count=8192 of=$(BUILD_PATH)/FULL_creativision_16k_LOW.bin
	dd if=$(BUILD_PATH)/FULL_creativision_16k.bin ibs=1 skip=8192 of=$(BUILD_PATH)/FULL_creativision_16k_HIGH.bin
	rm $(BUILD_PATH)/FULL_creativision_16k.bin 
	cat $(BUILD_PATH)/FULL_creativision_16k_HIGH.bin $(BUILD_PATH)/FULL_creativision_16k_LOW.bin > $(BUILD_PATH)/FULL_creativision_16k_SWAPPED.bin
	rm $(BUILD_PATH)/FULL_creativision_16k_LOW.bin
	rm $(BUILD_PATH)/FULL_creativision_16k_HIGH.bin	    
    



creativision_16k_m:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	-DNO_SET_SCREEN_COLORS \
	-DFULL_GAME -DALT_SLEEP \
	-DSIMPLE_STRATEGY \
	-DMEMORY_MAPPED \
	-DANIMATE_PLAYER \
	--config $(CFG_PATH)/cc65/creativision-16k.cfg \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/creativision/creativision_init_graphics.c \
	-o $(BUILD_PATH)/FULL_creativision_16k.bin	
	dd if=$(BUILD_PATH)/FULL_creativision_16k.bin ibs=1 count=8192 of=$(BUILD_PATH)/FULL_creativision_16k_LOW.bin
	dd if=$(BUILD_PATH)/FULL_creativision_16k.bin ibs=1 skip=8192 of=$(BUILD_PATH)/FULL_creativision_16k_HIGH.bin
	rm $(BUILD_PATH)/FULL_creativision_16k.bin 
	cat $(BUILD_PATH)/FULL_creativision_16k_HIGH.bin $(BUILD_PATH)/FULL_creativision_16k_LOW.bin > $(BUILD_PATH)/FULL_creativision_16k_SWAPPED.bin
	rm $(BUILD_PATH)/FULL_creativision_16k_LOW.bin
	rm $(BUILD_PATH)/FULL_creativision_16k_HIGH.bin	      
    
    
creativision_16k:
	$(CC65_PATH)$(MYCC65) -O -t creativision \
	-DNO_SET_SCREEN_COLORS \
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
	-DNO_SET_SCREEN_COLORS \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DTGI \
	-DFULL_GAME \
	-DALT_PRINT \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atari_lynx/atari_lynx_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/tgi_alt_print.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_ATARI_LYNX.lnx


bbc_test:
	$(CC65_PATH)$(MYCC65) -t bbc -c \
	-o $(BUILD_PATH)/bbc_test.o \
	-D__BBC__ \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DNO_PRINT \
	-DTINY_GAME \
	-DNO_WAIT \
	-DTURN_BASED \
	-DNO_GRAPHICS \
	-DNO_INPUT \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(SOURCE_PATH)/main.c

TINY_OBJ ?= character.o game_text.o level.o main.o move_player.o strategy.o

bbc_no_graphics:
	$(CC65_PATH)$(MYCC65) -t bbc -c \
	-D__BBC__ \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DNO_PRINT \
	-DTINY_GAME \
	-DNO_WAIT \
	-DTURN_BASED \
	-DNO_GRAPHICS \
	-DNO_INPUT \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(TINY_FILES)
	$(CC65_PATH)/ld65$(EXE) -t bbc \
	--obj-path $(SOURCE_PATH) \
	-m $(BUILD_PATH)/bbc_no_graphics.map \
	-o $(BUILD_PATH)/bbc_no_graphics.bin \
	$(TINY_OBJ)


c64_no_graphics:
	$(CC65_PATH)$(MYCC65) -t supervision -c \
	-D__BBC__ \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DNO_PRINT \
	-DTINY_GAME \
	-DNO_WAIT \
	-DTURN_BASED \
	-DNO_GRAPHICS \
	-DNO_INPUT \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(TINY_FILES)
	$(CC65_PATH)/ld65$(EXE) -t none \
	--obj-path $(SOURCE_PATH) \
	-m $(BUILD_PATH)/supervision_no_graphics.map \
	-o $(BUILD_PATH)/supervision_no_graphics.bin \
	$(TINY_OBJ)


none_no_graphics:
	$(CC65_PATH)$(MYCC65) -t none -c \
	-D__BBC__ \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DNO_PRINT \
	-DTINY_GAME \
	-DNO_WAIT \
	-DTURN_BASED \
	-DNO_GRAPHICS \
	-DNO_INPUT \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(TINY_FILES)
	$(CC65_PATH)/ld65$(EXE) -t none \
	--obj-path $(SOURCE_PATH) \
	-m $(BUILD_PATH)/none_no_graphics.map \
	-o $(BUILD_PATH)/none_no_graphics.bin \
	$(TINY_OBJ)



atari_lynx_hello_test:
	$(CC65_PATH)$(MYCC65) -O -t lynx $(SOURCE_PATH)/../test/tgi_lynx_test.c \
	-o $(BUILD_PATH)/ATARI_LYNX_HELLO_TEST.lnx
    
atari_lynx_black_test:
	$(CC65_PATH)$(MYCC65) -O -t lynx $(SOURCE_PATH)/../test/tgi_lynx_black_test.c \
	-o $(BUILD_PATH)/ATARI_LYNX_BLACK_TEST.lnx

atari_lynx_cgetc_test:
	$(CC65_PATH)$(MYCC65) -O -t lynx $(SOURCE_PATH)/../test/tgi_lynx_cgetc_test.c \
	-o $(BUILD_PATH)/ATARI_LYNX_CGETC_TEST.lnx
 
atari_lynx_light:
	$(CC65_PATH)$(MYCC65) -O -t lynx \
	-DNO_SET_SCREEN_COLORS \
	-D__ATARI_LYNX__ \
	-DNO_PRINT \
	-DNO_SLEEP \
	-DTGI \
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atari_lynx/atari_lynx_init_graphics.c \
	$(LIGHT_FILES) \
	-o $(BUILD_PATH)/LIGHT_ATARI_LYNX.lnx
	
	
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


supervision_dummy:
	$(CC65_PATH)$(MYCC65) -O -t supervision  \
	-DNO_PRINT -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN -DNO_SLEEP \
	-DBIT_MAPPED_4_DUMMY -DNO_INIT_GRAPHICS \
	-DDEFAULT_CLEAR_SCREEN \
	-DNO_WAIT \
	-DNO_SLEEP \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-o $(BUILD_PATH)/FULL_supervision_dummy.sv
	
    
supervision:
	$(CC65_PATH)$(MYCC65) -O -t supervision  \
	--config $(CFG_PATH)/cc65/supervision-16k_GFX.cfg \
	$(CROSS_LIB_PATH)/display/graphics_data/cc65/udc.s \
	-DNO_PRINT -DFULL_GAME -DBETWEEN_LEVEL -DEND_SCREEN -DNO_SLEEP \
	-DBIT_MAPPED_4 \
	-DDEFAULT_CLEAR_SCREEN \
	-DANIMATE_PLAYER \
	-DNO_WAIT \
	-DNO_SLEEP \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/supervision/supervision_init_graphics.c \
	-o $(BUILD_PATH)/FULL_supervision.sv
    
supervision_test:
	$(CC65_PATH)$(MYCC65) -O -t supervision  \
	--config $(CFG_PATH)/cc65/supervision-16k.cfg \
	$(SOURCE_PATH)/../test/supervision_test.c \
	-o $(BUILD_PATH)/TEST_supervision.sv


supervision_cc65_test:
	$(CC65_PATH)$(MYCC65) -O -t supervision  \
	/home/fabri/CC65_FIX/cc65/samples/supervisionhello.c \
	-o $(BUILD_PATH)/TEST_hellosupervision.sv

# ------------------------------------------------------------------------------------------
# Z88DK
#


pc88:
	$(Z88DK_PATH)$(MYZ88DK) +pc88  -Cz--audio $(SCCZ80_OPTS) -vn \
	-DSOUNDS -D__PC88__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-DCONIO -DNO_INIT_GRAPHICS \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES) \
	-create-app	\
	-o FULL_pc88
	rm FULL_pc88
	tar czvf FULL_pc88.tar.zip FULL_pc88.t88 FULL_pc88.wav
	rm FULL_pc88.t88
	rm FULL_pc88.wav
	mv FULL_pc88.tar.zip $(BUILD_PATH)


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
	-DDEFAULT_CLEAR_SCREEN \
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

aquarius_hello_world:
	$(Z88DK_PATH)$(MYZ88DK) +aquarius -clib=ansi $(SCCZ80_OPTS) -vn \
	$(SOURCE_PATH)/../test/hello_conio.c \
	-create-app


aquarius_hello_stack:
	$(Z88DK_PATH)$(MYZ88DK) +aquarius -pragma-redirect:fputc_cons=fputc_cons_generic -pragma-output:STACKPTR=20000 $(SCCZ80_OPTS) -vn \
	$(SOURCE_PATH)/../test/hello_fputc.c \
	-create-app -o minimal_hello


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
	rm $(BUILD_PATH)/FULL_vg5k_less_text.prg	

vg5k_udg:
	$(Z88DK_PATH)$(MYZ88DK) +vg5k \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	--reserve-regs-iy \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-vn -DFULL_GAME -D__VG5K__ \
	-DLESS_TEXT \
	-DUDG_GRAPHICS \
	-DANIMATE_PLAYER \
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
	

zx80_16k_turn_based_qaopm:
	$(Z88DK_PATH)$(MYZ88DK) +zx80 $(SCCZ80_OPTS) -vn \
	-D__ZX80__ -DFULL_GAME \
	-DZ88DK_JOYSTICK \
	-DEND_SCREEN \
	-DTURN_BASED \
	-DALT_SLEEP \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx80_16k_turn_based_qaopm.prg \
	$(CROSS_LIB_PATH)/sleep/sleep_macros.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_zx80_16k_turn_based_qaopm.prg


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


zx81_16k_turn_based_qaopm:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-v \
	-D__ZX81__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DZ88DK_JOYSTICK \
	-DTURN_BASED \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_16k_turn_based_qaopm.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_zx81_16k_turn_based_qaopm.prg

    
getk_test:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	-compiler=sdcc \
	-create-app -o  $(BUILD_PATH)/getk_test.prg \
	$(SOURCE_PATH)/../test/getk_test.c
	rm $(BUILD_PATH)/getk_test.prg
	
zx81_16k:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	$(ZSDCC_OPTS) \
	-compiler=sdcc \
	--opt-code-size \
	-vn \
	-D__ZX81__ -DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_16k.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_zx81_16k.prg
	

zx81_16k_qaopm:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 \
	$(ZSDCC_OPTS) \
	-compiler=sdcc \
	--opt-code-size \
	-vn \
	-D__ZX81__ -DFULL_GAME \
	-DZ88DK_JOYSTICK \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_16k_qaopm.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_zx81_16k_qaopm.prg
	
zx81_16k_wrx_no_text:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -subtype=wrx -clib=wrx \
	$(SCCZ80_OPTS) -O3 \
    -DANIMATE_PLAYER \
	-DFORCE_XSIZE=36 \
	-DFORCE_YSIZE=30 \
	-vn \
	-D__ZX81__ \
	-DZ88DK_SPRITES \
	-DNO_PRINT \
	-lndos \
	-create-app -o  $(BUILD_PATH)/LIGHT_zx81_16k_wrx_no_text.prg \
	$(LIGHT_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/zx81/zx81_init_graphics.c
	rm $(BUILD_PATH)/LIGHT_zx81_16k_wrx_no_text.prg


zx81_16k_wrx:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -subtype=wrx -clib=wrx \
	$(SCCZ80_OPTS) -O3 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib_no_printf.inc \
	-DALT_MODE \
	-DANIMATE_PLAYER \
	-DFORCE_GHOSTS_NUMBER=7 \
	-DFORCE_XSIZE=36 \
	-DFORCE_YSIZE=35 \
	-vn \
	-D__ZX81__ \
	-DZ88DK_SPRITES \
	-DLESS_PRINT \
	-lndos \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DNO_COLOR \
	-DSIMPLE_STRATEGY \
	-DNO_SET_SCREEN_COLORS \
	-DALT_PRINT \
	-DFEWER_SPRITES \
	-DNO_SMALL_LETTERS \
	-create-app -o  $(BUILD_PATH)/LIGHT_zx81_16k_wrx.prg \
	$(LIGHT_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/zx81/zx81_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	rm $(BUILD_PATH)/LIGHT_zx81_16k_wrx.prg


zx81_16k_wrx_qaopm:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -subtype=wrx -clib=wrx \
	$(SCCZ80_OPTS) -O3 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib_no_printf.inc \
	-DZ88DK_JOYSTICK \
	-DANIMATE_PLAYER \
	-DFORCE_GHOSTS_NUMBER=7 \
	-DFORCE_XSIZE=36 \
	-DFORCE_YSIZE=35 \
	-vn \
	-D__ZX81__ \
	-DZ88DK_SPRITES \
	-DLESS_PRINT \
	-lndos \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DNO_COLOR \
	-DSIMPLE_STRATEGY \
	-DNO_SET_SCREEN_COLORS \
	-DALT_PRINT \
	-DFEWER_SPRITES \
	-DNO_SMALL_LETTERS \
	-create-app -o  $(BUILD_PATH)/LIGHT_zx81_16k_wrx_qaopm.prg \
	$(LIGHT_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/zx81/zx81_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	rm $(BUILD_PATH)/LIGHT_zx81_16k_wrx_qaopm.prg


zx81_32k_wrx:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -subtype=wrx -clib=wrx \
	$(SCCZ80_OPTS) -O3 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib_no_printf.inc \
	-pragma-define:hrgpage=36096 \
	-DFORCE_GHOSTS_NUMBER=7 \
	-DANIMATE_PLAYER \
	-DFORCE_XSIZE=36 \
	-DFORCE_YSIZE=35 \
	-vn \
	-D__ZX81__ \
	-DZ88DK_SPRITES \
	-DLESS_PRINT \
	-DFULL_GAME \
	-lndos \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DNO_COLOR \
	-DSIMPLE_STRATEGY \
	-DALT_MOVE \
	-DNO_SET_SCREEN_COLORS \
	-DALT_PRINT \
	-DFEWER_SPRITES \
	-DNO_SMALL_LETTERS \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_32k_wrx.prg \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/zx81/zx81_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	rm $(BUILD_PATH)/FULL_zx81_32k_wrx.prg

zx81_32k_wrx_qaopm:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -subtype=wrx -clib=wrx \
	$(SCCZ80_OPTS) -O3 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib_no_printf.inc \
	-pragma-define:hrgpage=36096 \
	-DFORCE_GHOSTS_NUMBER=7 \
	-DANIMATE_PLAYER \
	-DFORCE_XSIZE=36 \
	-DFORCE_YSIZE=35 \
	-DZ88DK_JOYSTICK \
	-vn \
	-D__ZX81__ \
	-DZ88DK_SPRITES \
	-DLESS_PRINT \
	-DFULL_GAME \
	-lndos \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DNO_COLOR \
	-DSIMPLE_STRATEGY \
	-DNO_SET_SCREEN_COLORS \
	-DALT_PRINT \
	-DFEWER_SPRITES \
	-DNO_SMALL_LETTERS \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_32k_wrx_qaopm.prg \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/zx81/zx81_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	rm $(BUILD_PATH)/FULL_zx81_32k_wrx_qaopm.prg


zx81_32k_wrx_turn_based:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -subtype=wrx -clib=wrx \
	$(SCCZ80_OPTS) -O3 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib_no_printf.inc \
	-pragma-define:hrgpage=36096 \
	-DFORCE_GHOSTS_NUMBER=9 \
	-DANIMATE_PLAYER \
	-DNO_BLINKING \
	-DTURN_BASED \
	-DFORCE_XSIZE=36 \
	-DFORCE_YSIZE=35 \
	-vn \
	-D__ZX81__ \
	-DZ88DK_SPRITES \
	-DLESS_PRINT \
	-DFULL_GAME \
	-lndos \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DNO_COLOR \
	-DSIMPLE_STRATEGY \
	-DNO_SET_SCREEN_COLORS \
	-DALT_PRINT \
	-DFEWER_SPRITES \
	-DNO_SMALL_LETTERS \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_32k_wrx_turn_based.prg \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/zx81/zx81_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	rm $(BUILD_PATH)/FULL_zx81_32k_wrx_turn_based.prg


zx81_32k_wrx_turn_based_qaopm:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -subtype=wrx -clib=wrx \
	$(SCCZ80_OPTS) -O3 \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib_no_printf.inc \
	-pragma-define:hrgpage=36096 \
	-DZ88DK_JOYSTICK \
	-DFORCE_GHOSTS_NUMBER=9 \
	-DANIMATE_PLAYER \
	-DNO_BLINKING \
	-DTURN_BASED \
	-DFORCE_XSIZE=36 \
	-DFORCE_YSIZE=35 \
	-vn \
	-D__ZX81__ \
	-DZ88DK_SPRITES \
	-DLESS_PRINT \
	-DFULL_GAME \
	-lndos \
	-DANIMATE_PLAYER \
	-DLESS_TEXT \
	-DNO_COLOR \
	-DSIMPLE_STRATEGY \
	-DNO_SET_SCREEN_COLORS \
	-DALT_PRINT \
	-DFEWER_SPRITES \
	-DNO_SMALL_LETTERS \
	-create-app -o  $(BUILD_PATH)/FULL_zx81_32k_wrx_turn_based_qaopm.prg \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/zx81/zx81_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	rm $(BUILD_PATH)/FULL_zx81_32k_wrx_turn_based_qaopm.prg

zx81_wrx_targets: \
    zx81_16k_wrx zx81_32k_wrx zx81_32k_wrx_turn_based

zx81_16k_wrx_zsdcc:
	$(Z88DK_PATH)$(MYZ88DK) +zx81 -subtype=wrx -clib=wrx \
	$(ZSDCC_OPTS) \
	-compiler=sdcc \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib_no_printf.inc \
    -DANIMATE_PLAYER \
	-DFORCE_XSIZE=36 \
	-DFORCE_YSIZE=30 \
	-vn \
	-D__ZX81__ \
	-DZ88DK_SPRITES \
	-DLESS_PRINT \
	-lndos \
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
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	-create-app -o  $(BUILD_PATH)/LIGHT_zx81_16k_wrx.prg \
	$(LIGHT_FILES) \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/zx81/zx81_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	rm $(BUILD_PATH)/LIGHT_zx81_16k_wrx.prg

lambda_16k:
	$(Z88DK_PATH)$(MYZ88DK) +lambda $(SCCZ80_OPTS) \
	-vn -D__LAMBDA__ -DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-lndos -create-app -o  $(BUILD_PATH)/FULL_lambda_16k.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_lambda_16k.prg		


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

msx_mode1_test:
	$(Z88DK_PATH)$(MYZ88DK) +msx -subtype=rom \
	-vn \
	-lndos \
	$(SOURCE_PATH)/../test/msx_test.c \
	-create-app -o $(BUILD_PATH)/TEST_msx.prg
	rm $(BUILD_PATH)/TEST_msx.prg

msx_32k:
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) -lmsxbios \
	-DUSE_MSX_BIOS \
	-DSOUNDS -DANIMATE_PLAYER \
	-vn -DMSX_VPOKE -DFULL_GAME -D__MSX__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_msx_32k.prg \
	-DMEMORY_MAPPED \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	-DLESS_TEXT \
	-DZ88DK_JOYSTICK \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/msx/msx_redefined_chars_init_graphics.c	\
	$(CROSS_LIB_PATH)/sound/z88dk/psg/psg_sounds.c \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_msx_32k.prg 


msx_32k_rom:
	$(Z88DK_PATH)$(MYZ88DK) +msx $(SCCZ80_OPTS) -lmsxbios \
	-DUSE_MSX_BIOS \
	-DSOUNDS -DANIMATE_PLAYER \
	-vn -DMSX_VPOKE -DFULL_GAME -D__MSX__ -DEND_SCREEN -DBETWEEN_LEVEL \
	-lndos -subtype=rom \
	-create-app -o $(BUILD_PATH)/FULL_msx_32k.rom \
	-DMEMORY_MAPPED \
	-DDEFAULT_CLEAR_SCREEN \
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
	-pragma-define:REGISTER_SP=0xd000 \
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
	# mv a.bin $(BUILD_PATH)/LIGHT_abc80.bin
	mv a.ihx $(BUILD_PATH)/LIGHT_abc80.ihx 
	# $(TOOLS_PATH)/z88dk/abc80/bin2abc --bac80 -o $(BUILD_PATH)/xchase.bac -s 49200 a.bin
	# $(TOOLS_PATH)/z88dk/abc80/dosgen$(COMPILEDEXT) $(BUILD_PATH)/LIGHT_abc80.dsk 
	# $(TOOLS_PATH)/z88dk/abc80/doscopy$(COMPILEDEXT) $(BUILD_PATH)/LIGHT_abc80.dsk -b $(BUILD_PATH)/xchase.bac
	mv a.bin $(BUILD_PATH)/LIGHT_abc80.bin
	
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
	-DNO_RANDOM_LEVEL \
	-DNO_SET_SCREEN_COLORS \
	-DNO_SLEEP \
	-DFLAT_ENEMIES \
	-lndos -create-app \
	$(FULL_FILES)
	# $(TOOLS_PATH)/z88dk/abc80/bin2bac2$(COMPILEDEXT) a.bin $(BUILD_PATH)/xchase.bac 49200
	# $(TOOLS_PATH)/z88dk/abc80/bin2abc --bac80 -o $(BUILD_PATH)/xchase.bac -s 49200 a.bin	
	# $(TOOLS_PATH)/z88dk/abc80/dosgen$(COMPILEDEXT) $(BUILD_PATH)/FULL_abc80.dsk 
	# $(TOOLS_PATH)/z88dk/abc80/doscopy$(COMPILEDEXT) $(BUILD_PATH)/FULL_abc80.dsk -b $(BUILD_PATH)/xchase.bac
	# rm a.bin 
	# mv $(BUILD_PATH)/xchase.bac $(BUILD_PATH)/FULL_abc80.bac
	mv a.ihx $(BUILD_PATH)/FULL_abc80.ihx 
	mv a.bin $(BUILD_PATH)/FULL_abc80.bin
	
	
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

z9001_16k_full:
	$(Z88DK_PATH)$(MYZ88DK) +z9001 -compiler=sdcc $(ZSDCC_OPTS) -clib=ansi \
	-D__Z9001__ -vn -DFULL_GAME  \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DLESS_TEXT \
	-DNO_BLINKING \
	-DNO_RANDOM_LEVEL \
	-DNO_HINTS \
	-DSIMPLE_STRATEGY \
	-lndos -create-app -o $(BUILD_PATH)/FULL_z9001_less_text.z80 \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_z9001_less_text.z80
	
	
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
	rm a.bin
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
	rm a.bin
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
	$(SCCZ80_OPTS) \
	-pragma-need=ansiterminal \
	-D__GAL__ -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO -DNO_INIT_GRAPHICS \
	-vn -lndos -create-app -o  $(BUILD_PATH)/FULL_galaksija.prg \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_galaksija.prg	
	

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
	-DFORCE_NARROW \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DNO_SLEEP \
	-DNO_HINTS \
	-DNO_CONTROL_INSTRUCTIONS \
	-DNO_RANDOM_LEVEL \
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
	$(SCCZ80_OPTS) \
	-D__PX8__ \
	-DCONIO_ADM3A \
	-DTINY_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app \
	$(TINY_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/TINY_px8.ROM

px8_broken:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px8 \
	$(SCCZ80_OPTS) \
	-D__PX8__ \
	-DFULL_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DDEFAULT_CLEAR_SCREEN \
	-create-app \
	$(FULL_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px8_chars.ROM

	
px8_chars_ansi:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px8ansi \
	$(SCCZ80_OPTS) \
	-D__PX8__ \
	-DFULL_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DDEFAULT_CLEAR_SCREEN \
	-create-app \
	$(FULL_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px8_chars.ROM	

	
px8_chars:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px8 \
	$(SCCZ80_OPTS) \
	-D__PX8__ \
	-DCONIO_ADM3A_WITH_UNDEF \
	-DFULL_GAME \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DDEFAULT_CLEAR_SCREEN \
	-create-app \
	$(FULL_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px8_chars.ROM	

px8:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	$(SCCZ80_OPTS) \
	-D__PX8__ \
	-DFULL_GAME \
	-subtype=px8 \
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


px4_tiny_putc4x6:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(ZSDCC_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	-DDEFAULT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	$(TOOLS_PATH)/z88dk/oz/makewzd.exe a
	rm a.bin
	mv a.wzd $(BUILD_PATH)/TINY_oz.wzd
	
	
# it fails with sdcc
z88_tiny_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +z88 -subtype=app \
	$(SCCZ80_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	$(SCCZ80_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	$(SCCZ80_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	$(SCCZ80_OPTS) \
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

	
px4_chars:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lpx4 \
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
	-D__X1__ \
	-DTINY_GAME -DNO_SLEEP \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o $(BUILD_PATH)/TINY_x1.bin -vn -lndos \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_x1.bin
	
	
x1:
	$(Z88DK_PATH)$(MYZ88DK) +x1 \
	$(SCCZ80_OPTS) \
	-D__X1__ \
	-DFULL_GAME \
	-DBETWEEN_LEVEL -DEND_SCREEN \
	-DCONIO -DNO_INIT_GRAPHICS \
	-create-app -o $(BUILD_PATH)/FULL_x1.bin -vn -lndos \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_x1.bin

trs80_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +trs80 -lndos \
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=tiki100 -ltiki100 \
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
	mv a.dsk $(BUILD_PATH)/FULL_tiki100.dsk
	rm a.bin
	rm a_HIMEM.bin
	
kaypro: 
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lgfxkp \
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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

nc200:
	$(Z88DK_PATH)$(MYZ88DK) +nc -lgfxnc200 \
	$(SCCZ80_OPTS) \
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
	

attache_chars:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=attache \
	$(SCCZ80_OPTS) \
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
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=attache \
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
	$(SCCZ80_OPTS) \
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

	
osborne1_chars:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -lgfxosborne1 -subtype=osborne1 \
	$(SCCZ80_OPTS) \
	-D__OSBORNE1__ \
	-DNO_COLOR \
	-DFORCE_CONIO_LIB \
	-DCONIO_ADM3A_WITH_UNDEF \
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
	$(SCCZ80_OPTS) \
	-D__OSBORNE1__ \
	-DNO_COLOR \
	-DFORCE_CONIO_LIB \
	-DFULL_GAME -DLESS_TEXT \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	-DANIMATE_PLAYER \
	-DALT_PRINT \
	-create-app -o$(BUILD_PATH)/FULL_osborne1.bin \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	rm $(BUILD_PATH)/FULL_osborne1.bin	
	
	
excalibur64:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=excali64 \
	$(SCCZ80_OPTS) \
	-D__CPM_80X24__ \
	-DFORCE_CONIO_LIB \
	-D__EXCALI64__ \
	-DFULL_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_excalibur64.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_excalibur64.bin	
	rm $(BUILD_PATH)/FULL_excalibur64_HIMEM.bin
	
bic:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=bic \
	$(SCCZ80_OPTS) \
	-D__CPM_80X24__ \
	-DCONIO_ADM3A_WITH_UNDEF \
	-DFORCE_CONIO_LIB \
	-D__BIC__ \
	-DFULL_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_bic.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_bic.bin		
	
qc10:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=qc10 \
	$(SCCZ80_OPTS) \
	-D__CPM_80X24__ \
	-DCONIO_ADM3A_WITH_UNDEF \
	-DFORCE_CONIO_LIB \
	-D__QC10__ \
	-DFULL_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_qc10.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_qc10.bin	
	
dmv:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=dmv \
	$(SCCZ80_OPTS) \
	-D__CPM_80X24__ \
	-DCONIO_ADM3A_WITH_UNDEF \
	-DFORCE_CONIO_LIB \
	-D__DMV__ \
	-DFULL_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_dmv.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_dmv.bin	


sv8000:
	$(Z88DK_PATH)$(MYZ88DK) +sv8000 -subtype=32k \
	$(SCCZ80_OPTS) \
	-DFORCE_CONIO_LIB \
    -DZ88DK_JOYSTICK \
	-D__SV8000__ \
	-DFULL_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-create-app -o$(BUILD_PATH)/FULL_sv8000.bin \
	$(SOURCE_PATH)/end_screen.c \
	$(FULL_FILES)
	rm $(BUILD_PATH)/FULL_sv8000.bin	
    
cpm_adm3a:
	$(Z88DK_PATH)$(MYZ88DK) +cpm \
	$(SCCZ80_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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


ti86:
	$(Z88DK_PATH)$(MYZ88DK) +ti86 \
	$(SCCZ80_OPTS) \
	-D__TI86__ \
	-DFULL_GAME \
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
	$(FULL_FILES)
	mv $(SOURCE_PATH)/../a.86p $(BUILD_PATH)/FULL_ti86.86p	
	rm $(SOURCE_PATH)/../a.bin	
	
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

		
m5_tiny:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	$(SCCZ80_OPTS) \
	-lm -create-app -Cz--audio -subtype=tape \
	-D__M5__ \
	-clib=ansi -pragma-define:ansicolumns=32 \
	-DTINY_GAME -DLESS_TEXT \
	-DCONIO -DNO_INIT_GRAPHICS \
	-o$(BUILD_PATH)/TINY_m5.bin \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_m5.bin
	rm $(BUILD_PATH)/TINY_m5.cas


m5_udg:
	$(Z88DK_PATH)$(MYZ88DK) +m5 \
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	rm $(BUILD_PATH)/FULL_ts2068_BANK_7.bin	
		
	
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
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-create-app	
	rm a.bin
	mv a.COM $(BUILD_PATH)/FULL_aussie.COM
	
# TODO: Try -subtype=com
enterprise_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +enterprise  \
	$(ZSDCC_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
	-DSTDLIB \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-create-app
	mv a.app FULL_enterprise.app
	rm a.bin
	tar czvf FULL_enterprise.tar.zip FULL_enterprise.app
	rm FULL_enterprise.app
	mv FULL_enterprise.tar.zip $(BUILD_PATH)


mz2500_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +mz2500 \
	$(ZSDCC_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
	-DSTDLIB \
	-DNO_WAIT \
	-DTURN_BASED \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	-create-app 
	mv a.2D $(BUILD_PATH)/FULL_mz2500_buffered.2D
	rm a.bin

mz2500_random:
	$(Z88DK_PATH)$(MYZ88DK) +mz2500 -compiler=sdcc -SO3 $(SCCZ80_OPTS) \
	$(SOURCE_PATH)/../experiments/random_test.c \
	-create-app -o $(BUILD_PATH)/mz2500_random_test.bin

# It almost works except for TURN_BASED
mz2500_tiny_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +mz2500 \
	$(ZSDCC_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	-DNO_SLEEP \
	-DNO_INITIAL_SCREEN \
	-DNO_SET_SCREEN_COLORS \
	-lndos \
	-DBUFFERED -DNO_INIT_GRAPHICS \
	-DFORCE_NARROW \
	-DNO_STATS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
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
	$(ZSDCC_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES) \
	-create-app
	rm a.bin
	mv A.COM $(BUILD_PATH)/TINY_CPM_TINY_BUFFERED.COM
	

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

	
# ------------------------------------------------------------------------------------------
# CMOC
#

dragon:
	cmoc \
	$(DRAGON_OPTS) \
	-c $(CROSS_LIB_PATH)/display/display_macros.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(CROSS_LIB_PATH)/display/init_images.c	
	cmoc \
	$(DRAGON_OPTS) \
	-c $(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(CROSS_LIB_PATH)/input/input_macros.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(CROSS_LIB_PATH)/text/text_macros.c	
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/move_player.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/ghost.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/level.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/character.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/game_text.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/strategy.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/item.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/bullet.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/skull.c	
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/horizontal_missile.c
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/rocket.c 
	cmoc \
	$(DRAGON_OPTS) \
	-c $(SOURCE_PATH)/end_screen.c	
	cmoc -o $(BUILD_PATH)/xchase.bin \
	$(DRAGON_OPTS) \
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
	$(TOOLS_PATH)/cmoc/dragon/bin2cas.pl -o $(BUILD_PATH)/FULL_Dragon.cas -D $(BUILD_PATH)/xchase.bin -A
	$(TOOLS_PATH)/cmoc/dragon/File2VDK.exe $(BUILD_PATH)/xchase.bin
	mv $(BUILD_PATH)/xchase.bin.vdk $(BUILD_PATH)/FULL_Dragon.vdk
	mv $(BUILD_PATH)/xchase.bin $(BUILD_PATH)/FULL_Dragon.bin

	
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
	cmoc -o $(BUILD_PATH)/FULL_CoCo.bin \
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
	rm -rf $(BUILD_PATH)/FULL_CoCo.dsk
	cp $(BUILD_PATH)/FULL_CoCo.bin XCHASE.BIN
	$(TOOLS_PATH)/cmoc/coco/file2dsk XCHASE.BIN
	mv XCHASE.DSK $(BUILD_PATH)/FULL_CoCo.dsk
	$(TOOLS_PATH)/cmoc/dragon/bin2cas.pl -o $(BUILD_PATH)/FULL_CoCo.cas -C XCHASE.BIN -A
	rm XCHASE.BIN
	
      
coco_test_rand: 
	cmoc -O0 -c $(SOURCE_PATH)/../experiments/coco_rand_test.c
	cmoc -O0 -o $(BUILD_PATH)/coco_rand_test.bin $(SOURCE_PATH)/../coco_rand_test.o

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

mo5: 
	cmoc \
	$(MO5_OPTS) \
	-c $(CROSS_LIB_PATH)/display/alt_print/mo5_alt_print.c 
	cmoc \
	$(MO5_OPTS) \
	-c $(CROSS_LIB_PATH)/display/init_graphics/cmoc/mo5/mo5_init_graphics.c
	cmoc \
	$(MO5_OPTS) \
	-c $(CROSS_LIB_PATH)/display/display_macros.c
	cmoc \
	$(MO5_OPTS) \
	-c $(CROSS_LIB_PATH)/display/init_images.c	
	cmoc \
	$(MO5_OPTS) \
	-c $(CROSS_LIB_PATH)/input/input_macros.c
	cmoc \
	$(MO5_OPTS) \
	-c $(CROSS_LIB_PATH)/text/text_macros.c	
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/move_player.c
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/ghost.c
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/level.c
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/character.c
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/game_text.c
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/strategy.c
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/item.c
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/bullet.c
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/skull.c	
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/horizontal_missile.c
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/rocket.c 
	cmoc \
	$(MO5_OPTS) \
	-c $(SOURCE_PATH)/end_screen.c	
	cmoc \
	$(MO5_OPTS) \
	-c $(CROSS_LIB_PATH)/display/graphics_data/cmoc/xchase.s	
	cmoc -o $(BUILD_PATH)/FULL_MO5.bin \
	$(MO5_OPTS) \
	$(SOURCE_PATH)/main.c \
	mo5_alt_print.o \
	mo5_init_graphics.o \
	display_macros.o \
	xchase.o \
	init_images.o \
	ghost.o \
	move_player.o \
	level.o character.o \
	text_macros.o game_text.o \
	strategy.o \
	input_macros.o \
	item.o bullet.o skull.o \
	rocket.o horizontal_missile.o end_screen.o
	rm \
	mo5_alt_print.o \
	xchase.o \
	mo5_init_graphics.o \
	display_macros.o \
	init_images.o \
	ghost.o \
	move_player.o \
	level.o character.o \
	game_text.o text_macros.o \
	strategy.o \
	input_macros.o \
	item.o bullet.o skull.o \
	rocket.o horizontal_missile.o end_screen.o	
	$(TOOLS_PATH)/cmoc/mo5/f2k5.exe $(BUILD_PATH)/FULL_MO5.bin
	mv $(BUILD_PATH)/FULL_MO5.bin .
	$(TOOLS_PATH)/cmoc/mo5/sapfs.exe -c FULL_MO5.sap
	$(TOOLS_PATH)/cmoc/mo5/sapfs.exe --add FULL_MO5.sap FULL_MO5.bin
	mv ./FULL_MO5.bin $(BUILD_PATH)
	mv ./FULL_MO5.sap $(BUILD_PATH)
	mv ./FILE.k5 $(BUILD_PATH)/FULL_MO5.k5
	

to7:
	cmoc \
	$(TO7_OPTS) \
	-c $(CROSS_LIB_PATH)/display/init_graphics/cmoc/mo5/mo5_init_graphics.c
	cmoc \
	$(TO7_OPTS) \
	-c $(CROSS_LIB_PATH)/display/display_macros.c
	cmoc \
	$(TO7_OPTS) \
	-c $(CROSS_LIB_PATH)/display/init_images.c	
	cmoc \
	$(TO7_OPTS) \
	-c $(CROSS_LIB_PATH)/input/input_macros.c
	cmoc \
	$(TO7_OPTS) \
	-c $(CROSS_LIB_PATH)/text/text_macros.c	
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/move_player.c
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/ghost.c
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/level.c
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/character.c
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/game_text.c
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/strategy.c
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/item.c
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/bullet.c
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/skull.c	
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/horizontal_missile.c
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/rocket.c 
	cmoc \
	$(TO7_OPTS) \
	-c $(SOURCE_PATH)/end_screen.c	
	cmoc -o $(BUILD_PATH)/FULL_TO7.bin \
	$(TO7_OPTS) \
	$(SOURCE_PATH)/main.c \
	mo5_init_graphics.o \
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
	rm \
	mo5_init_graphics.o \
	display_macros.o \
	init_images.o \
	ghost.o \
	move_player.o \
	level.o character.o \
	game_text.o text_macros.o \
	strategy.o \
	input_macros.o \
	item.o bullet.o skull.o \
	rocket.o horizontal_missile.o end_screen.o	
	mv $(BUILD_PATH)/FULL_TO7.bin .
	$(TOOLS_PATH)/cmoc/mo5/sapfs.exe -c FULL_TO7.sap
	$(TOOLS_PATH)/cmoc/mo5/sapfs.exe --add FULL_TO7.sap FULL_TO7.bin
	mv ./FULL_TO7.bin $(BUILD_PATH)
	mv ./FULL_TO7.sap $(BUILD_PATH)
	
	
mo5_test: 
	cmoc -c -nodefaultlibs $(MO5_OPTS) ./experiments/mo5.c
	cmoc -nodefaultlibs $(MO5_OPTS) -o $(BUILD_PATH)/mo5.bin ./mo5.o
	rm ./mo5.o
	$(TOOLS_PATH)/cmoc/mo5/f2k5.exe $(BUILD_PATH)/mo5.bin
	mv $(BUILD_PATH)/mo5.bin .
	$(TOOLS_PATH)/cmoc/mo5/sapfs.exe -c mo5.sap
	$(TOOLS_PATH)/cmoc/mo5/sapfs.exe --add mo5.sap mo5.bin
	mv ./mo5.bin $(BUILD_PATH)
	mv ./mo5.sap $(BUILD_PATH)
	mv ./FILE.k5 $(BUILD_PATH)/mo5.k5
	
	

# ------------------------------------------------------------------------------------------
# ACK
#

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
	-DDEFAULT_CLEAR_SCREEN \
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
	-DDEFAULT_CLEAR_SCREEN \
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

cpm8080_targets: \
	cpm8080_buffered


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
	-DDEFAULT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv pc86.img $(BUILD_PATH)/FULL_pc8086_turn_based.img
	rm $(SOURCE_PATH)/../*.o

pc8086_targets: \
	pc8086

	
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
	-DDEFAULT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv linuxppc.exe $(BUILD_PATH)/FULL_linuxppc_turn_based.exe
	rm $(SOURCE_PATH)/../*.o

linuxppc_targets: \
	linuxppc


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
	-DDEFAULT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv linux68k.exe $(BUILD_PATH)/FULL_linux68k_turn_based.exe
	rm $(SOURCE_PATH)/../*.o
	
linux68k_targets: \
	linux68k


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
	-DDEFAULT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv linux386.exe $(BUILD_PATH)/FULL_linux386_turn_based.exe
	rm $(SOURCE_PATH)/../*.o

linux386_targets: \
	linux386


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
	-DDEFAULT_CLEAR_SCREEN \
	-DALT_PRINT \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c
	mv linuxmips.exe $(BUILD_PATH)/FULL_linuxmips_turn_based.exe
	rm $(SOURCE_PATH)/../*.o	

linuxmips_targets: \
	linuxmips


# buffered
pdp11v7_buffered:
	$(_ACK) $(_ACK_OPTS) -mpdpv7 \
	-DFORCE_XSIZE=78 \
	-DFORCE_YSIZE=22 \
	-DCONIO_VT100 \
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
	-DDEFAULT_CLEAR_SCREEN \
	$(FULL_FILES)
	mv a.out $(BUILD_PATH)/FULL_pdp11v7_turn_based.bin
	rm $(SOURCE_PATH)/../*.o
	
    
pdp11v7:
	$(_ACK) $(_ACK_OPTS) -mpdpv7 \
	-DFORCE_XSIZE=78 \
	-DFORCE_YSIZE=22 \
	-DCONIO_VT100 \
	-DCONIO \
	-DNO_COLOR \
	-DACK \
	-D__PDP11V7__ \
	-DFULL_GAME \
	-DNO_INIT_GRAPHICS \
	-DNO_PRINT \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DDEFAULT_CLEAR_SCREEN \
	$(FULL_FILES)
	mv a.out $(BUILD_PATH)/FULL_pdp11v7.bin
	rm $(SOURCE_PATH)/../*.o    

pdp11v7_targets: \
	pdp11v7 \
	pdp11v7_buffered


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
	$(CROSS_LIB_PATH)/display/init_graphics/gcc/ncurses_init_graphics.c \
	$(FULL_FILES) \
	-lncurses
	# mv a.exe $(BUILD_PATH)/FULL_dos_turn_based.exe

dos_targets: \
	dos

# ------------------------------------------------------------------------------------------
# GCC
#

ncurses_turn_based:
	$(GCC) $(GCC_OPTS) -D__NCURSES__ \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DTURN_BASED \
	-DCONIO  \
	-DALT_PRINT \
	$(CROSS_LIB_PATH)/display/alt_print/ncurses_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/gcc/ncurses_init_graphics.c \
	$(FULL_FILES) \
	-lncurses
	mv a$(COMPILEDEXT) $(BUILD_PATH)/FULL_ncurses_turn_based$(COMPILEDEXT)
	
ncurses:
	$(GCC) $(GCC_OPTS) -D__NCURSES__ \
	-DFULL_GAME \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO  \
	-DALT_PRINT \
	-DNO_TITLE_LINE \
	$(CROSS_LIB_PATH)/display/alt_print/ncurses_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/gcc/ncurses_init_graphics.c \
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
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-create-app
	rm a.bin
	mv A.COM $(BUILD_PATH)/FULL_CPM_BUFFERED.COM
	# cp $(BUILD_PATH)/FULL_CPM_BUFFERED.COM $(BUILD_PATH)/CPM.COM
	
	

amiga_core_test:
	$(AMIGA_CC) -mcrt=nix13 \
	$(SOURCE_PATH)/../test/main_use_amiga_core.c

amiga_buffered:
	$(AMIGA_CC) $(AMIGA_OPTS) -mcrt=nix13 \
	-D__AMIGA__ \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=39 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_COLOR \
	-DNO_SLEEP \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv a.out $(BUILD_PATH)/FULL_amiga_buffered.exe

emcc_buffered:
	$(EM_CC) $(EM_OPTS)  \
	-D__AMIGA__ \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=80 \
	-DFORCE_YSIZE=24 \
	-DSTDLIB \
	-DNO_COLOR \
	-DNO_SLEEP \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES) \
	-o FULL_emcc_buffered.html
	mv FULL_emcc_buffered.js $(BUILD_PATH)
	mv FULL_emcc_buffered.wasm $(BUILD_PATH)
	mv FULL_emcc_buffered.html $(BUILD_PATH)

atari_st_buffered:
	$(ATARI_ST_CC) $(ATARI_ST_OPTS) \
	-D__ATARI_ST__ \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=39 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_COLOR \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv a.out $(BUILD_PATH)/FULL_atari_st_buffered.tos
	
atari_st_ncurses:
	$(ATARI_ST_CC) $(ATARI_ST_OPTS) -D__NCURSES__ \
	-D__ATARI_ST__ \
	-DFULL_GAME \
	-DFORCE_XSIZE=39 \
	-DFORCE_YSIZE=22 \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO  \
	-DALT_PRINT \
	-DNO_TITLE_LINE \
	$(CROSS_LIB_PATH)/display/alt_print/ncurses_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/gcc/ncurses_init_graphics.c \
	$(FULL_FILES) \
	-lncurses
	mv a.out $(BUILD_PATH)/FULL_atari_st_ncurses.tos
    


atari_st_ncurses_turn_based:
	$(ATARI_ST_CC) $(ATARI_ST_OPTS) -D__NCURSES__ \
	-D__ATARI_ST__ \
	-DTURN_BASED \
	-DFULL_GAME \
	-DFORCE_XSIZE=39 \
	-DFORCE_YSIZE=22 \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO  \
	-DNO_SLEEP  \
	-DALT_PRINT \
	-DNO_TITLE_LINE \
	$(CROSS_LIB_PATH)/display/alt_print/ncurses_alt_print.c \
	$(SOURCE_PATH)/end_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/gcc/ncurses_init_graphics.c \
	$(FULL_FILES) \
	-lncurses
	mv a.out $(BUILD_PATH)/FULL_atari_st_ncurses_turn_based.tos
    
    
gcc_buffered:
	$(GCC) $(GCC_OPTS) \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_COLOR \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv a$(COMPILEDEXT) $(BUILD_PATH)/FULL_gcc_buffered$(COMPILEDEXT)

	
gcc_no_print:
	$(GCC) $(GCC_OPTS) \
	-I$(SOURCE_PATH) \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_COLOR \
	-DTINY_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DNO_RANDOM_LEVEL \
	-DNO_PRINT \
	$(TINY_FILES)	


gcc_tiny_buffered:
	$(GCC) $(GCC_OPTS) \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_COLOR \
	-DTINY_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DNO_RANDOM_LEVEL \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES)
	mv a$(COMPILEDEXT) $(BUILD_PATH)/TINY_gcc_buffered$(COMPILEDEXT)

# ------------------------------------------------------------------------------------------
# XTC68
#


XTC68_ALL_OPTS ?= \
	$(XTC68_OPTS) \
	-I$(SOURCE_PATH) \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_COLOR \
	-DTINY_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	-DNO_WAIT \
	-DNO_SLEEP \
	-DNO_RANDOM_LEVEL \
	-DNO_PRINT 	
	
	

sinclair_ql_tiny_buffered:
	$(XTC68) $(XTC68_ALL_OPTS) -c $(SOURCE_PATH)/level.c -o level.o
	$(XTC68) $(XTC68_ALL_OPTS) -c $(CROSS_LIB_PATH)/display/init_images.c -o init_images.o 
	$(XTC68) $(XTC68_ALL_OPTS) -c $(CROSS_LIB_PATH)/display/display_macros.c -o display_macros.o 
	$(XTC68) $(XTC68_ALL_OPTS) -c $(CROSS_LIB_PATH)/input/input_macros.c -o input_macros.o 
	$(XTC68) $(XTC68_ALL_OPTS) -c $(CROSS_LIB_PATH)/text/text_macros.c -o text_macros.o 
	$(XTC68) $(XTC68_ALL_OPTS) -c $(SOURCE_PATH)/move_player.c -o move_player.o 
	$(XTC68) $(XTC68_ALL_OPTS) -c $(SOURCE_PATH)/ghost.c  -o ghost.o 
	$(XTC68) $(XTC68_ALL_OPTS) -c $(SOURCE_PATH)/character.c -o character.o 
	$(XTC68) $(XTC68_ALL_OPTS) -c $(SOURCE_PATH)/game_text.c -o game_test.o 
	$(XTC68) $(XTC68_ALL_OPTS) -c $(SOURCE_PATH)/strategy.c -o strategy.o 
	$(XTC68) $(XTC68_ALL_OPTS) -c $(SOURCE_PATH)/main.c -o main.o 
	$(XTC68) $(XTC68_ALL_OPTS) /usr/local/qdos/lib/crt.o \
	level.o init_images.o display_macros.o input_macros.o text_macros.o move_player.o ghost.o character.o game_test.o strategy.o main.o 
	rm level.o init_images.o display_macros.o input_macros.o text_macros.o move_player.o ghost.o character.o game_test.o strategy.o main.o
	mv a_out $(BUILD_PATH)/TINY_SinclairQL_buffered.out
	
	
# ------------------------------------------------------------------------------------------
# TMS9000-GCC
#   
    

# Paths to TMS9900 compilation tools
GAS=tms9900-as
LD=tms9900-ld
TI99CC=tms9900-gcc
CXX=tms9900-c++
OBJCOPY=tms9900-objcopy

# Flags used during linking
# Refer to the linker rules in an external file
LDFLAGS=\
  --script=$(CFG_PATH)gcc4ti99/linkfile.cfg

# List of compiled objects used in executable
OBJECT_LIST= \
  $(TOOLS_PATH)/ti99/cart_header.o \
  $(TOOLS_PATH)/ti99/crt0.o \
  $(CROSS_LIB_PATH)/display/display_macros.o \
  $(CROSS_LIB_PATH)/display/init_images.o \
  $(CROSS_LIB_PATH)/input/input_macros.o \
  $(CROSS_LIB_PATH)/text/text_macros.o \
  $(SOURCE_PATH)/move_player.c \
  $(SOURCE_PATH)/ghost.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
  $(SOURCE_PATH)/main.c
  

# Recipes to compile individual files
%.o: %.asm
	$(GAS) $< -o $@
%.o: %.c
	$(TI99CC) -c $< -O2 -o $@
%.o: %.cpp
	$(CXX) -c $< -O2 -o $@

# List of all files needed in executable
PREREQUISITES=\
  $(OBJECT_LIST)
  
# Recipe to compile the executable
ti99: ti99_cross_chase.elf
	$(OBJCOPY) -O binary -j .text -j .data -j .ctors ti99_cross_chase.elf ti99_cross_chase.cart

ti99_cross_chase.elf: $(PREREQUISITES)
	$(LD) $(OBJECT_LIST) $(LDFLAGS) -o $@

# Recipe to clean all compiled objects
.phony ti99clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.cart


    
    
ti99_tiny_no_graphics:
	$(TI99_CC) $(TI99_OPTS) \
	-I$(SOURCE_PATH) \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DNO_PRINT \
	-DTINY_GAME \
	-DNO_WAIT \
	-DTURN_BASED \
	-DNO_GRAPHICS \
	-DNO_INPUT \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/rand/rand.c \
	$(TOOLS_PATH)/ti99/cart_header.o \
	$(TOOLS_PATH)/ti99/crt0.o
	
cc_tiny_no_graphics:
	$(_CC) $(_CC_OPTS) \
	-I$(SOURCE_PATH) \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DNO_PRINT \
	-DTINY_GAME \
	-DNO_WAIT \
	-DTURN_BASED \
	-DNO_GRAPHICS \
	-DNO_INPUT \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(TINY_FILES)
	
# ------------------------------------------------------------------------------------------
# SDCC
#
	
sdcc_display_macros: 
	sdcc -c $(INCLUDE_OPTS) -I$(SOURCE_PATH) $(MACROS_NO_GRAPHICS) $(CROSS_LIB_PATH)/display/display_macros.c
	
# ------------------------------------------------------------------------------------------
# WINCMOC
#
	
tiny_combo.cat:
	cat \
	$(TINY_FILES) \
	> tiny_combo.cat


light_combo.cat: tiny_combo.cat
	cat \
	tiny_combo.cat \
	$(LIGHT_ONLY_FILES) \
	> light_combo.cat
	rm tiny_combo.cat


full_combo.cat: light_combo.cat
	cat \
	light_combo.cat \
	$(FULL_ONLY_FILES) \
	> full_combo.cat
	rm light_combo.cat


tiny_combo_rand.cat: tiny_combo.cat
	cat \
	$(CROSS_LIB_PATH)/rand/rand.c \
	tiny_combo.cat \
	> tiny_combo_rand.cat
	rm tiny_combo.cat
    
light_combo_rand.cat: light_combo.cat
	cat \
	$(CROSS_LIB_PATH)/rand/rand.c \
	light_combo.cat \
	> light_combo_rand.cat
	rm light_combo.cat
    
    
full_combo_rand.cat: full_combo.cat
	cat \
	$(CROSS_LIB_PATH)/rand/rand.c \
	full_combo.cat \
	> full_combo_rand.cat
	rm full_combo.cat

wincmoc_debug: tiny_combo_rand.cat 
	mv tiny_combo_rand.cat tiny_combo_rand.c
	$(WINCMOC_DIR)cmoc.exe  $(INCLUDE_ALL) $(WINCMOC_OPTS) \
	$(DEBUG_MACROS) \
	tiny_combo_rand.c
	$(WINCMOC_DIR)lwasm.exe -f obj -o tiny_combo_rand.o tiny_combo_rand.asm



# ------------------------------------------------------------------------------------------
# LCC1802
#    -target=xr18CX
# https://sites.google.com/site/lcc1802/downloads/Quarrantine.zip?attredirects=0&d=1


comx_hello0: 
	$(LCC1802)  -O -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/hwcomx.c
	mv a.hex $(BUILD_PATH)/comx_hello0.hex


comx_hello: 
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/hwcomx.c
	mv a.hex $(BUILD_PATH)/comx_hello.hex


comx_hello1: 
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/hwcomx1.c
	mv a.hex $(BUILD_PATH)/comx_hello1.hex


comx_hello2: 
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/hwcomx2.c
	mv a.hex $(BUILD_PATH)/comx_hello2.hex

comx_hello_integer: 
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/comx_hello_integer.c
	mv a.hex $(BUILD_PATH)/comx_hello_integer.hex


comx_gotoxy: 
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/hwcomx_gotoxy.c
	mv a.hex $(BUILD_PATH)/hwcomx_gotoxy.hex


comx_gotoxy2: 
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/hwcomx_gotoxy2.c
	mv a.hex $(BUILD_PATH)/hwcomx_gotoxy2.hex


comx_mod: 
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/comx_mod.c
	mv a.hex $(BUILD_PATH)/comx_mod.hex


comx_pp_debug0: 
	$(LCC1802) -Wf-volatile -target=xr18CX -D_FOO_DEF \
	$(SOURCE_PATH)/../test/hwcomx_pp_debug.c
	mv a.hex $(BUILD_PATH)/comx_pp_debug0.hex


comx_size_debug:
	$(LCC1802) -Wf-volatile -target=xr18CX -D_FOO_DEF \
	$(SOURCE_PATH)/../test/comx_size_debug.c
	mv a.hex $(BUILD_PATH)/comx_size_debug.hex


comx_conv_debug:
	$(LCC1802) -S -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/comx_conv_debug.c
	mv a.hex $(BUILD_PATH)/comx_conv_debug.hex


comx_class:
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/comx_class.c
	mv a.hex $(BUILD_PATH)/comx_class.hex


comx_simple_class:
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/comx_simple_class.c
	mv a.hex $(BUILD_PATH)/comx_simple_class.hex


comx_simplest_class:
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/comx_simplest_class.c
	mv a.hex $(BUILD_PATH)/comx_simplest_class.hex


comx_pointer2func:
	$(LCC1802) -Wf-volatile -target=xr18CX \
	$(SOURCE_PATH)/../test/comx_pointer2func.c
	mv a.hex $(BUILD_PATH)/comx_pointer2func.hex


tiny_comx_combo.cat: tiny_combo_rand.cat
	cat \
	$(CROSS_LIB_PATH)/display/clear_screen/lcc1802/comx/comx_clear_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/lcc1802/comx/comx_init_graphics.c \
	tiny_combo_rand.cat > tiny_comx_combo.cat
	rm tiny_combo_rand.cat

light_comx_combo.cat: light_combo_rand.cat
	cat \
	$(CROSS_LIB_PATH)/display/clear_screen/lcc1802/comx/comx_clear_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/lcc1802/comx/comx_init_graphics.c \
	light_combo_rand.cat > light_comx_combo.cat
	rm light_combo_rand.cat
    
full_comx_combo.cat: full_combo_rand.cat
	cat \
	$(CROSS_LIB_PATH)/display/clear_screen/lcc1802/comx/comx_clear_screen.c \
	$(CROSS_LIB_PATH)/display/init_graphics/lcc1802/comx/comx_init_graphics.c \
	full_combo_rand.cat > full_comx_combo.cat
	rm full_combo_rand.cat

COMX_DEBUG_MACROS ?= \
	-D__LCC1802__ \
	-DFORCE_NO_CONIO_LIB \
	-DNO_WAIT \
	-DFORCE_GHOSTS_NUMBER=7 \
	-DROUND_ENEMIES \
	-DNO_SET_SCREEN_COLORS \
	-DNO_PRINT \
	-DNO_SLEEP \
	-DNO_COLOR \
	-DALT_RAND \
	-DMEMORY_MAPPED


tiny_comx: tiny_comx_combo.cat
	mv tiny_comx_combo.cat tiny_comx_combo.c
	$(LCC1802)  $(LC1802_OPTS) "-target=xr18CX" "-Wf-g,;"  "-Wf-volatile" \
	-DFORCE_XSIZE=20 \
	-DFORCE_YSIZE=21 \
	-D__COMX__ \
	-DTINY_GAME \
	-DLCC1802_JOYSTICK \
	-DANIMATE_PLAYER \
	-DALT_CLEAR_SCREEN \
	$(COMX_DEBUG_MACROS) \
	tiny_comx_combo.c
	# rm tiny_comx_combo.c
	mv a.hex $(BUILD_PATH)/TINY_comx_debug.hex


light_comx: light_comx_combo.cat
	mv light_comx_combo.cat light_comx_combo.c
	$(LCC1802)  $(LC1802_OPTS) "-target=xr18CX" "-Wf-g,;"  "-Wf-volatile"  \
	-D__COMX__ \
	-DLIGHT_GAME \
	-DLCC1802_JOYSTICK \
	-DANIMATE_PLAYER \
	-DALT_CLEAR_SCREEN \
	$(COMX_DEBUG_MACROS) \
	light_comx_combo.c
	# rm light_comx_combo.c
	mv a.hex $(BUILD_PATH)/LIGHT_comx_debug.hex


full_comx : full_comx_combo.cat
	mv full_comx_combo.cat full_comx_combo.c
	$(LCC1802)  $(LC1802_OPTS) "-target=xr18CX" "-Wf-g,;"  "-Wf-volatile"  \
	-D__COMX__ \
	-DFULL_GAME \
	-DLCC1802_JOYSTICK \
	-DANIMATE_PLAYER \
	-DALT_CLEAR_SCREEN \
	$(COMX_DEBUG_MACROS) \
	full_comx_combo.c
	# rm full_comx_combo.c
	mv a.hex $(BUILD_PATH)/FULL_comx_debug.hex


tiny_comx_debug: tiny_comx

comx_targets: 
	make tiny_comx 
	make light_comx 
	make full_comx

comx_pp_debug: 
	$(LCC1802)  $(LC1802_OPTS) -Wf-volatile -target=xr18CX \
	-DFORCE_XSIZE=40 \
	-DFORCE_YSIZE=24 \
	-D__COMX__ \
	-DANIMATE_PLAYER \
	-DALT_CLEAR_SCREEN \
	$(COMX_DEBUG_MACROS) \
	$(SOURCE_PATH)/../test/comx_pp_debug.c
	mv a.hex $(BUILD_PATH)/comx_pp_debug.hex



lcc1802_debug : tiny_combo_rand.cat
	mv tiny_combo_rand.cat tiny_combo_rand.c
	$(LCC1802)  $(LC1802_OPTS) -Wf-volatile \
	$(DEBUG_MACROS) \
	-DALT_RAND \
	tiny_combo_rand.c
	rm tiny_combo_rand.c
	mv a.hex $(BUILD_PATH)/TINY_lcc1802_debug.hex

# ------------------------------------------------------------------------------------------
# Z8K-PCOS-GCC
#

	
olivetti_m20_buffered:
	$(Z8K-PCOS-GCC) $(Z8K-PCOS-GCC_OPTS) \
	-D__GCC_BUFFERED__ \
	-D__OLIVETTI_M20__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_COLOR \
	-DFULL_GAME \
	-DTURN_BASED \
	-DBUFFERED \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	cp $(TOOLS_PATH)/olivetti_m20/pcos20h_empty.img FULL_m20_buffered.img
	$(TOOLS_PATH)/olivetti_m20/m20 FULL_m20_buffered.img put aout.cmd
	rm aout.cmd
	mv FULL_m20_buffered.img $(BUILD_PATH)
	
	
olivetti_m20_no_graphics:
	$(Z8K-PCOS-GCC) $(Z8K-PCOS-GCC_OPTS) \
	-D__GCC_BUFFERED__ \
	-DFORCE_XSIZE=79 \
	-DFORCE_YSIZE=22 \
	-DSTDLIB \
	-DNO_COLOR \
	-DFULL_GAME \
	-DTURN_BASED \
	-DNO_GRAPHICS \
	-DNO_INIT_GRAPHICS \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(FULL_FILES)
	mv a$(COMPILEDEXT) $(BUILD_PATH)/FULL_gcc_buffered$(COMPILEDEXT)		
	
.PHONY: mtx vic20exp_8k vic20exp_16k  atari_color atari_no_color atari_no_color_16k atari5200 atmos c128_40col c128_80col c16_16k c16_32k c64 pet cbm510 cbm610 nes apple2 apple2enh

# xtc86 targets
sinclair_ql_targets: \
	sinclair_ql_tiny_buffered


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
	cx16_targets \
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
	supervision_targets \
	telestrat_targets 


# Number of systems: 41 - 1 (c128_z80) = 40


zsdcc_test: \
	aquarius_zsdcc_test \
	c128_z80_zsdcc_test \
	mc1000_zsdcc_test \
	spectrum_zsdcc_test \
	svi_zsdcc_test \
	vz200_zsdcc_test \
	vg5k_zsdcc_test \
	zx81_zsdcc_test 


sccz80_test: \
	abc80_test \
	ace_test \
	aquarius_test \
	c128_z80_test \
	cpc_test \
	cpm_test \
 	eg2k_test \
	einstein_test \
	gal_test \
	kc_test \
	lambda_test \
	m5_test \
	mc1000_test \
	microbee_test \
	msx_test \
	mtx_test \
	mz_test \
	nascom_test \
	nc100_test \
	p2000_test \
	pc6001_test \
	pps_test \
	pv2000_test \
	px4_test \
	px8_test \
	samcoupe_test \
	sc3000_test \
	sg1000_test \
	spectrum_test \
	srr_test \
	svi_test \
	ti82_test \
	ti83_test \
	ti85_test \
	trs80_test \
	vg5k_test \
	vz200_test \
	x1_test \
	z1013_test \
	z9001_test \
	zx80_test \
	zx81_test 
	
z88dk_test: \
	sccz80_test \
	zsdcc_test	
	

# broken : 	

#OK

# missing: abc800, xo7, c7420, rex
# missing 8080: m100
z88dk_targets: \
	abc80_targets \
	ace_targets \
	alphatro_targets \
	aquarius_targets \
	attache_targets \
	aussie_targets \
	bic_targets \
	c128_z80_targets \
	camputers_lynx_targets \
	coleco_targets \
	cpc_targets \
	cpm_targets \
	dmv_targets \
 	eg2k_targets \
	einstein_targets \
	enterprise_targets \
	excalibur64_targets \
	fp1100_targets \
	g800_targets \
	gal_targets \
	gb_targets \
	hemc_targets \
	hgmc_targets \
	homelab2_targets \
	homelab_targets \
	kaypro_targets \
	kc_targets \
	kramermc_targets \
	lambda_targets \
	laser500_targets \
	m100_targets \
	m5_targets \
	mc1000_targets \
	microbee_targets \
	mikro80_targets \
	msx_targets \
	mtx_targets \
	multi8_targets \
	myvision_targets \
	mz_targets \
	mz2500_targets \
	nascom_targets \
	nc100_targets \
	nc200_targets \
	newbrain_targets \
	osborne1_targets \
	osca_targets \
	oz_targets \
	pasopia7_targets \
	p2000_targets \
	pc6001_targets \
	pc88_targets \
	pmd85_targets \
	pps_targets \
	primo_targets \
	pv1000_targets \
	pv2000_targets \
	px4_targets \
	px8_targets \
	qc10_targets \
	rc700_targets \
	rx78_targets \
	samcoupe_targets \
	sc3000_targets \
	smc777_targets \
	sms_targets \
	sos_targets \
	spc1000_targets \
	spectrum_targets \
	srr_targets \
	super80_targets \
	sv8000_targets \
	svi_targets \
	ti82_targets \
	ti83_targets \
	ti85_targets \
	ti86_targets \
	ti8x_targets \
	tiki100_targets \
	trs80_targets \
	ts2068_targets \
	tvc_targets \
	vector06c_targets \
	vg5k_targets \
	vz200_targets \
	x1_targets \
	z1013_targets \
	z80tvgame_targets \
	z88_targets \
	z9001_targets \
	zx80_targets \
	zx81_targets

cmoc_targets: \
	coco dragon mo5
	
        
ack_targets: \
	pdp11v7_targets \
    cpm8080_targets \
    pc8086_targets \
    linux386_targets \
    linux68k_targets \
    linuxppc_targets \
    linuxmips_targets
	
atari_st_targets: \
	atari_st_buffered atari_st_ncurses

amiga_targets: \
	amiga_buffered

gcc_targets: \
	gcc_buffered ncurses_targets  
    
# All targets that can be compiled under CYGWIN, i.e., not including m20
all: cc65_targets z88dk_targets cmoc_targets gcc_targets ack_targets atari_st_targets sinclair_ql_targets amiga_targets

clean_chase_obj:
	rm -rf $(SOURCE_PATH)/*.o
	rm -rf $(SOURCE_PATH)/wincmoc_main.c
	rm -rf $(SOURCE_PATH)/wincmoc_main.asm
	rm -rf $(SOURCE_PATH)/wincmoc_main.coco.bin
	rm -rf $(SOURCE_PATH)/../full_comx_combo.c
	rm -rf $(SOURCE_PATH)/../light_comx_combo.c
	rm -rf $(SOURCE_PATH)/../tiny_comx_combo.c
	rm -rf $(SOURCE_PATH)/../full_comx_combo.asm
	rm -rf $(SOURCE_PATH)/../light_comx_combo.asm
	rm -rf $(SOURCE_PATH)/../tiny_comx_combo.asm

clean_lib_obj:
	find -type f -name '*.o' -delete

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
	m5_udg

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
	px8
	
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
	z9001_16k_full z9001_32k

vg5k_targets: \
	vg5k_udg vg5k_exp_16k_udg
	
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
	
pc88_targets: \
	pc88
	
super80_targets: \
	super80 super80r
	
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
	
zx81_targets_ijkl_spc: \
	zx81_16k zx81_16k_turn_based zx81_16k_wrx zx81_32k_wrx zx81_32k_wrx_turn_based
  
zx81_targets_qaopm: \
	zx81_16k_qaopm zx81_16k_turn_based_qaopm zx81_16k_wrx_qaopm zx81_32k_wrx_qaopm zx81_32k_wrx_turn_based_qaopm

zx81_targets: \
    zx81_targets_ijkl_spc
	
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
	coleco_udg coleco_adam
	
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
	
excalibur64_targets: \
	excalibur64
	
smc777_targets: \
	smc777
	
pasopia7_targets: \
	pasopia7
	
z80tvgame_targets: \
	z80tvgame
	
qc10_targets: \
	qc10
	    
dmv_targets: \
	dmv
	
bic_targets: \
	bic
	
sv8000_targets: \
	sv8000

tvc_targets: \
	tvc
	
rc700_targets: \
	rc700
	    
myvision_targets: \
	myvision
	
kramermc_targets: \
	kramermc
	
hgmc_targets: \
	hgmc

hemc_targets: \
	hemc
	    
homelab2_targets: \
	homelab2
	
homelab_targets: \
	homelab
	
pencil2_targets: \
	pencil2

m100_targets: \
	m100
	
vector06c_targets: \
	vector06c
	
pmd85_targets: \
	pmd85

gb_targets: \
	gb
	
mikro80_targets: \
	mikro80

primo_targets: \
	primo

##

telestrat_targets: \
	telestrat

supervision_targets: \
	supervision

vic20_targets: \
	vic20_unexpanded vic20_exp_3k vic20_exp_8k vic20_exp_16k

cx16_targets: \
	cx16

atari_targets: \
	atari_color atari_no_color
	
oric_targets: \
	atmos oric1_16k oric1_16k_full

c264_targets: \
	c16_16k_gfx c16_32k
	
pet_targets: \
	pet_8k pet_8k_light pet_16k
	
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
	creativision_8k_tiny creativision_8k_light creativision_16k
	
gamate_targets: \
	gamate
	
atari_lynx_targets: \
	atari_lynx
	
osic1p_targets: \
	osic1p_8k osic1p_32k osic1p_8k_turn_based osic1p_32k_turn_based
	
c128_targets: \
	c128_8502_targets c128_z80_targets


##

ncurses_targets: \
	ncurses ncurses_turn_based
	
####################################################################################################################

# TESTS

FAST_TEST_OPTS ?= \
$(INCLUDE_OPTS) \
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
	-DSOUNDS \
	-DCONIO \
	-clib=default \
	-DNO_INIT_GRAPHICS \
	$(FAST_TEST_OPTS) \
	-Cz--audio -create-app \
	$(TEST_FILES)
	cp a.wav $(BUILD_PATH)/TEST_ace_exp_16k.wav
	rm a.wav
	rm a.tap
	rm a.bin
	
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
	$(FAST_TEST_OPTS) \
	-D__PX4__ \
	-subtype=px4 -Cz--32k \
	-create-app \
	-vn -lndos \
	-DALT_PRINT \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TEST_FILES)
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px4.ROM		
	
    

px8_test:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=px8 \
	$(FAST_TEST_OPTS) \
	-D__PX8__ \
	-create-app \
	-vn -lndos \
	-DALT_PRINT \
	-DZ88DK_SPRITES -DNO_INIT_GRAPHICS \
	$(TEST_FILES) \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c 
	rm a.bin
	mv A.ROM $(BUILD_PATH)/FULL_px8.ROM		
	
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
	 -vn -lndos \
	-DSOUNDS \
	-D__SVI__ \
	-DCONIO \
	-DFORCE_CONIO_LIB \
	-pragma-define:ansicolumns=32 \
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
	-DUSE_MSX_BIOS \
	-DSOUNDS -DANIMATE_PLAYER \
	-vn \
	-lndos \
	-create-app -o $(BUILD_PATH)/FULL_msx_32k.prg \
	-DZ88DK_JOYSTICK \
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
	-pragma-define:ansicolumns=32 \
	-DSOUNDS -DCLIB_ANSI -D__SPECTRUM__ \
	-DNO_INIT_GRAPHICS \
	-DCONIO \
	-lndos -create-app -o $(BUILD_PATH)/TEST_spectrum.prg \
	$(FAST_TEST_OPTS) \
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
	$(Z88DK_PATH)$(MYZ88DK) +zx  -v -zorg=24055 \
	-compiler=sdcc \
	$(FAST_TEST_OPTS) \
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
	-create-app -o $(BUILD_PATH)/TEST_ZSDCC_spectrum.prg \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	$(FAST_TEST_OPTS) \
	-DCONIO \
	$(TEST_FILES)
	rm $(BUILD_PATH)/TEST_ZSDCC_spectrum.prg 
	rm $(BUILD_PATH)/TEST_ZSDCC_spectrum_BANK_7.bin


svi_zsdcc_test:
	$(Z88DK_PATH)$(MYZ88DK) +svi \
	-compiler=sdcc \
	$(FAST_TEST_OPTS) \
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
	-create-app \
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

# 	$(CROSS_LIB_PATH)/memory/oric1.s 
# 	-DUSE_MEMORY_BUFFERS
	
oric1_16k_full: 
	$(CC65_PATH)$(MYCC65) -Or -Cl \
	-DANIMATE_PLAYER -DFULL_GAME \
	-DLESS_TEXT -DNO_SLEEP -DNO_BORDER \
	-DSIMPLE_STRATEGY -DNO_HINTS -DFORCE_GHOSTS_NUMBER=8 \
	-DNO_BLINKING \
	-DNO_RANDOM_LEVEL \
	-t atmos \
	-DMEMORY_MAPPED \
	-DFORCE_NARROW \
	-DUSE_MEMORY_BUFFERS \
	--config $(CFG_PATH)/cc65/atmos_better_tap.cfg \
	$(CROSS_LIB_PATH)/display/init_graphics/cc65/atmos/atmos_redefined_characters_init_graphics.c \
	$(FULL_FILES) \
	$(CROSS_LIB_PATH)/memory/oric1.s \
	-o $(BUILD_PATH)/FULL_oric1_16k_less_text.tap	
	
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

super80:
	$(Z88DK_PATH)$(MYZ88DK) +super80 $(SCCZ80_OPTS) \
	-D__SUPER80__ \
	-DFORCE_XSIZE=32 \
	-DFORCE_YSIZE=16 \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-DCONIO_VT52 \
	-DNO_INIT_GRAPHICS \
	-lndos \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app 
	mv a.ql $(BUILD_PATH)/FULL_super80.ql
	rm a.bin	
	
super80r:
	$(Z88DK_PATH)$(MYZ88DK) +super80 $(SCCZ80_OPTS) -clib=vduem \
	-D__SUPER80R__ \
	-DFORCE_XSIZE=80 \
	-DFORCE_YSIZE=25 \
	-DUDG_GRAPHICS \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO \
	-DCONIO_VT52 \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/udg/udg_init_graphics.c \
	$(CROSS_LIB_PATH)/display/graphics_data/z88dk/xchase.asm \
	chase/end_screen.c \
	$(FULL_FILES) \
	-lndos \
	-create-app
	mv a.ql $(BUILD_PATH)/FULL_super80r.ql
	rm a.bin

	
coleco_adam:
	$(Z88DK_PATH)$(MYZ88DK) +cpm $(SCCZ80_OPTS) -subtype=adam \
	-D__COLECO_ADAM__ \
	-DFULL_GAME  \
	-DEND_SCREEN -DBETWEEN_LEVEL \
	-DCONIO_VT52 \
	-DCONIO \
	-DNO_INIT_GRAPHICS \
	-lndos \
	chase/end_screen.c \
	$(FULL_FILES) \
	-create-app 
	mv a.img $(BUILD_PATH)/FULL_coleco_adam.dsk
	rm a.bin
	
	
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


smc777:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=smc777 $(SCCZ80_OPTS) \
	-D__SMC777__ \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_smc777.bin
	rm $(BUILD_PATH)/FULL_smc777.bin


pencil2:
	$(Z88DK_PATH)$(MYZ88DK) +pencil2 $(SCCZ80_OPTS) \
	-D__PENCIL2__ \
	-DFORCE_XSIZE=32 \
	-DFORCE_YSIZE=24 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_pencil2.bin
	rm $(BUILD_PATH)/FULL_pencil2.bin
	rm $(BUILD_PATH)/FULL_pencil2_BSS.bin
	rm $(BUILD_PATH)/FULL_pencil2_DATA.bin



homelab2:
	$(Z88DK_PATH)$(MYZ88DK) +homelab2 $(SCCZ80_OPTS) \
	-D__HOMELAB2__ \
	-DFORCE_XSIZE=40 \
	-DFORCE_YSIZE=25 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_homelab2.bin
	rm $(BUILD_PATH)/FULL_homelab2.bin


homelab:
	$(Z88DK_PATH)$(MYZ88DK) +homelab $(SCCZ80_OPTS) \
	-D__HOMELAB__ \
	-DFORCE_XSIZE=64 \
	-DFORCE_YSIZE=32 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_homelab.bin
	rm $(BUILD_PATH)/FULL_homelab.bin

hemc:
	$(Z88DK_PATH)$(MYZ88DK) +hemc $(SCCZ80_OPTS) \
	-D__HEMC__ \
	-DFORCE_XSIZE=64 \
	-DFORCE_YSIZE=24 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_hemc.bin
	rm $(BUILD_PATH)/FULL_hemc.bin


hgmc:
	$(Z88DK_PATH)$(MYZ88DK) +hgmc $(SCCZ80_OPTS) \
	-D__HGMC__ \
	-DFORCE_XSIZE=32 \
	-DFORCE_YSIZE=32 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_hgmc.bin
	rm $(BUILD_PATH)/FULL_hgmc.bin


kramermc:
	$(Z88DK_PATH)$(MYZ88DK) +kramermc $(SCCZ80_OPTS) \
	-D__KRAMERMC__ \
	-DFORCE_XSIZE=64 \
	-DFORCE_YSIZE=16 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_kramermc.bin
	rm $(BUILD_PATH)/FULL_kramermc.bin


myvision:
	$(Z88DK_PATH)$(MYZ88DK) +myvision $(SCCZ80_OPTS) \
	-D__MYVISION__ \
	-DFORCE_XSIZE=32 \
	-DFORCE_YSIZE=24 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_myvision.bin
	rm $(BUILD_PATH)/FULL_myvision_BSS.bin
	rm $(BUILD_PATH)/FULL_myvision_DATA.bin


mikro80:
	$(Z88DK_PATH)$(MYZ88DK) +mikro80 $(SCCZ80_OPTS) \
	-D__MIKRO80__ \
	-DFORCE_XSIZE=48 \
	-DFORCE_YSIZE=32 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-v -lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_mikro80.bin
	rm $(BUILD_PATH)/FULL_mikro80.bin


primo:
	$(Z88DK_PATH)$(MYZ88DK) +primo $(SCCZ80_OPTS) \
	-D__PRIMO__ \
	-DFORCE_XSIZE=48 \
	-DFORCE_YSIZE=32 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-v -lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_primo.bin
	rm $(BUILD_PATH)/FULL_primo.bin


pmd85:
	$(Z88DK_PATH)$(MYZ88DK) +pmd85 $(SCCZ80_OPTS) \
	-D__PMD85__ \
	-DFORCE_XSIZE=48 \
	-DFORCE_YSIZE=32 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-v -lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_pmd85.bin
	rm $(BUILD_PATH)/FULL_pmd85.bin

rc700:
	$(Z88DK_PATH)$(MYZ88DK) +cpm -subtype=rc700 $(SCCZ80_OPTS) \
	-D__RC700__ \
	-DFORCE_XSIZE=80 \
	-DFORCE_YSIZE=25 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_rc700.bin
	rm $(BUILD_PATH)/FULL_rc700.bin


gb:
	$(Z88DK_PATH)$(MYZ88DK) +gb $(SCCZ80_OPTS) \
	-D__GB__ \
	-DFORCE_XSIZE=20 \
	-DFORCE_YSIZE=18 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_gb.bin
	rm $(BUILD_PATH)/FULL_gb.bin
	rm $(BUILD_PATH)/FULL_gb_BSS.bin
	rm $(BUILD_PATH)/FULL_gb_DATA.bin
	rm $(BUILD_PATH)/FULL_gb_BANK_1F.bin

m100:
	$(Z88DK_PATH)$(MYZ88DK) +m100 $(SCCZ80_OPTS) \
	-D__M100__ \
	-DFORCE_XSIZE=32 \
	-DFORCE_YSIZE=24 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-v -lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_m100.bin
	rm $(BUILD_PATH)/FULL_m100.bin

tvc:
	$(Z88DK_PATH)$(MYZ88DK) +tvc $(SCCZ80_OPTS) \
	-D__TVC__ \
	-DFORCE_XSIZE=40 \
	-DFORCE_YSIZE=25 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_tvc.bin
	rm $(BUILD_PATH)/FULL_tvc.bin


zxn:
	$(Z88DK_PATH)$(MYZ88DK) +zxn -clib=classic -subtype=nex $(SCCZ80_OPTS) \
	-D__ZXN__ \
	-DFORCE_XSIZE=32 \
	-DFORCE_YSIZE=24 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_zxn.bin
	rm $(BUILD_PATH)/FULL_zxn.bin
	rm $(BUILD_PATH)/FULL_zxn_UNASSIGNED.bin


rc2014:
	$(Z88DK_PATH)$(MYZ88DK) +rc2014  $(SCCZ80_OPTS) \
	-D__ZXN__ \
	-DFORCE_XSIZE=32 \
	-DFORCE_YSIZE=24 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_rc2014.bin


yaz180:
	$(Z88DK_PATH)$(MYZ88DK) +yaz180 $(SCCZ80_OPTS) \
	-D__ZXN__ \
	-DFORCE_XSIZE=32 \
	-DFORCE_YSIZE=24 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_yaz180.bin


vector06c:
	$(Z88DK_PATH)$(MYZ88DK) +vector06c $(SCCZ80_OPTS) \
	-D__VECTOR06C__ \
	-DFORCE_XSIZE=32 \
	-DFORCE_YSIZE=32 \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_vector06c.bin
	rm $(BUILD_PATH)/FULL_vector06c.bin


pasopia7:
	$(Z88DK_PATH)$(MYZ88DK) +pasopia7 $(SCCZ80_OPTS) \
	-D__PASOPIA__ \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_pasopia7.bin
	rm $(BUILD_PATH)/FULL_pasopia7.bin
	rm $(BUILD_PATH)/FULL_pasopia7_BOOTSTRAP.bin	
	
	
z80tvgame: 
	$(Z88DK_PATH)$(MYZ88DK) +z80tvgame $(SCCZ80_OPTS) \
	-D__PASOPIA__ \
	-DNO_SET_SCREEN_COLORS \
	-DNO_COLOR \
	-DFULL_GAME  \
	-DCONIO_VT52 \
	-DCONIO \
	-lndos \
	-DNO_INIT_GRAPHICS \
	$(FULL_FILES) \
	-create-app -o$(BUILD_PATH)/FULL_z80tvgame.bin
	rm $(BUILD_PATH)/FULL_z80tvgame.bin
	rm $(BUILD_PATH)/FULL_z80tvgame_DATA.bin	
	rm $(BUILD_PATH)/FULL_z80tvgame_BSS.bin	

	
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
	$(GCC) -D__NCURSES__ \
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
	$(ZSDCC_OPTS) \
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
	-DDEFAULT_CLEAR_SCREEN \
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
	
    
gamegear_tiny_buffered:
	$(Z88DK_PATH)$(MYZ88DK) +sms -lgamegear -clib=sdcc_iy -startup=17 -v \
	$(ZSDCC_OPTS) \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_sms.inc \
	-DFORCE_XSIZE=20 \
	-DFORCE_YSIZE=16 \
	-D__SMS__ \
	-DNO_COLOR \
	-DTINY_GAME \
	-DSTDLIB \
	-DLESS_TEXT \
	-create-app \
	-DBUFFERED \
	-DTURN_BASED \
	-DALT_PRINT \
	-DDEFAULT_CLEAR_SCREEN \
	-DNO_SLEEP \
	-DNO_WAIT \
	-o $(BUILD_PATH)/TINY_gamegear_buffered.bin \
	$(CROSS_LIB_PATH)/display/init_graphics/z88dk/sms/sms_init_graphics.c \
	$(CROSS_LIB_PATH)/display/alt_print/memory_mapped_alt_print.c \
	$(TINY_FILES)
	rm $(BUILD_PATH)/TINY_gamegear_buffered_CODE.bin
	rm $(BUILD_PATH)/TINY_gamegear_buffered_DATA.bin
	rm $(BUILD_PATH)/TINY_gamegear_buffered_BSS.bin
	rm $(BUILD_PATH)/TINY_gamegear_buffered_UNASSIGNED.bin    

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
	$(SCCZ80_OPTS) \
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
	$(SCCZ80_OPTS) \
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
	
# supervision_test:
	# $(CC65_PATH)$(MYCC65) -t supervision \
	# $(SOURCE_PATH)/../experiments/supervision_test.c \
	# -o $(BUILD_PATH)/supervision_test.sv

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
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-DTINY_GAME \
	-DNO_SLEEP -DLESS_TEXT \
	-D__GAL__ \
	-DALT_MOVE \
	-DNO_RANDOM_LEVEL \
	-DNO_SET_SCREEN_COLORS \
	-DFORCE_BOMBS_NUMBER=4 \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DNO_DEAD_GHOSTS \
	-DNO_INIT_GRAPHICS \
	-DFLAT_ENEMIES \
	-DALT_HIGHSCORE \
	-DCONIO \
	-DUSE_TINY_MEMORY_BUFFERS \
	-v -lndos -create-app -Cz--audio -o  $(BUILD_PATH)/TINY_galaksija_6k.prg \
	$(TINY_FILES) \
	$(CROSS_LIB_PATH)/memory/gal_memory.asm
	rm $(BUILD_PATH)/TINY_galaksija_6k.prg
	#rm $(BUILD_PATH)/TINY_galaksija_6k.wav
		
	
gal_6k_mm:
	$(Z88DK_PATH)$(MYZ88DK) +gal \
	-compiler=sdcc \
	$(ZSDCC_OPTS) \
	-pragma-need=ansiterminal \
	--reserve-regs-iy \
	-pragma-include:$(CFG_PATH)/z88dk/zpragma_clib.inc \
	-DTINY_GAME \
	-DNO_SLEEP -DLESS_TEXT \
	-D__GAL__ \
	-DNO_INITIAL_SCREEN \
	-DNO_CONTROL_INSTRUCTIONS \
	-DALT_MOVE \
	-DNO_RANDOM_LEVEL \
	-DNO_SET_SCREEN_COLORS \
	-DFORCE_BOMBS_NUMBER=4 \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DNO_DEAD_GHOSTS \
	-DNO_INIT_GRAPHICS \
	-DNO_STATS \
	-DNO_MESSAGE \
	-DFLAT_ENEMIES \
	-DMEMORY_MAPPED \
	-DDEFAULT_CLEAR_SCREEN \
	-v -lndos -create-app -Cz--audio -o  $(BUILD_PATH)/TINY_galaksija_6k_mm.prg \
	$(TINY_FILES)	
	
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
	$(CC65_PATH)$(MYCC65) -Or -t pce -Cl \
	--config $(CFG_PATH)cc65/pce_8k_less_stack.cfg \
	-DLESS_TEXT \
	-DNO_MESSAGE -DNO_BLINKING \
	-DNO_CONTROL_INSTRUCTIONS -DNO_INITIAL_SCREEN \
	-DNO_DEAD_GHOSTS -DNO_SET_SCREEN_COLORS -DNO_RANDOM_LEVEL \
	-DFORCE_GHOSTS_NUMBER=8 \
	-DFORCE_NARROW \
	-DCONIO -DNO_INIT_GRAPHICS \
	-DUSE_LIGHT_MEMORY_BUFFERS \
	-DNO_STATS \
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
	
