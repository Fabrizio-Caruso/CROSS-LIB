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

#include <peekpoke.h>

#include "../display_macros.h"

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
#endif

Image PLAYER_RIGHT;
Image PLAYER_LEFT;
Image PLAYER_UP;
Image PLAYER_DOWN;	

extern char YSize; 

void redefine(unsigned long loc, const unsigned char * data)
{
	unsigned short i;
	for(i=0;i<8;++i)
	{
		POKE(loc+i,data[i]);
	}
}

// struct redefine_struct
// {
   // unsigned char ascii;
   // unsigned char bitmap[8];
// } ;

// struct redefine_struct redefine_map[] =
// {
	// {_PLAYER_DOWN, {12,18,12,51,45,12,18,51}},
	// {_PLAYER_UP, { 24, 60, 24,102,153, 24, 36,102}},
	// {_PLAYER_RIGHT, { 24, 52, 25,118,152, 24, 20, 20}},	
	// {_PLAYER_LEFT, { 24, 44,152,110, 25, 24, 40, 40}},
	// {_GHOST, {129,126,165,129,129,189,129,126}},
	// {_RIGHT_ENEMY_MISSILE, {  0,  0, 15,252,252, 15,  0,  0}},
	// {_LEFT_ENEMY_MISSILE, {  0,  0,240, 63, 63,240,  0,  0}},
	// {_INVINCIBLE_GHOST, { 60, 66,165,129, 90, 36, 36, 60}},
	// {_GUN, {  0,128,126,200,248,192,128,  0}},
	// {_POWERUP, {  0, 60, 54,223,231,122, 36, 24}},
	// {_MISSILE, {  0,  0,  8, 56, 28, 16,  0,  0}},
	// {_BOMB, { 60, 66,165,153,153,165, 66, 60}},
	// {_BUBBLE, { 24, 60, 60, 60,126, 90, 66, 66}},
	// {_INVINCIBILITY, { 24, 36, 24,  0,153,  0, 36,102}},
	// {_VERTICAL_BRICK, { 24, 24, 24, 48, 24, 12, 24, 24}},
	// {_HORIZONTAL_BRICK, {  0,  0,  0,255,  0,  0,  0,  0}}			
// };

