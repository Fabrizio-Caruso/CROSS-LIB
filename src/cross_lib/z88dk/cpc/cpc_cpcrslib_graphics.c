/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#include <stdio.h>
#include <conio.h>
#include "cpcrslib.h"

#define CPC_WHITE 1
#define CPC_YELLOW 2 
#define CPC_RED 3
#define CPC_BLACK 4

#include "../../display/display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image SKULL_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;

#if defined(FULL_GAME)
	extern Image FREEZE_IMAGE;

	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_POINTS_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	

	extern Image CHASE_IMAGE;
	extern Image SUPER_IMAGE;
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif

#if defined(REDEFINED_CHARS)
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
#endif

#define UDG_N 20

char char_list[UDG_N*2] = 
{ 
33, '\0', // PLAYER
37, '\0', // GHOST
44, '\0', // BOMB
40, '\0', // SKULL
43, '\0', // MISSILE
42, '\0', // POWERUP
41, '\0', // GUN
49, '\0', // EXTRA_POINTS_IMAGE
46, '\0', // INVINCIBILITY
33, '\0', // EXTRA LIFE
39, '\0', // LEFT_MISSILE
38, '\0', // RIGHT_MISSILE
45, '\0', // BUBBLE
33, '\0', // DOWN
34, '\0', // UP
35, '\0', // RIGHT
36, '\0', // LEFT
47, '\0', // VERTICAL_BRICK
48, '\0' // HORIZONTAL_BRICK 
};

// Offsets in the "meta-string" 
#define _PLAYER 0*2
#define _GHOST 1*2
#define _BOMB 2*2
#define _SKULL 3*2
#define _MISSILE 4*2
#define _POWERUP 5*2
#define _GUN 6*2
#define _EXTRA_POINTS 7*2
#define _INVINCIBILITY 8*2
#define _EXTRA_LIFE 9*2
#define _LEFT_MISSILE 10*2
#define _RIGHT_MISSILE 11*2
#define _BUBBLE 12*2
#define _PLAYER_DOWN 13*2
#define _PLAYER_UP 14*2
#define _PLAYER_RIGHT 15*2
#define _PLAYER_LEFT 16*2
#define _VERTICAL_BRICK 17*2
#define _HORIZONTAL_BRICK 18*2

#define _DRAW(color,str,x,y) cpc_PrintGphStrStdXY(color,str,(x+X_OFFSET)*2,(y+Y_OFFSET)*8);

char space_str[2] = {' ', '\0'};

#if !defined(TINY_GAME)
	char *vertical_brick_str;
	char *horizontal_brick_str;
	char *broken_wall_str;
#endif

void set_palette()
{ 
#asm
    EXTERN firmware

	ld bc,$0000
    call firmware
	defw 0xbc38	
 
    ld  a,$00
    ld  bc,$0000
    call firmware
    defw 0xbc32
	
    ld  a,$02
    ld  bc,$1a1a
    call firmware
    defw 0xbc32
	
    ld  a,$03
    ld  bc,$0606
    call firmware
    defw 0xbc32
	
    ld  a,$01
    ld  bc,$1818
    call firmware
    defw 0xbc32
#endasm 
}

void INIT_GRAPHICS(void)
{
	set_palette();
	
	#if !defined(TINY_GAME)
		vertical_brick_str = (char *) char_list + _VERTICAL_BRICK;
		horizontal_brick_str = (char *) char_list + _HORIZONTAL_BRICK;
		broken_wall_str = (char *) char_list + _BOMB;	
	#endif
}

void INIT_IMAGES(void)
{		
	#if !defined(REDEFINED_CHARS)
	PLAYER_IMAGE._imageData = _PLAYER;
	PLAYER_IMAGE._color = CPC_YELLOW;
	#endif
	
	BOMB_IMAGE._imageData = _BOMB;
	BOMB_IMAGE._color = CPC_RED;
	
	GHOST_IMAGE._color = CPC_WHITE;
	GHOST_IMAGE._imageData = _GHOST;
	
	DEAD_GHOST_IMAGE._imageData = _GHOST;	
	DEAD_GHOST_IMAGE._color = CPC_RED;
	
	#if defined(REDEFINED_CHARS)
		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		PLAYER_LEFT._imageData = _PLAYER_LEFT;
		
		PLAYER_DOWN._color = CPC_YELLOW;
		PLAYER_UP._color = CPC_YELLOW;	
		PLAYER_RIGHT._color = CPC_YELLOW;
		PLAYER_LEFT._color = CPC_YELLOW;	
	#endif
	
	#if !defined(TINY_GAME)
		SKULL_IMAGE._color = CPC_YELLOW;
		POWERUP_IMAGE._color = CPC_YELLOW;
		GUN_IMAGE._color = CPC_YELLOW;
			
		SKULL_IMAGE._imageData = _SKULL;

		POWERUP_IMAGE._imageData = _POWERUP;

		GUN_IMAGE._imageData = _GUN;
		MISSILE_IMAGE._imageData = _MISSILE;

		MISSILE_IMAGE._color = CPC_WHITE;
	#endif
	
	#if defined(FULL_GAME)
		FREEZE_IMAGE._color = CPC_WHITE;
		FREEZE_IMAGE._imageData = _POWERUP;
		
		LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_MISSILE;
		LEFT_ENEMY_MISSILE_IMAGE._color = CPC_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_MISSILE;
		RIGHT_ENEMY_MISSILE_IMAGE._color = CPC_WHITE;	
		
		BUBBLE_IMAGE._imageData = _BUBBLE;
		BUBBLE_IMAGE._color = CPC_WHITE;
		
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		#if !defined(REDEFINED_CHARS)
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		#else
		EXTRA_LIFE_IMAGE._imageData = PLAYER_DOWN._imageData;			
		#endif
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;
		CHASE_IMAGE._imageData = _MISSILE;
		SUPER_IMAGE._imageData = _POWERUP;
		CONFUSE_IMAGE._imageData = _SKULL;
		ZOMBIE_IMAGE._imageData = _GHOST;
		
		EXTRA_POINTS_IMAGE._color = CPC_YELLOW;
		EXTRA_LIFE_IMAGE._color = CPC_RED;
		INVINCIBILITY_IMAGE._color = CPC_YELLOW;
		CHASE_IMAGE._color = CPC_RED;
		SUPER_IMAGE._color = CPC_RED;
		CONFUSE_IMAGE._color = CPC_RED;
		ZOMBIE_IMAGE._color = CPC_YELLOW;
	#endif
}

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{		
		_DRAW(CPC_RED,broken_wall_str,x,y);
	}
#endif
	
void _draw(unsigned char x, unsigned char y, Image * image) 
{		
	_DRAW(image->_color,char_list+image->_imageData,x,y);	
}

void _delete(unsigned char x, unsigned char y)
{
	_DRAW(CPC_BLACK,space_str,x,y);	
}

#if !defined(NO_BLINKING)
void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	char str[2];	

	if(*blinkCounter) 
	{	
		_DRAW(image->_color,char_list+image->_imageData,x,y);			
		*blinkCounter=0;
	} 
	else 
	{
		_DRAW(CPC_BLACK,space_str,x,y);			
		*blinkCounter=1;
	}	
}
#endif

#if !defined(TINY_GAME)
	void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	{ 
		unsigned char i;

		for(i=0;i<length;++i)
		{
			_DRAW(CPC_YELLOW,vertical_brick_str,x,y+i);		
		}	
	}

	void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	{
		unsigned char i;

		for(i=0;i<length;++i)
		{
			_DRAW(CPC_YELLOW,horizontal_brick_str,x+i,y);		
		}
	}
#endif



