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
-I$(CROSS_LIB_PATH)/input \
-I$(CROSS_LIB_PATH)/rand

LIGHT_DRAW_SKIP ?= -DBOMB_DRAW_SKIP=1 -DWALL_DRAW_SKIP=3
DRAW_SKIP ?= -DBOMB_DRAW_SKIP=1 -DWALL_DRAW_SKIP=7
EXTRA_DRAW_SKIP ?= -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15
ULTRA_DRAW_SKIP ?= -DGHOST_DRAW_SKIP=1 -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15


XTC68 ?= qcc
XTC68_OPTS = -D__XTC68__ -I$(SOURCE_PATH) \


MYCC65 ?= cl65$(EXEEXT)
MYZ88DK ?= zcc$(EXEEXT)
MYZ88DKASM ?= z80asm$(EXEEXT)

SCCZ80_OPTS ?= 
#-O3

ZSDCC_OPTS ?= 
# -SO3 --max-allocs-per-node200000

Z8K-PCOS-GCC ?= /opt/z8kgcc-jan-19-2009/bin/z8k-pcos-gcc
Z8K-PCOS-GCC_OPTS ?= -Wno-cpg 

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
_CC_OPTS ?= 

TI99_CC ?= tms9900-gcc
TI99_OPTS ?= -nostdlib -DNO_RANDOM_LEVEL

GCC ?= gcc
GCC_OPTS ?= -Wpedantic -Wall

ATARI_ST_CC ?= m68k-atari-mint-gcc
ATARI_ST_OPTS ?= 

AMIGA_CC ?= m68k-amigaos-gcc
AMIGA_OPTS ?= 

EM_CC ?= emcc
EM_OPTS = 

LCC1802_PATH ?= /cygdrive/c/lcc42/bin/
LCC1802_EXE ?= lcc$(EXEEXT)
LCC1802 ?= $(LCC1802_PATH)$(LCC1802_EXE)
#/cygdrive/c/lcc42/bat/caCX.bat
#/cygdrive/c/lcc42/bin/lcc.exe
LCC1802_OPTS ?= -target=xr18CX "-Wf-volatile" -O "-Wa-D LCCCX=1"
COMX_OPTS ?= "-Wa-D CODELOC=0x4400"
PECOM_OPTS ?= "-Wa-D CODELOC=0x200"
#-target=xr18CX -O "-Wf-volatile" "-Wa-D CODELOC=0x4400" "-Wa-D LCCCX=1"
#-Wf-volatile -target=xr18CX

COCO_OPTS_TINY  ?= -O0 -Wno-const  -D__COCO__ -DTINY_GAME 

COCO_OPTS_LIGHT ?= -O0 -Wno-const -D__COCO__

CMOC_OPTS       ?= -O0 -Wno-const  -DDEFAULT_CLEAR_SCREEN -D__CMOC__  -DMEMORY_MAPPED -DNO_INIT_GRAPHICS -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL

COCO_OPTS       ?= -D__COCO__ $(CMOC_OPTS)

DRAGON_OPTS     ?= --dragon -D__DRAGON__ $(COCO_OPTS) 

VECTREX_OPTS    ?= -O0 --vectrex  -DNO_SLEEP -DNO_WAIT -DNO_INPUT -DNO_PRINT -Wno-const -DDEFAULT_CLEAR_SCREEN -D__CMOC__ -D__VECTREX__ -DVECTORIAL -DNO_INIT_GRAPHICS -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL

MO5_OPTS        ?= -O2 -DANIMATE_PLAYER -DFORCE_NO_CONIO_LIB -DNO_SLEEP -DNO_WAIT -Wno-const -DDEFAULT_CLEAR_SCREEN -D__CMOC__ -D__MO5__ -DBIT_MAPPED -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL

TO7_OPTS        ?= -O1 -DFORCE_NO_CONIO_LIB -DNO_INPUT -DNO_SLEEP -DNO_WAIT -DNO_PRINT -Wno-const -DDEFAULT_CLEAR_SCREEN -D__CMOC__ -D__TO7__ -DBIT_MAPPED -DALT_PRINT -DLESS_TEXT -DFULL_GAME -DEND_SCREEN -DBETWEEN_LEVEL

