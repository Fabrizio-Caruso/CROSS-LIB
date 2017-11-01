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

const unsigned char PALETTE[]={
0x1f, 0x00, 0x10, 0x20
}; //black, gray, lt gray, white

extern unsigned char XSize;

#define PPU_CTRL  *((unsigned char*)0x2000)
#define PPU_MASK  *((unsigned char*)0x2001)
#define PPU_STATUS  *((unsigned char*)0x2002)
#define SCROLL   *((unsigned char*)0x2005)
#define PPU_ADDRESS  *((unsigned char*)0x2006)
#define PPU_DATA  *((unsigned char*)0x2007)

#define _DRAW(x,y,image) 

#define _DELETE(x,y)     

#define _DRAW_WALL(x,y)  

#define _DRAW_BROKEN_WALL(x,y)  


#include "../display_macros.h"
#include "../patch/generic_conio_patch.h"

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


void INIT_GRAPHICS(void)
{
	unsigned char index;
	
	const unsigned char TEXT[]={"Hello World!"};	
	
	// turn off the screen
	PPU_CTRL = 0;
	PPU_MASK = 0;

	// load the palette
	PPU_ADDRESS = 0x3f; // set an address in the PPU of 0x3f00
	PPU_ADDRESS = 0x00;
	
	// load the palette
	PPU_ADDRESS = 0x3f; // set an address in the PPU of 0x3f00
	PPU_ADDRESS = 0x00;
	for(index = 0; index < sizeof(PALETTE); ++index)
	{
		PPU_DATA = PALETTE[index];
	}	
	
	//
	// load the text
	PPU_ADDRESS = 0x21; // set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0xca;  // about the middle of the screen
	for( index = 0; index < sizeof(TEXT); ++index ){
	PPU_DATA = TEXT[index];
	}

	// reset the scroll position 
	PPU_ADDRESS = 0;
	PPU_ADDRESS = 0;
	SCROLL = 0;
	SCROLL = 0;

	// turn on screen
	PPU_CTRL = 0x90; // NMI on
	PPU_MASK = 0x1e; // screen on

	// infinite loop
	while (1); 	
}

void INIT_IMAGES(void)
{		

	#if !defined(NO_COLOR)
		PLAYER_IMAGE._color = COLOR_WHITE;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
		POWERUP_IMAGE._color = COLOR_WHITE;
		GUN_IMAGE._color = COLOR_WHITE;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_WHITE;
	#endif
	
	GHOST_IMAGE._imageData = 'o';
	INVINCIBLE_GHOST_IMAGE._imageData = '+';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	POWERUP_IMAGE._imageData = 'S';
	GUN_IMAGE._imageData = '!';
	MISSILE_IMAGE._imageData = '.';
	DEAD_GHOST_IMAGE._imageData = 'O';

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';

		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';

		BUBBLE_IMAGE._imageData = '^';
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'V';
		
		#if !defined(NO_COLOR)
			LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
			RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
			BUBBLE_IMAGE._color = COLOR_WHITE;				
			EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
			EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
			INVINCIBILITY_IMAGE._color = COLOR_WHITE;	
		#endif
	#endif
}



void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	if(*blinkCounter) 
	{
		_DRAW(x,y,image);
		*blinkCounter=0;
	} 
	else 
	{
		_DELETE(x,y);
		*blinkCounter=1;
	}	
}

void _draw(unsigned char x, unsigned char y, Image * image) 
{
	_DRAW(x,y,image);
}

void _delete(unsigned char x, unsigned char y)
{
	_DELETE(x,y);
}


#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		_DRAW_BROKEN_WALL(x,y);
	}
#endif
	


		


