
#include <msx/gfx.h>

#define CHAR_BASE 0x0100
#define COLOR_DEF 0x2000

#include "../../../image_settings/msx_redefined_chars_settings.h"

void redefine(unsigned short offset, const char *new_char)
{
	unsigned char i;
	
	for(i=0;i<8;++i)
	{
		msx_vpoke(CHAR_BASE+(offset<<3)+i-32*8,new_char[i]);
	}
}

void set_group_color(unsigned char group, unsigned char color)
{
	msx_vpoke(COLOR_DEF+group, color<<4);
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

void set_udg_images(void)
{
	static const char player_down[8] =      { 24, 36, 24,102,153, 24, 36,102};
	static const char player_up[8] =        { 24, 60, 24,102,153, 24, 36,102};
	static const char player_right[8] =     { 24, 52, 25,118,152, 24, 20, 20};	
	static const char player_left[8] =      { 24, 44,152,110, 25, 24, 40, 40};
	static const char ghost[8] =            {129,126,165,129,129,189,129,126};
	static const char invincible_ghost[8] = { 60, 66,165,129, 90, 36, 36, 60};
	static const char gun[8] =              {  0,128,126,200,248,192,128,  0};
	static const char powerUp[8] =          {  0, 60, 54,223,231,122, 36, 24};
	static const char missile[8] =          {  0,  0,  8, 56, 28, 16,  0,  0};
	static const char bomb[8] =             { 60, 66,165,153,153,165, 66, 60};
	
	static const char bubble[8] =           { 24, 60, 60, 60,126, 90, 66, 66};	
	static const char missile_right[8] =    {  0,  0, 15,252,252, 15,  0,  0};
	static const char missile_left[8] =     {  0,  0,240, 63, 63,240,  0,  0};	
	static const char invincibility[8] =    { 24, 36, 24,  0,153,  0, 36,102};
	
	static const char dead_ghost[8] =       {129,126,165,129,129,189,129,126};
	
	static const char vertical_brick[8] =   { 24, 24, 24, 48, 24, 12, 24, 24};
	static const char horizontal_brick[8] = {  0,  0,  0,255,  0,  0,  0,  0};	
	static const char extra_points[8]     = { 16, 62, 32, 60,  4,124,  8,  0};
		
	redefine(_PLAYER,player_down);
	redefine(_PLAYER_DOWN,player_down);
	redefine(_PLAYER_UP,player_up);	
	redefine(_PLAYER_LEFT,player_left);	
	redefine(_PLAYER_RIGHT, player_right);	
		
	redefine(_BOMB, bomb);
	redefine(_GUN, gun);
	redefine(_POWERUP, powerUp);		
	redefine(_SUPER, powerUp);
		
	redefine(_GHOST, ghost);
	redefine(_ZOMBIE, ghost);
	redefine(_SKULL, invincible_ghost);	
	redefine(_BULLET, missile);	
		
	redefine(_EXTRA_LIFE,player_down);	
	redefine(_INVINCIBILITY,invincibility);	
	redefine(_DEAD_GHOST,dead_ghost);	
		
	redefine(_VERTICAL_BRICK,vertical_brick);	
	redefine(_HORIZONTAL_BRICK,horizontal_brick);		
		
	redefine(_FREEZE, powerUp);
		
	redefine(_LEFT_HORIZONTAL_MISSILE,missile_left);	
	redefine(_RIGHT_HORIZONTAL_MISSILE,missile_right);	
	redefine(_ROCKET, bubble);
	redefine(_EXTRA_POINTS, extra_points);
	
}

void INIT_GRAPHICS(void)
{
	set_color(15, 1, 1);	
	set_mode(mode_1);
	
	set_udg_colors();
	set_udg_images();
}
