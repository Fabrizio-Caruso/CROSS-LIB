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

#include "../../cross_lib.h"
#include "../../display/display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(TINY_GAME)
	extern Image SKULL_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image MISSILE_IMAGE;
	extern Image GUN_IMAGE;
#endif

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
		SKULL_IMAGE._imageData = '+';	
		POWERUP_IMAGE._imageData = 'S';
		#if defined(__ZX80__)
			GUN_IMAGE._imageData = 'G';		
		#else
			GUN_IMAGE._imageData = '!';
		#endif
		MISSILE_IMAGE._imageData = '.';
	#endif

	#if defined(FULL_GAME)
		FREEZE_IMAGE._imageData = 'F';
	
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		
		BUBBLE_IMAGE._imageData = 'I';
		
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

		


