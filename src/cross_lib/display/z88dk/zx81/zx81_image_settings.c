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
#ifndef _ZX81_GRAPHICS_SETTINGS
#define _ZX81_GRAPHICS_SETTINGS

#include "../../cross_lib.h"

#define BASE_ADDR (unsigned short)(0x0400)

#define NOT_INVERTED 64
#define INVERTED -64
	
// BLUE
#define _PLAYER ('*')
// #define _PLAYER_DOWN 0
// #define _PLAYER_UP 1
// #define _PLAYER_RIGHT 2
// #define _PLAYER_LEFT 3

#define _SPACE (' ')

#define _GUN '!'

// YELLOW
#define _SKULL ('+')
#define _VERTICAL_BRICK ('I')
#define _HORIZONTAL_BRICK ('-')
#define _EXTRA_LIFE '*'
#define _EXTRA_POINTS '$'

// GREEN
#define _POWERUP  ('S')

// RED
#define _BOMB ('X')
#define _DEAD_GHOST 'O'

#define _INVINCIBILITY 'V'
#define _BULLET ('.')

#define _GHOST 'o'

#define _LEFT_HORIZONTAL_MISSILE ('>')
#define _RIGHT_HORIZONTAL_MISSILE ('<')
#define _ROCKET ('I')

#define _SUPER ('H')
#define _CONFUSE ('C')
#define _ZOMBIE ('Z')	

#define _BROKEN_WALL _BOMB

#endif // _ZX81_GRAPHICS_SETTINGS





void INIT_IMAGES(void)
{		
	GHOST_IMAGE._imageData = 'o';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	DEAD_GHOST_IMAGE._imageData = 'O';
	
	#if !defined(TINY_GAME)
		SKULL_IMAGE._imageData = '+';	
		POWERUP_IMAGE._imageData = 'S';
		#if defined(__ZX80__)
			GUN_IMAGE._imageData = 'G';		
		#else
			GUN_IMAGE._imageData = '!';
		#endif
		BULLET_IMAGE._imageData = '.';
	#endif

	#if defined(FULL_GAME)
		FREEZE_IMAGE._imageData = 'F';
	
		LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = '>';
		RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = '<';
		
		ROCKET_IMAGE._imageData = 'I';
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'I';
		
		CHASE_IMAGE._imageData = '.';
		SUPER_IMAGE._imageData = 'H';
		CONFUSE_IMAGE._imageData = 'C';
		ZOMBIE_IMAGE._imageData = 'Z';
		
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

#if !defined(TINY_GAME) && !defined(NO_BLINKING)
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
		#if defined(__ZX80__)
			unsigned char i;
			
			if(length>16) // Ugly workaround to avoid the screen to roll while drawn the horizontal wall
			{
				PRINT(x,y,"-------------------------------");
			}
			else
			{
				gotoxy(X_OFFSET+x,Y_OFFSET+y); 			
				for(i=0;i<length;++i)
				{
					cputc('-');
				}	
			}			
		#else
			unsigned char i;
		
			gotoxy(X_OFFSET+x,Y_OFFSET+y); 			
			for(i=0;i<length;++i)
			{
				cputc('-');
			}
		#endif
	}
#endif

		


