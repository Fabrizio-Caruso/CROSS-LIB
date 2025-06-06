#

COCO_COLOR_GFX_GAME_OPTS ?= -DMAX_ARROWS_ON_SCREEN=7 -DFASTER_WALL_REDRAW -DNUMBER_OF_MISSILES=3

CREATIVISION_GAME_OPTS ?= -DNUMBER_OF_MISSILES=2 -DMAX_ARROWS_ON_SCREEN=7 -DNO_BOTTOM_WALL_REDRAW

GAMATE_GAME_OPTS ?= -DNUMBER_OF_MISSILES=1 -DMAX_ARROWS_ON_SCREEN=9 -DNO_BOTTOM_WALL_REDRAW -DNO_EXTRA_TITLE

C128_Z80_80COL_GAME_OPTS ?= -DMAX_ARROWS_ON_SCREEN=6 -DNUMBER_OF_MISSILES=2 -DSMALL_WALL -DNO_BOTTOM_WALL_REDRAW -DBULLETS_NUMBER=3
    
ZX81_NO_GFX_GAME_OPTS ?= -DMAX_ARROWS_ON_SCREEN=7 -DNUMBER_OF_MISSILES=4 -DMISSILE_DROP_LOOP_MASK=1 -DSMALL_WALL 

ZX81_GFX_GAME_OPTS ?= -DMAX_ARROWS_ON_SCREEN=6 -DNUMBER_OF_MISSILES=2 -DMISSILE_DROP_LOOP_MASK=7 -DSMALL_WALL -DNO_BOTTOM_WALL_REDRAW

APPLE2_GAME_OPTS ?= -DMAX_ARROWS_ON_SCREEN=7 -DNUMBER_OF_MISSILES=3

COMX_GAME_OPTS ?= 	-DMAX_ARROWS_ON_SCREEN=6 -DNUMBER_OF_MISSILES=3 -DMISSILE_DROP_LOOP_MASK=1 -DSMALL_WALL -DNO_BOTTOM_WALL_REDRAW -DNORMAL_ZOMBIE_SPEED=1 -DSLOW_ZOMBIE_SPEED=7

PECOM_GAME_OPTS ?= 	-DMAX_ARROWS_ON_SCREEN=6 -DNUMBER_OF_MISSILES=3 -DSMALL_WALL -DNO_BOTTOM_WALL_REDRAW -DNORMAL_ZOMBIE_SPEED=1 -DSLOW_ZOMBIE_SPEED=7

TMC600_GAME_OPTS ?= 	-DMAX_ARROWS_ON_SCREEN=6 -DNUMBER_OF_MISSILES=4 -DSMALL_WALL -DNO_BOTTOM_WALL_REDRAW -DNORMAL_ZOMBIE_SPEED=1 -DSLOW_ZOMBIE_SPEED=7

CAMPUTERS_LYNX_GAME_OPTS ?= -DMAX_ARROWS_ON_SCREEN=6 -DNUMBER_OF_MISSILES=3

PV1000_GAME_OPTS  ?=  -DMAX_ARROWS_ON_SCREEN=6 -DNUMBER_OF_MISSILES=3 -DMISSILE_DROP_LOOP_MASK=1 -DSMALL_WALL -DNO_BOTTOM_WALL_REDRAW -DNORMAL_ZOMBIE_SPEED=1 -DSLOW_ZOMBIE_SPEED=7