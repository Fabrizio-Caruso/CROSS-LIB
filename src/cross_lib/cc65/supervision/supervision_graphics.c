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

#include <string.h>

#include <supervision.h>

// BLUE
#define _PLAYER 0

#define _PLAYER_DOWN 0
#define _PLAYER_UP 1
#define _PLAYER_RIGHT 2
#define _PLAYER_LEFT 3

#define _GHOST 4

#define _BOMB 5

#define _POWERUP 6

// YELLOW
#define _VERTICAL_BRICK 7
#define _HORIZONTAL_BRICK 8
#define _EXTRA_LIFE 9
#define _EXTRA_POINTS 10

// GREEN
#define _SKULL 11

// RED
#define _GUN 12

#define _DEAD_GHOST 13

#define _INVINCIBILITY 14
#define _BULLET 15

#define _SPACE 16

#define _LEFT_HORIZONTAL_MISSILE 17
#define _RIGHT_HORIZONTAL_MISSILE 18
#define _ROCKET 19

#define _SUPER 20
#define _CONFUSE 21
#define _ZOMBIE 22

#include "../../display/display_macros.h"

extern Image PLAYER_IMAGE;
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

#define BASE_ADDR (unsigned short)(0x0400)



#if defined(REDEFINED_CHARS)
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
#endif


#define _DRAW(x,y,image) \
{ \
	unsigned char k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[2*x+48*k+48*8*y]=128+32+8+2; \
		SV_VIDEO[2*x+48*k+48*8*y+1]=128+32+8+2; \
	} \
}

#define _DELETE(x,y) \
{ \
	unsigned char k; \
	for(k=0;k<8;++k) \
	{ \
		SV_VIDEO[2*x+48*k+48*8*y]=0; \
		SV_VIDEO[2*x+48*k+48*8*y+1]=0; \
	} \
}

#define _DRAW_VERTICAL_WALL(x,y) \
{ \
}
 
#define _DRAW_HORIZONTAL_WALL(x,y) \
{ \
}

#define _DRAW_BROKEN_WALL(x,y) \
{ \
}

static unsigned char sprites[] = {
 24, 36, 24,102,153, 24, 36,102, // down
 24, 60, 24,102,153, 24, 36,102, // up
 24, 52, 25,118,152, 24, 20, 20, // right
 24, 44,152,110, 25, 24, 40, 40, // left
 129,126,165,129,129,189,129,126, // ghost
  60, 66,165,153,153,165, 66, 60, // bomb
   0, 60, 54,223,231,122, 36, 24 // powerup
};

void INIT_GRAPHICS(void)
{
	unsigned char i;
	unsigned short j;
	unsigned short k;
	
	for(j=0;j<20;++j)
	{
		for(i=0;i<20;++i)
		{
			for(k=0;k<8;++k)
			{
				SV_VIDEO[2*i+48*k+48*8*j]=0;
				SV_VIDEO[2*i+48*k+48*8*j+1]=0;	
			}
			// WAIT_PRESS();
		}
	}
}

void INIT_IMAGES(void)
{		
	GHOST_IMAGE._imageData = _GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	PLAYER_IMAGE._imageData = _PLAYER;
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
		SUPER_IMAGE._imageData = _SUPER;
		CONFUSE_IMAGE._imageData = _CONFUSE;
		ZOMBIE_IMAGE._imageData = _ZOMBIE;
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


void CLEAR_SCREEN(void)
{
	unsigned char i;
	unsigned char j;
	
	for(i=0;i<20;++i)
	{
		for(j=0;j<20;++j)
		{
		_DELETE(i,j);
		}
	}
}

#if defined(ALT_PRINT)
void PRINT(unsigned char x, unsigned char y, char * str)
{
	unsigned char i = 0;
	while(str[i]!='\0')
	{
		// POKE(BASE_ADDR+x+i+y*((unsigned short)XSize), screenCode(str[i])); 
		++i;
	}
}

void print_05u0(unsigned char x, unsigned char y, unsigned short val)
{
	unsigned char i;
	unsigned char digits[6];
	unsigned short tmp = val;
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (unsigned char) ((tmp)%10);
		tmp-= digits[i];
		tmp/=10;
	}
	
	for(i=0;i<6;++i)
	{
		// POKE(BASE_ADDR+x+i+y*((unsigned short)XSize), (unsigned char) (digits[5-i])+48);
	}
}	

void print_02u(unsigned char x, unsigned char y, unsigned short val)
{
	// POKE(BASE_ADDR+x+y*  ((unsigned short)XSize), ((unsigned char) val)/10+48);		
	// POKE(BASE_ADDR+x+1+y*((unsigned short)XSize), ((unsigned char) val)%10+48);	
}	


void print_u(unsigned char x, unsigned char y, unsigned short val)
{
	// POKE(BASE_ADDR+x+y*((unsigned short)XSize), (unsigned char) (val+48));
}

void print_level(unsigned short val)
{
	// PRINT(XSize/2-4,YSize/2,"level");
	// print_u(XSize/2+2, YSize/2, val);
}

void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
{
	if(strlen(str)==5)
	{	
		print_05u0(x,y,val);
	}
	else if(strlen(str)==4)
	{
		print_02u(x,y,val);		
	}
	else if(strlen(str)==2)
	{
		print_u(x,y,val);		
	}
	else
	{
		print_level(val);
	}
}
#endif



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


