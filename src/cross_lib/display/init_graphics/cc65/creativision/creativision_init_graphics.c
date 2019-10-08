
#include "8x8_chars.h"

#include "creativision_settings.h"

#include "udg_map.h"

#include "memory_mapped_graphics.h"

const unsigned char player_down_image[8] = _PLAYER_DOWN_UDG;
const unsigned char player_up_image[8] = _PLAYER_UP_UDG;
const unsigned char player_left_image[8] = _PLAYER_LEFT_UDG;
const unsigned char player_right_image[8] = _PLAYER_RIGHT_UDG;
const unsigned char skull_image[8] = _SKULL_UDG;
const unsigned char ghost_image[8] = _GHOST_UDG;
const unsigned char bomb_image[8] = _BOMB_UDG;
const unsigned char gun_image[8] = _GUN_UDG;
const unsigned char powerup_image[8] = _POWERUP_UDG;

const unsigned char super_image[8] = _SUPER_UDG;
const unsigned char extra_life_image[8] = _EXTRA_LIFE_UDG;
const unsigned char bullet_image[8] = _BULLET_UDG;
const unsigned char freeze_image[8] = _FREEZE_UDG;
const unsigned char invincibility_image[8] = _INVINCIBILITY_UDG;

const unsigned char dead_ghost_image[8] = _DEAD_GHOST_UDG;
const unsigned char rocket_image[8] = _ROCKET_UDG;
const unsigned char left_horizontal_missile_image[8] = _LEFT_HORIZONTAL_MISSILE_UDG;
const unsigned char right_horizontal_missile_image[8] = _RIGHT_HORIZONTAL_MISSILE_UDG;

const unsigned char extra_points_image[8] = _EXTRA_POINTS_UDG;

void set_group_color(unsigned char group, unsigned char color)
{
	DISPLAY_POKE((unsigned short) COLOR_DEF + (unsigned short) group, ((unsigned short) color)<<4);
}

void set_udg_colors(void)
{
	unsigned char i;
	
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

void redefine(const unsigned char ch, const unsigned char* image) 
{ 
    unsigned char i; 
    
    for(i=0;i<8;++i) 
    { 
        DISPLAY_POKE(CHAR_BASE +(unsigned short)(ch<<3)+i,image[i]); 
    } 
} 

// #include <conio.h>
// #define redefine(offset, data) \
// { \
    // unsigned short i; \
    // \
    // for(i=0;i<8;++i) \
    // { \
        // gotoxy(1,0); \
        // cprintf("offset %d ", offset); \
        // gotoxy(1,i+1); \
        // cprintf("i: %d  addr: %d <- %d ", i, (offset)*8,  (data)[i]); \
        // DISPLAY_POKE(CHAR_BASE+(unsigned short)((offset)*8)+i,(data)[i]); \
    // } \
    // clrscr(); \
// } \

#include<conio.h>
void SET_UDG_IMAGES(void) 
{ 
	unsigned char i;
    
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) 
	{ 
       gotoxy(1,15); cprintf("i: %d - %d <- %d %d", i, redefine_map[i].ascii, redefine_map[i].bitmap[0], redefine_map[i].bitmap[1]); 
	   redefine(redefine_map[i].ascii, redefine_map[i].bitmap); 
	} 
}

#include <conio.h>
void INIT_GRAPHICS(void)
{
    SET_UDG_IMAGES();
    // redefine(_PLAYER_RIGHT,player_right_image);
    // redefine(_PLAYER_LEFT,player_left_image);
    // redefine(_PLAYER_UP,player_up_image);
    // redefine(_PLAYER_DOWN,player_down_image);
    // redefine(_SKULL,skull_image);
    // redefine(_GUN,gun_image);
    // redefine(_POWERUP,powerup_image);
    // redefine(_BOMB,bomb_image);
    // redefine(_GHOST,ghost_image);
    // redefine(_ROCKET,rocket_image);
    // redefine(_LEFT_HORIZONTAL_MISSILE,left_horizontal_missile_image);
    // redefine(_RIGHT_HORIZONTAL_MISSILE,right_horizontal_missile_image);
    // redefine(_BULLET,bullet_image); 
    // redefine(_DEAD_GHOST,dead_ghost_image);    
    // redefine(_EXTRA_POINTS,extra_points_image);
    
    // REDEFINE_CHAR(59, player_down_image);
    // gotoxy(1,12);
    // cprintf("debug 1: %d", _PLAYER_DOWN);
    // cprintf("debug 1 off: %d", _PLAYER_DOWN_OFFSET_); 
    
    set_udg_colors();  
    // while(1){};
}