_ACK ?= ack
_ACK_OPTS ?= -O6 
# -O 
# -ansi


# ------------------------------------------------------------------------------------------
# Combine all .c source files into one for cross-compilers that only handle single files, e.g., LCC1802, WINCMOC

	
tiny_combo.cat:
	cat \
	$(TINY_FILES) \
	> $(SOURCE_PATH)/../tiny_combo.cat


light_combo.cat: tiny_combo.cat
	cat \
	$(SOURCE_PATH)/../tiny_combo.cat \
	$(LIGHT_ONLY_FILES) \
	> $(SOURCE_PATH)/../light_combo.cat
	rm -rf tiny_combo.cat


full_combo.cat: light_combo.cat
	cat \
	$(SOURCE_PATH)/../light_combo.cat \
	$(FULL_ONLY_FILES) \
	> $(SOURCE_PATH)/../full_combo.cat
	rm -rf light_combo.cat


tiny_combo_rand.cat: tiny_combo.cat
	cat \
	$(CROSS_LIB_PATH)/rand/rand.c \
	$(SOURCE_PATH)/../tiny_combo.cat \
	> $(SOURCE_PATH)/../tiny_combo_rand.cat
	rm -rf tiny_combo.cat
    
light_combo_rand.cat: light_combo.cat
	cat \
	$(CROSS_LIB_PATH)/rand/rand.c \
	$(SOURCE_PATH)/../light_combo.cat \
	> $(SOURCE_PATH)/../light_combo_rand.cat
	rm -rf light_combo.cat
    
    
full_combo_rand.cat: full_combo.cat
	cat \
	$(CROSS_LIB_PATH)/rand/rand.c \
	$(SOURCE_PATH)/../full_combo.cat \
	> $(SOURCE_PATH)/../full_combo_rand.cat
	rm -rf full_combo.cat




.PHONY: mtx vic20exp_8k vic20exp_16k  atari_color atari_no_color atari_no_color_16k atari5200 atmos c128_40col c128_80col c16_16k c16_32k c64 pet cbm510 cbm610 nes apple2 apple2enh

# xtc86 targets
sinclair_ql_targets: \
	sinclair_ql_tiny_buffered

xtc68_targets: sinclair_ql_targets

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

# broken : 	

#OK

z88dk_targets_a-c: \
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
	cpm_z80_targets

z88dk_targets_d-k: \
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
	kramermc_targets

z88dk_targets_l-m: \
	lambda_targets \
	laser500_targets \
	m5_targets \
	mc1000_targets \
	microbee_targets \
	mikro80_targets \
	msx_targets \
	mtx_targets \
	multi8_targets \
	myvision_targets \
	mz_targets \
	mz2500_targets

z88dk_targets_n-p: \
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
	px8_targets

z88dk_targets_q-s: \
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
	svi_targets 

z88dk_targets_t-v: \
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
	vz200_targets 

z88dk_targets_w-z: \
	x1_targets \
	z1013_targets \
	z80tvgame_targets \
	z88_targets \
	z9001_targets \
	zx80_targets \
	zx81_targets


# missing: abc800, xo7, c7420, rex
# missing 8080: m100
z88dk_targets:
	make z88dk_targets_a-c \
	make z88dk_targets_d-k \
	make z88dk_targets_l-m \
	make z88dk_targets_o-p \
	make z88dk_targets_q-s \
	make z88dk_targets_t-v \
	make z88dk_targets_w-z


cmoc_targets: \
	coco_targets dragon_targets mo5_targets
	
        
ack_targets: \
    pdp11v7_targets \
    cpm_8080_targets \
    pc8086_targets \
    linux386_targets \
    linux68k_targets \
    linuxppc_targets \
    linuxmips_targets
	
atari_st_targets: \
	atari_st_buffered atari_st_ncurses

gcc_atari_st_targets: \
    atari_st_targets

