
#include "8x8_chars.h"

#include "creativision_settings.h"

#include "udg_map.h"

#include "memory_mapped_graphics.h"

const uint8_t player_down_image[8] = _PLAYER_DOWN_UDG;
const uint8_t player_up_image[8] = _PLAYER_UP_UDG;
const uint8_t player_left_image[8] = _PLAYER_LEFT_UDG;
const uint8_t player_right_image[8] = _PLAYER_RIGHT_UDG;
const uint8_t skull_image[8] = _SKULL_UDG;
const uint8_t ghost_image[8] = _GHOST_UDG;
const uint8_t bomb_image[8] = _BOMB_UDG;
const uint8_t gun_image[8] = _GUN_UDG;
const uint8_t powerup_image[8] = _POWERUP_UDG;

const uint8_t super_image[8] = _SUPER_UDG;
const uint8_t extra_life_image[8] = _EXTRA_LIFE_UDG;
const uint8_t bullet_image[8] = _BULLET_UDG;
const uint8_t freeze_image[8] = _FREEZE_UDG;
const uint8_t invincibility_image[8] = _INVINCIBILITY_UDG;

const uint8_t dead_ghost_image[8] = _DEAD_GHOST_UDG;
const uint8_t rocket_image[8] = _ROCKET_UDG;
const uint8_t left_horizontal_missile_image[8] = _LEFT_HORIZONTAL_MISSILE_UDG;
const uint8_t right_horizontal_missile_image[8] = _RIGHT_HORIZONTAL_MISSILE_UDG;

const uint8_t extra_points_image[8] = _EXTRA_POINTS_UDG;

void set_group_color(uint8_t group, uint8_t color)
{
	DISPLAY_POKE((uint16_t) COLOR_DEF + (uint16_t) group, ((uint16_t) color)<<4);
}

void set_udg_colors(void)
{
	uint8_t i;
	
	set_group_color(0,9);
	set_group_color(1,7);
	set_group_color(2,9);
	set_group_color(3,11);
	set_group_color(4,10);
	set_group_color(5,2);
	set_group_color(6,4);
	set_group_color(7,4);
	
	for(i=8;i<=11;++i)
	{
		set_group_color(i,8);
	}
}

void redefine(const uint8_t ch, const uint8_t* image) 
{ 
    uint8_t i; 
    
    for(i=0;i<8;++i) 
    { 
        DISPLAY_POKE(CHAR_BASE +(uint16_t)(ch<<3)+i,image[i]); 
    } 
} 


void SET_UDG_IMAGES(void) 
{ 
	uint8_t i;
    
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) 
	{ 
	   redefine(redefine_map[i].ascii, redefine_map[i].bitmap); 
	} 
}

#include <conio.h>
void INIT_GRAPHICS(void)
{
    SET_UDG_IMAGES();

    
    set_udg_colors();  
}
