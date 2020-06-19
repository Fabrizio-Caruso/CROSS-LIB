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


SOURCE_PATH := games/chase
CROSS_LIB_PATH := cross_lib


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

#####################################################################
# Common includes and targets
include makefiles/auxiliary/Makefile_common


# Draw skip
LIGHT_DRAW_SKIP ?= -DBOMB_DRAW_SKIP=1 -DWALL_DRAW_SKIP=3
DRAW_SKIP ?=       -DBOMB_DRAW_SKIP=1 -DWALL_DRAW_SKIP=7
EXTRA_DRAW_SKIP ?= -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15
ULTRA_DRAW_SKIP ?= -DBOMB_DRAW_SKIP=7 -DWALL_DRAW_SKIP=15 -DGHOST_DRAW_SKIP=1 



#
BASIC_CROSS_LIB_FILES ?= \
	$(CROSS_LIB_PATH)/display/display_macros.c \
	$(CROSS_LIB_PATH)/input/input_macros.c \
	$(CROSS_LIB_PATH)/text/text_macros.c

NO_CHASE_FILES ?= \
	$(BASIC_CROSS_LIB_FILES) \
	$(SOURCE_PATH)/move_player.c \
	$(SOURCE_PATH)/ghost.c  $(SOURCE_PATH)/level.c $(SOURCE_PATH)/character.c $(SOURCE_PATH)/game_text.c \
	$(SOURCE_PATH)/main.c \
	$(SOURCE_PATH)/init_images.c

#
TINY_FILES ?= $(NO_CHASE_FILES) $(SOURCE_PATH)/strategy.c

TEST_FILES ?= $(NO_CHASE_FILES)

LIGHT_ONLY_FILES ?= $(SOURCE_PATH)/item.c $(SOURCE_PATH)/skull.c $(SOURCE_PATH)/bullet.c 
FULL_ONLY_FILES ?= $(SOURCE_PATH)/horizontal_missile.c $(SOURCE_PATH)/rocket.c 

LIGHT_FILES ?= $(TINY_FILES) $(LIGHT_ONLY_FILES) 

FULL_FILES ?= $(LIGHT_FILES) $(FULL_ONLY_FILES)


#############################################################################################################################################
# Auxiliary targets

# - Combine source files
include makefiles/auxiliary/Makefile_combo


#############################################################################################################################################

# All vintage and GCC targets that can be compiled under CYGWIN, i.e., not including gcc_z8k_targets and not including emcc_targets
all: \
    cc65_targets \
    cmoc_targets \
    ack_targets \
    lcc1802_targets \
    xtc68_targets \
    gcc_targets \
    gcc_atari_st_targets \
    gcc_amiga_targets \
    z88dk_targets



####################################################################################################################
# TESTS


include makefiles/tests/Makefile.tests


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



#############################################################################################################################################
# Cross dev-kit 
#include makefiles/targets/Makefile.cross-dev-kit_targets


####################################################################################################################
# Alternative builds	

# - Z88DK
include makefiles/alt/Makefile.z88dk_alternative_builds

# - CC65
include makefiles/alt/Makefile.cc65_alternative_builds

####################################################################################################################
# WIP

# - Z88DK
include makefiles/wip/Makefile.z88dk_wip_targets

# - CC65
include makefiles/wip/Makefile.cc65_wip_targets

# - ACK
include makefiles/wip/Makefile.ack_wip_targets

####################################################################################################################
# EXPERIMENTAL

include makefiles/experimental/Makefile.z88dk_experimental_targets


####################################################################################################################
# DEBUG	

# - Z88DK
#include makefiles/debug/Makefile.cc65_debug

# - CC65
#include makefiles/debug/Makefile.z88dk_debug

# - LCC1802
include makefiles/debug/Makefile.lcc1802_debug

