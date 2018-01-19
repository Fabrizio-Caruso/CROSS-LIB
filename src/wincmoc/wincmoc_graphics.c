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

	
// BLUE
#define _PLAYER 0 
#define _PLAYER_DOWN 0
#define _PLAYER_UP 1
#define _PLAYER_RIGHT 2
#define _PLAYER_LEFT 3

#define _GUN 4

// YELLOW
#define _INVINCIBLE_GHOST 5
#define _VERTICAL_BRICK 6
#define _HORIZONTAL_BRICK 7
#define _EXTRA_LIFE 8
#define _EXTRA_POINTS 9

// GREEN
#define _POWERUP  10

// RED
#define _BOMB 11
#define _DEAD_GHOST 12


#define _INVINCIBILITY 13
#define _MISSILE 14


#define _GHOST 15

#define _LEFT_ENEMY_MISSILE 16
#define _RIGHT_ENEMY_MISSILE 17
#define _BUBBLE 18

#include "display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_POINTS_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	
	extern Image BROKEN_WALL_IMAGE;
	
	extern Image FREEZE_IMAGE;
	extern Image CHASE_IMAGE;
	extern Image SUPER_IMAGE;
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
#endif

#define BASE_ADDR

Image PLAYER_DOWN;
Image PLAYER_UP;
Image PLAYER_RIGHT;
Image PLAYER_LEFT;



#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))

#define _DRAW(x,y,image) \
{ \
	unsigned char i; \
	for (i=0;i<8;++i) \
	{ \
		POKE(BASE_ADDR+x+X_OFFSET+(y+i+Y_OFFSET)*XSize,char_map[image].bitmap[i]); \
	} \
}

#define _DELETE(x,y) \
{ \
	unsigned char i; \
	for (i=0;i<8;++i) \
	{ \
		POKE(BASE_ADDR+x+X_OFFSET+(y+i+Y_OFFSET)*XSize,0); \
	} \
}


#define _DRAW_VERTICAL_WALL(x,y)  
#define _DRAW_HORIZONTAL_WALL(x,y)  
#define _DRAW_BROKEN_WALL(x,y)  	


struct redefine_struct
{
   unsigned char index;
   unsigned char bitmap[8];
} ;

struct redefine_struct char_map[] =
{
	{_PLAYER_DOWN, { 24, 36, 24,102,153, 24, 36,102}},
	{_PLAYER_UP, { 24, 60, 24,102,153, 24, 36,102}},
	{_PLAYER_RIGHT, { 24, 52, 25,118,152, 24, 20, 20}},	
	{_PLAYER_LEFT, { 24, 44,152,110, 25, 24, 40, 40}},
	{_GHOST, {129,126,165,129,129,189,129,126}},
	{_RIGHT_ENEMY_MISSILE, {  0,  0, 15,252,252, 15,  0,  0}},
	{_LEFT_ENEMY_MISSILE, {  0,  0,240, 63, 63,240,  0,  0}},
	{_INVINCIBLE_GHOST, { 60, 66,165,129, 90, 36, 36, 60}},
	{_GUN, {  0,128,126,200,248,192,128,  0}},
	{_POWERUP, {  0, 60, 54,223,231,122, 36, 24}},
	{_MISSILE, {  0,  0,  8, 56, 28, 16,  0,  0}},
	{_BOMB, { 60, 66,165,153,153,165, 66, 60}},
	{_BUBBLE, { 24, 60, 60, 60,126, 90, 66, 66}},
	{_INVINCIBILITY, { 24, 36, 24,  0,153,  0, 36,102}},
	{_VERTICAL_BRICK, { 24, 24, 24, 48, 24, 12, 24, 24}},
	{_HORIZONTAL_BRICK, {  0,  0,  0,255,  0,  0,  0,  0}},

// BOGUS
	{_EXTRA_LIFE, { 24, 36, 24,102,153, 24, 36,102}},
	{_EXTRA_POINTS, { 24, 24, 24, 48, 24, 12, 24, 24}}
};


void INIT_GRAPHICS(void)
{


/*
10 poke &hff22,&he0
20 poke &hffc3,1
30 poke &hffc5,1
40 goto 40
*/	
	POKE(0xFF22,0xF8);
	POKE(0xFFC3,0x01);
	POKE(0xFFC5,0x01);

}

void INIT_IMAGES(void)
{		

	GHOST_IMAGE._imageData = _GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	PLAYER_IMAGE._imageData = _PLAYER;
	DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;
	
	#if !defined(TINY_GAME)
		INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;	
		POWERUP_IMAGE._imageData = _POWERUP;
		GUN_IMAGE._imageData = _GUN;
		MISSILE_IMAGE._imageData = _MISSILE;
	#endif
	
	PLAYER_DOWN._imageData = _PLAYER_DOWN;
	PLAYER_UP._imageData = _PLAYER_UP;	
	PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
	PLAYER_LEFT._imageData = _PLAYER_LEFT;		

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_ENEMY_MISSILE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_ENEMY_MISSILE;
		
		BUBBLE_IMAGE._imageData = _BUBBLE;
		
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;	
			
	#endif
}

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_DRAW_BROKEN_WALL(x,y);
	}
#endif
	
void _draw(unsigned char x, unsigned char y, Image * image) 
{
	_DRAW(x,y,image->_imageData);
}

void _delete(unsigned char x, unsigned char y)
{
	_DELETE(x,y);
}

void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	if(*blinkCounter) 
	{
		_DRAW(x,y,image->_imageData);
		*blinkCounter=0;
	} 
	else 
	{
		_DELETE(x,y);
		*blinkCounter=1;
	}	
}

// #if defined(REDEFINED_CHARS)
	// void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	// { 
		// unsigned char i;
		// for(i=0;i<length;++i)
		// {
			// _DRAW_VERTICAL_WALL(x,y+i);
		// }
	// }

	// void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	// {
		// unsigned char i;
		// for(i=0;i<length;++i)
		// {
			// _DRAW_HORIZONTAL_WALL(x+i,y);
		// }
	// }
// #endif	