amiga_targets: \
	amiga_buffered

gcc_amiga_targets: \
    amiga_targets

gcc_targets: \
	gcc_buffered ncurses_targets  

lcc1802_targets: \
	comx_targets pecom_targets

# Currently to be compiled in Linux (because the source code of the compiler won't easily compile)
olivetti_m20_targets: \
	olivetti_m20_buffered
    
gcc_z8k_targets: \
    olivetti_m20_targets

emcc_targets: \
    emcc_buffered

# Cross dev-kit 
c128_targets: \
	c128_8502_targets c128_z80_targets
    
cpm_targets: \
    cpm_8080_targets cpm_z80_targets

# All vintage and GCC targets that can be compiled under CYGWIN, i.e., not including gcc_z8k_targets and not including emcc_targets
all: cc65_targets z88dk_targets cmoc_targets ack_targets xtc68_targets gcc_targets gcc_atari_st_targets gcc_amiga_targets lcc1802_targets

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
	rm -rf $(SOURCE_PATH)/../full_pecom_combo.c
	rm -rf $(SOURCE_PATH)/../light_pecom_combo.c
	rm -rf $(SOURCE_PATH)/../tiny_pecom_combo.c
	rm -rf $(SOURCE_PATH)/../full_pecom_combo.asm
	rm -rf $(SOURCE_PATH)/../light_pecom_combo.asm
	rm -rf $(SOURCE_PATH)/../tiny_pecom_combo.asm

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
	cat ../docs/BUILD.md

list:
	cat ../docs/SYSTEMS.md


simple_test: gcc_buffered


test: simple_test z88dk_test


####################################################################################################################
# TARGETS PER DEV-KIT

# ------------------------------------------------------------------------------------------
# - CC65 

include makefiles/targets/Makefile.cc65_targets

# ------------------------------------------------------------------------------------------
# - Z88DK 

include makefiles/targets/Makefile.z88dk_targets

# ------------------------------------------------------------------------------------------
# - CMOC

include makefiles/targets/Makefile.cmoc_targets

# ------------------------------------------------------------------------------------------
# - ACK

include makefiles/targets/Makefile.ack_targets

# ------------------------------------------------------------------------------------------
# - GCC

include makefiles/targets/Makefile.gcc_targets

# ------------------------------------------------------------------------------------------
# - GCC AMIGA

include makefiles/targets/Makefile.gcc_amiga_targets

# ------------------------------------------------------------------------------------------
# - GCC ATARI_ST

include makefiles/targets/Makefile.gcc_atari_st_targets

# ------------------------------------------------------------------------------------------
# - XTC68

include makefiles/targets/Makefile.xtc68_targets

# ------------------------------------------------------------------------------------------
# - LCC1802

include makefiles/targets/Makefile.lcc1802_targets

# ------------------------------------------------------------------------------------------
# - GCC Z8K PCOS

include makefiles/targets/Makefile.gcc_z8k_pcos_targets


# ------------------------------------------------------------------------------------------
# - EMCC

include makefiles/targets/Makefile.emcc_targets


# ------------------------------------------------------------------------------------------
# - GCC TMS9900

include makefiles/targets/Makefile.gcc_tms9900_targets

# ------------------------------------------------------------------------------------------
# - SDCC

include makefiles/targets/Makefile.sdcc_targets


# ------------------------------------------------------------------------------------------
# - WINCMOC
#

#include makefiles/targets/Makefile.wincmoc_targets


####################################################################################################################
# Compilation Tests

# - Z88DK
include makefiles/tests/Makefile.z88dk_tests

####################################################################################################################
	
# Alternative builds	

# - Z88DK
#include makefiles/alt/Makefile.z88dk_alternative_builds

# - CC65
#include makefiles/alt/Makefile.cc65_alternative_builds

####################################################################################################################
	
# DEBUG	

# - Z88DK
#include makefiles/debug/Makefile.cc65_debug

# - CC65
#include makefiles/debug/Makefile.z88dk_debug

# - LCC1802
include makefiles/debug/Makefile.lcc1802_debug

