#

# COCO_COLOR_GFX_GAME_OPTS ?= -DMAX_ARROWS_ON_SCREEN=7 -DFASTER_WALL_REDRAW -DNUMBER_OF_MISSILES=3

CREATIVISION_GAME_OPTS ?= -DLESS_TEXT 

GAMATE_GAME_OPTS ?= -DLESS_TEXT -DNO_BLINKING -DSIMPLE_STRATEGY -DNO_HINTS -DNO_CONTROL_INSTRUCTIONS

C128_Z80_80COL_GAME_OPTS ?= -DLESS_TEXT -DNO_BLINKING -DNO_HINTS -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15
    
# ZX81_NO_GFX_GAME_OPTS ?= -DMAX_ARROWS_ON_SCREEN=7 -DNUMBER_OF_MISSILES=4 -DMISSILE_DROP_LOOP_MASK=1 -DSMALL_WALL 

ZX81_GFX_GAME_OPTS ?= -DLESS_TEXT -DNO_BLINKING -DNO_HINTS -DBOMB_DRAW_SKIP=7 -DWALL_DRAW_SKIP=15

# APPLE2_GAME_OPTS ?= -DMAX_ARROWS_ON_SCREEN=7 -DNUMBER_OF_MISSILES=3

COMX_GAME_OPTS ?= -DNO_BLINKING -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15 -DGHOSTS_NUMBER=7

PECOM_GAME_OPTS ?= -DNO_BLINKING -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15 -DGHOSTS_NUMBER=7

TMC600_GAME_OPTS ?= -DNO_BLINKING -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15 -DGHOSTS_NUMBER=7

MICRO_GAME_OPTS ?= -DNO_BLINKING -DGHOSTS_NUMBER=7

CAMPUTERS_LYNX_GAME_OPTS ?= -DNO_BLINKING -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15 -DGHOSTS_NUMBER=7

VIC20_EXP_3K_GAME_OPTS ?= -DNO_BLINKING -DNO_HINTS -DLESS_TEXT -DGHOSTS_NUMBER=8

VIC20_UNEXPANDED_GAME_OPTS ?= -DNO_EXTRA_TITLE 	\
	-DROUND_ENEMIES \
	-DSIMPLE_RANDOM_LEVEL \
	-DNO_END_SCREEN \
	-DNO_CONTROL_INSTRUCTIONS \
	-DLESS_TEXT -DNO_BLINKING \
	-DNO_STATS \
	-DGHOSTS_NUMBER=8 \
	-DNO_DEAD_GHOSTS

STDIO_GAME_OPTS ?= -DEVEN_LOOP_MOVE

PV1000_GAME_OPTS ?=  -DNO_BLINKING -DBOMB_DRAW_SKIP=3 -DWALL_DRAW_SKIP=15 -DGHOSTS_NUMBER=7