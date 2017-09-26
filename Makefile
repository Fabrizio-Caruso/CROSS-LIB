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
Z88DK_PATH ?= /cygdrive/c/z88dk/bin/
BUILD_PATH ?= build
MYCC65 ?= cl65$(EXEEXT)
MYZ88DK ?= zcc$(EXEEXT)
TOOLS_PATH ?= ./tools


# ------------------------------------------------------------------------------------------
#CC65

atari_color:
	$(CC65_PATH)$(MYCC65) -O -t atari  -DREDEFINED_CHARS -DFULL_GAME -DATARI_MODE1 -DSOUNDS --config $(SOURCE_PATH)/../cfg/atari_mode1_redefined_chars.cfg $(SOURCE_PATH)/atari/atari_sounds.c $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_atari_color.xex

atari_no_color:
	$(CC65_PATH)$(MYCC65) -O -t atari -DFULL_GAME -DSOUNDS $(SOURCE_PATH)/atari/atari_sounds.c $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_atari_no_color.xex

atari_no_color_16k:
	$(CC65_PATH)$(MYCC65) -O -t atari -DSOUNDS $(SOURCE_PATH)/atari/atari_sounds.c $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_atari_no_color.xex

atari5200:
	$(CC65_PATH)$(MYCC65) -O -t atari5200 $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_atari5200.rom


atmos:
	$(CC65_PATH)$(MYCC65)  -O  -DSOUNDS -DREDEFINED_CHARS -DFULL_GAME -t atmos --config $(SOURCE_PATH)/../cfg/atmos_better_tap.cfg $(SOURCE_PATH)/atmos/atmos_input.c  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_atmos_16k.tap

atmos_16k:
	$(CC65_PATH)$(MYCC65) -O  -DSOUNDS -t atmos --config $(SOURCE_PATH)/../cfg/atmos_better_tap.cfg $(SOURCE_PATH)/atmos/atmos_input.c  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_atmos_16k.tap


vic20exp_16k:
	$(CC65_PATH)$(MYCC65) -O -t vic20 -DFULL_GAME -DSOUNDS --config $(SOURCE_PATH)/../cfg/vic20-16k.cfg $(SOURCE_PATH)/vic20/vic20_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_vic20exp_16k.prg

vic20exp_8k:
	$(CC65_PATH)$(MYCC65) -O -t vic20  -DSOUNDS --config $(SOURCE_PATH)/../cfg/vic20-16k.cfg $(SOURCE_PATH)/vic20/vic20_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_vic20exp_8k.prg

c64:
	$(CC65_PATH)$(MYCC65) -O -t c64 -DFULL_GAME -DREDEFINED_CHARS -DSOUNDS --config $(SOURCE_PATH)/../cfg/c64_GFXat0xC000.cfg  $(SOURCE_PATH)/graphics/graphics.s  $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_c64.prg
	$(TOOLS_PATH)/exomizer sfx basic $(BUILD_PATH)/FULL_c64.prg -o $(BUILD_PATH)/FULL_c64_exomized.prg
	rm $(BUILD_PATH)/FULL_c64.prg

c128_40col:
	$(CC65_PATH)$(MYCC65) -O -t c128 -DFULL_GAME -DSOUNDS $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_c128_40col.prg

c128_80col:
	$(CC65_PATH)$(MYCC65) -O -D C128_80COL_VIDEO_MODE -DFULL_GAME -DSOUNDS -t c128 $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_c128_80col.prg


c16_16k:
	$(CC65_PATH)$(MYCC65) -O -t c16 -DSOUNDS --config $(SOURCE_PATH)/../cfg/c16-16k.cfg $(SOURCE_PATH)/c264/c264_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/LIGHT_c16_16k.prg

c16_32k:
	$(CC65_PATH)$(MYCC65) -O -t c16 -DREDEFINED_CHARS -DFULL_GAME -DSOUNDS  $(SOURCE_PATH)/c264/c264_sounds.c $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_c16_32k.prg



pet:
	$(CC65_PATH)$(MYCC65) -O -t pet -DFULL_GAME $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_pet.prg

cbm610:
	$(CC65_PATH)$(MYCC65) -O -t cbm610 -DFULL_GAME $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_cbm610.prg

cbm510:
	$(CC65_PATH)$(MYCC65) -O -t cbm510 -DFULL_GAME -DSOUNDS $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_cbm510.prg


nes:
	$(CC65_PATH)$(MYCC65) -O -t nes -DFULL_GAME $(SOURCE_PATH)/display_macros.c  $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c  -o $(BUILD_PATH)/FULL_nes.nes

# ------------------------------------------------------------------------------------------
#Z88DK

# ace_11k:
	# $(Z88DK_PATH)$(MYZ88DK) +ace -O3 -D__ACE__ -clib=ansi  -Cz--audio -o min -create-app  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	# cp min.wav $(BUILD_PATH)/MINIMAL_ace_experimental.wav
	# rm min.wav
	# rm min.tap
	# rm min.bin
	# rm min
	# rm min_BBS.bin
	# rm min_DATA.bin

# ace_19k:
	# $(Z88DK_PATH)$(MYZ88DK) +ace -O3 -D__ACE__ -DFULL_GAME -clib=ansi -o full -Cz--audio -create-app  $(SOURCE_PATH)/display_macros.c $(SOURCE_PATH)/enemy.c $(SOURCE_PATH)/invincible_enemy.c $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/text.c $(SOURCE_PATH)/missile.c $(SOURCE_PATH)/strategy.c $(SOURCE_PATH)/input_macros.c $(SOURCE_PATH)/main.c
	# cp full.wav $(BUILD_PATH)/FULL_ace_experimental.wav
	# rm full.wav
	# rm full.tap
	# rm full.bin
	# rm full
	# rm full_BBS.bin
	# rm full_DATA.bin

.PHONY: vic20exp_8k vic20exp_16k  atari_color atari_no_color atari_no_color_16k atari5200 atmos atmos_16k c128_40col c128_80col c16_16k c16_32k c64 pet cbm510 cbm610 nes

all: vic20exp_8k vic20exp_16k  atari_color atari_no_color atari_no_color_16k atari5200 atmos atmos_16k c128_40col c128_80col c16_16k c16_32k c64 pet cbm510 cbm610 nes

clean:
	rm -rf $(BUILD_PATH)/*

help:
	cat BUILD.txt