void INIT_IMAGES(void)
{		
	unsigned char i;
	
	const unsigned char player[] =                {12,18,12,51,45,12,18,51};
	const unsigned char ghost[]  =                {33,30,33,51,33,45,33,30};
	const unsigned char bomb[]  =                 {30,33,51,45,45,51,33,30};
	const unsigned char powerUp[]  =              { 0,30,51,55,51,26,18,12};
	const unsigned char invincible_ghost[]  =     {30,33,51,33,33,18,18,12};
	const unsigned char gun[]  =                  { 0,32,31,40,56,32, 0, 0};
	const unsigned char missile[]  =              { 0, 0, 4,28,14, 8, 0, 0};
	
	const unsigned char dead_ghost[]  =           {18,30,33,51,33,45,51,30};
	
	const unsigned char player_left[] =           {12,22,12,62,13,12,20,20};
	const unsigned char player_right[] =          {12,26,12,31,44,12,10,10};		
	const unsigned char player_up[] =             {12,30,12,51,45,12,18,51};
	const unsigned char player_down[] =           {12,18,12,51,45,12,18,51};

	const unsigned char vertical_bar[] =             {12,12,12,12,12,12,12,12};
	const unsigned char horizontal_bar[] =           { 0, 0, 0,63,63,00,00,00};
	const unsigned char corner[] =                   {63,63,63,51,51,63,63,63};


	
	static const char vertical_brick[] =   { 24, 24, 24, 48, 24, 12, 24, 24};
	static const char horizontal_brick[] = {  0,  0,  0,255,  0,  0,  0,  0};		
	
	#if defined(FULL_GAME)
		const unsigned char right_arrow[] =             { 0, 0,56,31,31,56, 0, 0};
		const unsigned char left_arrow[] =            { 0, 0, 7,62,62, 7, 0, 0};
		
		const unsigned char bubble[] =            	   {12,30,30,30,63,45,33,33};
		const unsigned char invincibility[] =           {12,18,12,51, 0,12, 0,51};		
	#endif
	
	PLAYER_DOWN._color = 0;		
	GHOST_IMAGE._color = 0;
	MISSILE_IMAGE._color = 0;
	INVINCIBLE_GHOST_IMAGE._color = 0;
	POWERUP_IMAGE._color = 128u;
	GUN_IMAGE._color = 128u;
	BOMB_IMAGE._color = 128u;
	DEAD_GHOST_IMAGE._color = 128u;
		
	GHOST_IMAGE._imageData = (char) 0x60;
	INVINCIBLE_GHOST_IMAGE._imageData = (char) 0x7b;
	BOMB_IMAGE._imageData = (char) 0x5b;
	PLAYER_IMAGE._imageData = (char) 0x5c;
	

	POWERUP_IMAGE._imageData = (char) 0x5d;
	GUN_IMAGE._imageData = (char) 0x5e;
	MISSILE_IMAGE._imageData = (char) 0x5f;
	DEAD_GHOST_IMAGE._imageData = (char) 0x5a;

	PLAYER_IMAGE._color = 0;
	PLAYER_LEFT._color = 0;
	PLAYER_RIGHT._color = 0;
	PLAYER_UP._color = 0;
		
	PLAYER_LEFT._imageData = '*';
	PLAYER_RIGHT._imageData = ',';
	PLAYER_UP._imageData = '.';
	PLAYER_DOWN._imageData = '/';
	
	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = 0;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = 0;
		
		BUBBLE_IMAGE._imageData = (char) 0x23;
		BUBBLE_IMAGE._color = 0;	
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		EXTRA_POINTS_IMAGE._color = 128u;
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_DOWN._imageData;
		EXTRA_LIFE_IMAGE._color = 128u;
		
		INVINCIBILITY_IMAGE._imageData = 0x3B;
		INVINCIBILITY_IMAGE._color = 128u;
		
		redefine(0xb400 + INVINCIBILITY_IMAGE._imageData*8,invincibility);					
		redefine(0xb400 + BUBBLE_IMAGE._imageData*8,bubble);
		redefine(0xb400 + '<'*8,left_arrow);
		redefine(0xb400 + '>'*8,right_arrow);
		
		redefine(0xb400 + '|'*8,vertical_brick);		
		redefine(0xb400 + '-'*8,horizontal_brick);
		
	#endif 
	
	redefine(0xb400 + GHOST_IMAGE._imageData*8, ghost);		
	redefine(0xb400 + BOMB_IMAGE._imageData*8,bomb);
	redefine(0xb400 + POWERUP_IMAGE._imageData*8,powerUp);		
	redefine(0xb400 + INVINCIBLE_GHOST_IMAGE._imageData*8,invincible_ghost);
	redefine(0xb400 + GUN_IMAGE._imageData*8,gun);		
	redefine(0xb400 + MISSILE_IMAGE._imageData*8,missile);
	redefine(0xb400 + PLAYER_IMAGE._imageData*8,player);		
	redefine(0xb400 + PLAYER_LEFT._imageData*8,player_left);
	redefine(0xb400 + PLAYER_RIGHT._imageData*8,player_right);
	redefine(0xb400 + PLAYER_UP._imageData*8,player_up);		
	redefine(0xb400 + PLAYER_DOWN._imageData*8,player_down);

	// redefine(0xb400 + '|'*8,vertical_bar);
	// redefine(0xb400 + '-'*8,horizontal_bar);		
	redefine(0xb400 + '+'*8,corner);
	
	for(i=0;i<8;++i)
	{
		POKE(0xb400 + DEAD_GHOST_IMAGE._imageData*8 + i, dead_ghost[i]);
	}

	
	#if defined(FULL_GAME)
		BROKEN_WALL_IMAGE._imageData = 'X';
		BROKEN_WALL_IMAGE._color = 128u;
	#endif
}

void INIT_GRAPHICS(void)
{
	unsigned char i;
	
	// Initialize colors 
	for(i=0;i<Y_OFFSET;++i)
	{
		POKE(0xBB80+i*40,16);POKE(0xBB81+i*40,1); // red on black (inverted: cyan on white)
	}
	for(i=0+Y_OFFSET;i<28;++i)
	{
		POKE(0xBB80+i*40,16);POKE(0xBB81+i*40,3); // yellow on black (inverted: blue on white)
	}		
}
 
#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y) 
		{
			gotoxy(x+X_OFFSET,(y+Y_OFFSET)); 
			cputc(BOMB_IMAGE._imageData + BOMB_IMAGE._color);
		};	 
#endif

void _draw(unsigned char x,unsigned char y,Image * image) 
{
	POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, (unsigned char) image->_imageData + (unsigned char) image->_color);
};

void _delete(unsigned char x, unsigned char y)  {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);};

void _blink_draw(char x, char y, Image * image, unsigned char * blinkCounter) 
{
	(void) textcolor (image->_color);
	if(*blinkCounter) 
	{
		_draw(x,y,image); 
		*blinkCounter=0;
	} 
	else 
	{
		_delete(x,y);
		*blinkCounter=1;
	}	
};
