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

#include "../display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(TINY_GAME)
	extern Image INVINCIBLE_GHOST_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image FREEZE_IMAGE;
	extern Image MISSILE_IMAGE;
	extern Image GUN_IMAGE;
#endif

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_POINTS_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	extern Image SUPER_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
#endif

extern unsigned char XSize;

void INIT_GRAPHICS(void)
{
	#if defined(__ZX80__)
		gen_tv_field_init(0);
	#endif
}

void INIT_IMAGES(void)
{		
	GHOST_IMAGE._imageData = 'o';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	DEAD_GHOST_IMAGE._imageData = 'O';
	
	#if !defined(TINY_GAME)
		INVINCIBLE_GHOST_IMAGE._imageData = '+';	
		POWERUP_IMAGE._imageData = 'S';
		FREEZE_IMAGE._imageData = 'F';
		GUN_IMAGE._imageData = '!';
		MISSILE_IMAGE._imageData = '.';
	#endif

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		
		BUBBLE_IMAGE._imageData = 'I';
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'I';
		SUPER_IMAGE._imageData = 'H';
		
	#endif
}

	
#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		cputc('X');
	}
#endif
	
void _draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	cputc(image->_imageData);
}

void _delete(unsigned char x, unsigned char y)
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	cputc(' ');
}

#if !defined(TINY_GAME)
void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
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
}
#endif



#if !defined(TINY_GAME)
	void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	{ 
		unsigned char i;
		for(i=0;i<length;++i)
		{
			gotoxy((x+X_OFFSET),(y+i+Y_OFFSET));  
			cputc('i');		
		}
	}

	void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	{
		unsigned char i;
		gotoxy(X_OFFSET+x,Y_OFFSET+y); 
		for(i=0;i<length;++i)
		{
			cputc('-');
		}
	}
#endif

		


