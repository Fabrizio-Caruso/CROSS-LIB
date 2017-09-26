MYPATH ?= .

CC_PATH ?= /cygdrive/c/cc65-snapshot-win32/bin
DELIVERABLES ?= $(MYPATH)/deliverables
MYCC ?= cl65.exe

vic20exp_16k:
	$(CC_PATH)/$(MYCC) -O -t vic20 -DFULL_GAME -DSOUNDS --config $(MYPATH)/../cfg/vic20-16k.cfg $(MYPATH)/vic20/vic20_sounds.c $(MYPATH)/display_macros.c $(MYPATH)/enemy.c $(MYPATH)/invincible_enemy.c $(MYPATH)/level.c $(MYPATH)/character.c $(MYPATH)/text.c $(MYPATH)/missile.c $(MYPATH)/strategy.c $(MYPATH)/input_macros.c $(MYPATH)/main.c  -o $(DELIVERABLES)/FULL_cvic20-16k_sounds.prg

atari_mode1:
	$(CC_PATH)/$(MYCC) -O -t atari  -DREDEFINED_CHARS -DFULL_GAME -DATARI_MODE1 -DSOUNDS --config $(MYPATH)/../cfg/atari_mode1_redefined_chars.cfg $(MYPATH)/atari/atari_sounds.c $(MYPATH)/atari/atari_input.c $(MYPATH)/display_macros.c  $(MYPATH)/enemy.c $(MYPATH)/invincible_enemy.c $(MYPATH)/level.c $(MYPATH)/character.c $(MYPATH)/text.c $(MYPATH)/missile.c $(MYPATH)/strategy.c $(MYPATH)/input_macros.c $(MYPATH)/main.c  -o $(DELIVERABLES)/FULL_atari_mode1_redefined_chars.xex

	
.PHONY: vic20exp_16k atari_mode1

all: vic20exp_16k atari_mode1



