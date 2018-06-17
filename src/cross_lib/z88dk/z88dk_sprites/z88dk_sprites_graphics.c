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


#include "../../display/display_macros.h"

#include <graphics.h>
#include <games.h>

#include "z88dk_sprites_definitions.h"

#define _PLAYER_DOWN 0
#define _PLAYER_UP 1
#define _PLAYER_LEFT 2
#define _PLAYER_RIGHT 3
#define _BOMB 4
#define _GHOST 5

#define _SKULL 6
#define _BULLET 7
#define _POWERUP 8
#define _GUN 9
#define _EXTRA_POINTS 10

#define _DEAD_GHOST 11

#define _VERTICAL_BAR 12
#define _HORIZONTAL_BAR 13

#define _SPACE 14



// extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image DEAD_GHOST_IMAGE;

	
#if !defined(TINY_GAME)
	extern Image SKULL_IMAGE;
	extern Image BULLET_IMAGE;
	
	extern Image POWERUP_IMAGE;	
	extern Image GUN_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
	
#endif
	
#if defined(FULL_GAME)
	extern Image FREEZE_IMAGE;

	extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
	extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

	extern Image ROCKET_IMAGE;

	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	
	extern Image BROKEN_WALL_IMAGE;
	
	extern Image CHASE_IMAGE;
	extern Image SUPER_IMAGE;
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
#endif


#if defined(REDEFINED_CHARS)
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
#endif



extern unsigned char sprites[];


#define _DRAW(x,y,image) \
{ \
	putsprite(spr_or,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char) (image*(2+SPRITE_Y_SIZE))); \
}

	
#define _DELETE(x,y) \
{ \
	putsprite(spr_and,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char *) (_SPACE*(2+SPRITE_Y_SIZE)) ); \
}

#define _DRAW_VERTICAL_WALL(x,y) \
{ \
	putsprite(spr_or,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char *) (_VERTICAL_BAR*(2+SPRITE_Y_SIZE)) ); \
}
 
#define _DRAW_HORIZONTAL_WALL(x,y) \
{ \
	putsprite(spr_or,x*SPRITE_X_STEP,y*SPRITE_Y_STEP,sprites + (unsigned char *) (_HORIZONTAL_BAR*(2+SPRITE_Y_SIZE)) ); \
}

#define _DRAW_BROKEN_WALL(x,y) \
{ \
}



void INIT_GRAPHICS(void)
{

}


void INIT_IMAGES(void)
{		
	GHOST_IMAGE._imageData = _GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	// PLAYER_IMAGE._imageData = _PLAYER_DOWN;
	DEAD_GHOST_IMAGE._imageData = _DEAD_GHOST;
		
	#if !defined(TINY_GAME)
		SKULL_IMAGE._imageData = _SKULL;	
		BULLET_IMAGE._imageData = _BULLET;
	
		
		POWERUP_IMAGE._imageData = _POWERUP;
		
		GUN_IMAGE._imageData = _GUN;
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;		
	#endif

	#if defined(REDEFINED_CHARS)
		PLAYER_DOWN._imageData = _PLAYER_DOWN;
		PLAYER_UP._imageData = _PLAYER_UP;	
		PLAYER_RIGHT._imageData = _PLAYER_RIGHT;
		PLAYER_LEFT._imageData = _PLAYER_LEFT;		
	#endif
		
	#if defined(FULL_GAME)
		FREEZE_IMAGE._imageData = _POWERUP;
	
		LEFT_HORIZONTAL_MISSILE_IMAGE._imageData = _LEFT_HORIZONTAL_MISSILE;
		RIGHT_HORIZONTAL_MISSILE_IMAGE._imageData = _RIGHT_HORIZONTAL_MISSILE;
		
		ROCKET_IMAGE._imageData = _ROCKET;
				
		EXTRA_LIFE_IMAGE._imageData = _EXTRA_LIFE;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;	
		
		CHASE_IMAGE._imageData = _BULLET;
		SUPER_IMAGE._imageData = 'H';
		CONFUSE_IMAGE._imageData = 'C';
		ZOMBIE_IMAGE._imageData = 'Z';
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

#if !defined(NO_BLINKING)
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
#endif

#ifndef CLEAR_SCREEN
	void CLEAR_SCREEN(void)
	{
		unsigned char i;
		unsigned char j;
		
		for(i=0;i<YSize;++i)
		{
			for(j=0;j<XSize;++j)
			{
				_DELETE(j,i);
			}
		}
	}
#endif

// void PRINT(unsigned char x, unsigned char y, char * str)
// {
	// unsigned char i = 0;
	// while(str[i]!='\0')
	// {
		// POKE(BASE_ADDR+x+i+y*((unsigned short)XSize), str[i]); //str[i]-32-64);
		// ++i;
	// }
// }

// void print_05u0(unsigned char x, unsigned char y, unsigned short val)
// {
	// unsigned char i;
	// unsigned char digits[6];
	// unsigned short tmp = val;
	
	// digits[0] = 0;
	// for(i=1;i<6;++i)
	// {
		// digits[i] = (unsigned char) ((tmp)%10);
		// tmp-= digits[i];
		// tmp/=10;
	// }
	
	// for(i=0;i<6;++i)
	// {
		// POKE(BASE_ADDR+x+i+y*((unsigned short)XSize), (unsigned char) (digits[5-i])+48);
	// }
// }	

// void print_02u(unsigned char x, unsigned char y, unsigned short val)
// {
	// POKE(BASE_ADDR+x+y*  ((unsigned short)XSize), ((unsigned char) val)/10+48);		
	// POKE(BASE_ADDR+x+1+y*((unsigned short)XSize), ((unsigned char) val)%10+48);	
// }	


// void print_u(unsigned char x, unsigned char y, unsigned short val)
// {
	// POKE(BASE_ADDR+x+y*((unsigned short)XSize), (unsigned char) (val+48));
// }

// void print_level(unsigned short val)
// {
	// PRINT(XSize/2-4,YSize/2,"level");
	// print_u(XSize/2+2, YSize/2, val);
// }

// void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
// {
	// if(strlen(str)==5)
	// {	
		// print_05u0(x,y,val);
	// }
	// else if(strlen(str)==4)
	// {
		// print_02u(x,y,val);		
	// }
	// else if(strlen(str)==2)
	// {
		// print_u(x,y,val);		
	// }
	// else
	// {
		// print_level(val);
	// }
// }




#if !defined(TINY_GAME)
	void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	{ 
		unsigned char i;
		for(i=0;i<length;++i)
		{
			_DRAW_VERTICAL_WALL(x,y+i);
		}
	}

	void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length)
	{
		unsigned char i;
		for(i=0;i<length;++i)
		{
			_DRAW_HORIZONTAL_WALL(x+i,y);
		}
	}
#endif	


