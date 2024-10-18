#ifndef _INIT_IMAGES_H
#define _INIT_IMAGES_H

#define _PLAYER_DOWN_TILE      _TILE_0
#define _PLAYER_UP_TILE        _TILE_1
#define _PLAYER_RIGHT_TILE     _TILE_2
#define _PLAYER_LEFT_TILE      _TILE_3

#define _GHOST_TILE            _TILE_4
#define _BOMB_TILE             _TILE_5
#define _DEAD_GHOST_TILE       _TILE_6
#define _SKULL_TILE            _TILE_7
#define _POWERUP_TILE          _TILE_8
#define _GUN_TILE              _TILE_9
#define _INVINCIBILITY_TILE    _TILE_10
#define _BULLET_TILE           _TILE_11

#if _XL_NUMBER_OF_TILES<=6
    #define _VERTICAL_BRICK_TILE   _BOMB_TILE
    #define _HORIZONTAL_BRICK_TILE _BOMB_TILE
#else
    #define _VERTICAL_BRICK_TILE   _TILE_12
    #define _HORIZONTAL_BRICK_TILE _TILE_13
#endif
#define _LEFT_MISSILE_TILE     _TILE_14
#define _RIGHT_MISSILE_TILE    _TILE_15
#define _ROCKET_TILE           _TILE_16
#define _FREEZE_TILE           _TILE_17
#define _SUPER_TILE            _TILE_18

#define _EXTRA_POINTS_TILE     _TILE_25

#define _PLAYER_COLOR _XL_CYAN
#define _GHOST_COLOR _XL_WHITE
#define _DEAD_GHOST_COLOR _XL_RED
#define _SKULL_COLOR _XL_YELLOW
#define _BOMB_COLOR _XL_RED
#define _MISSILE_COLOR _XL_WHITE
#define _ROCKET_COLOR _XL_WHITE
#define _POWERUP_COLOR _XL_GREEN
#define _FREEZE_COLOR _XL_CYAN
#define _SUPER_COLOR _XL_RED
#define _GUN_COLOR _XL_CYAN
#define _BULLET_COLOR _XL_WHITE
#define _BRICK_COLOR _XL_YELLOW
#define _EXTRA_POINTS_COLOR _XL_YELLOW
#define _EXTRA_LIFE_COLOR _XL_WHITE
#define _INVINCIBILITY_COLOR _XL_YELLOW
#define _CONFUSE_COLOR _XL_RED
#define _ZOMBIE_COLOR _XL_YELLOW


void INIT_IMAGES(void);




#endif // _INIT_IMAGES_H

