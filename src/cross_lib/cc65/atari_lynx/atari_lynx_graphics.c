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
#include <string.h>

#include <tgi.h>

extern unsigned char XSize;

// tgi_updatedisplay();
#define _DRAW(x,y,image) \
	{ \
	char chString[2]; \
	chString[0] = image->_imageData; \
	chString[1]='\0'; \
	tgi_setbgcolor(COLOR_BLACK); \
	tgi_setcolor(image->_color); \
	tgi_outtextxy(x*8,y*8,chString); \
	}
#define _DELETE(x,y) \
	{ \
	char chString[2]; \
	chString[0] = ' '; \
	chString[1]='\0'; \
	tgi_setbgcolor(_BG_COLOR); \
	tgi_setcolor(COLOR_BLACK); \
	tgi_outtextxy(x*8,y*8,chString); \
	}    
#define _DRAW_WALL(x,y)  {char chString[2]; chString[0] = '|'; chString[1]='\0'; tgi_outtextxy(x*8,y*8,chString);}   
#define _DRAW_BROKEN_WALL(x,y) 	{char chString[2]; chString[0] = 'X'; chString[1]='\0'; tgi_outtextxy(x*8,y*8,chString);}   

#include "../../display/display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image SKULL_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image BULLET_IMAGE;
extern Image GUN_IMAGE;

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image ROCKET_IMAGE;

	extern Image EXTRA_POINTS_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	
	extern Image FREEZE_IMAGE;	
	extern Image CHASE_IMAGE;
	extern Image SUPER_IMAGE;
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;
	
	extern Image BROKEN_WALL_IMAGE;
	
#endif

#define _BG_COLOR COLOR_GREY


// enable Mikeys interrupt response
#define CLI() asm("\tcli")

void CLEAR_SCREEN(void)
{
	unsigned char i;

	CLI();

	while (tgi_busy())  {  };

	tgi_setpalette(tgi_getdefpalette());
	tgi_setcolor(COLOR_WHITE);
	tgi_setbgcolor(_BG_COLOR);
	
	for(i=0;i<13;++i)
	{
		// sleep(1);	
		tgi_outtextxy(0,i*8,"                   ");
	}	
}


void INIT_GRAPHICS(void)
{
	    tgi_install (tgi_static_stddrv);


	// tgi_setpalette(tgi_getdefpalette());
    // tgi_clear ();
	
	// tgi_install(tgi_static_stddrv);
	
	// tgi_init();
	tgi_init ();		
	CLI();

	while (tgi_busy())  {  };

	tgi_setpalette(tgi_getdefpalette());
	tgi_setcolor(COLOR_WHITE);
	tgi_setbgcolor(_BG_COLOR);


	CLEAR_SCREEN();
}

void INIT_IMAGES(void)
{		

	#if !defined(NO_COLOR)
		PLAYER_IMAGE._color = COLOR_BLUE;
		SKULL_IMAGE._color = COLOR_YELLOW;
		POWERUP_IMAGE._color = COLOR_GREEN;
		GUN_IMAGE._color = COLOR_BROWN;
		BOMB_IMAGE._color = COLOR_RED;
		DEAD_GHOST_IMAGE._color = COLOR_RED;
		GHOST_IMAGE._color = COLOR_WHITE;
		BULLET_IMAGE._color = COLOR_WHITE;
	#endif
	
	GHOST_IMAGE._imageData = 'o';
	SKULL_IMAGE._imageData = '+';
	BOMB_IMAGE._imageData = 'X';
	PLAYER_IMAGE._imageData = '*';
	POWERUP_IMAGE._imageData = 'S';
	GUN_IMAGE._imageData = '!';
	BULLET_IMAGE._imageData = '.';
	DEAD_GHOST_IMAGE._imageData = 'O';

	#if defined(FULL_GAME)
		
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';

		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';

		ROCKET_IMAGE._imageData = '^';
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'V';
		
		FREEZE_IMAGE._imageData = 'F';												
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'V';	
		SUPER_IMAGE._imageData = 'H';	
		CONFUSE_IMAGE._imageData = 'C';
		ZOMBIE_IMAGE._imageData = 'Z';
		
		#if !defined(NO_COLOR)
			LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
			RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
			ROCKET_IMAGE._color = COLOR_WHITE;				
			EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
			EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
			INVINCIBILITY_IMAGE._color = COLOR_WHITE;	

			FREEZE_IMAGE._color = COLOR_CYAN;
			EXTRA_LIFE_IMAGE._color = COLOR_BLUE;
			INVINCIBILITY_IMAGE._color = COLOR_YELLOW;	
			SUPER_IMAGE._color = COLOR_RED;
			CONFUSE_IMAGE._color = COLOR_RED;
			ZOMBIE_IMAGE._color = COLOR_RED;			
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
	
void PRINT(unsigned char x, unsigned char y, char * str)
{
	tgi_outtextxy(x*8,y*8,str);
}


void print_05u0(unsigned char x, unsigned char y, unsigned short val)
{
	// unsigned char i;
	// unsigned char digits[7];
	// unsigned short tmp = val;
	
	// digits[0] = 0;
	// for(i=1;i<6;++i)
	// {
		// digits[i] = (unsigned char) ((tmp)%10);
		// tmp-= digits[i];
		// tmp/=10;
	// }
	
	// digits[6] = '\0';
	// tgi_outtextxy(x*8,y*8,digits);

}	

void print_02u(unsigned char x, unsigned char y, unsigned short val)
{
	// char str[3];
	
	// str[0] = val/10+49;
	// str[1] = val%10+48;
	// str[2] = '\0';	
	
	// tgi_outtextxy(x*8,y*8,str);
}	


void print_u(unsigned char x, unsigned char y, unsigned short val)
{
	// POKE(BASE_ADDR+x+y*((unsigned short)XSize), (unsigned char) (val+48));
	char str[2];
	
	str[0] = val+48;
	str[1] = '\0';
	
	tgi_outtextxy(x*8,y*8,str);
}

void print_level(unsigned short val)
{
	PRINT(XSize/2-4,YSize/2,"level");
	print_u(XSize/2+2, YSize/2, val);
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

#if !defined(TINY_GAME)
void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length) 
{
	unsigned char i;
	SET_TEXT_COLOR(COLOR_YELLOW);

	for(i=0;i<length;++i) 
	{ 
		gotoxy(x+i+X_OFFSET,y+Y_OFFSET);  cputc('-');
	} 	
}

void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length)
{
	// textcolor (COLOR_YELLOW);
	// cvlinexy (x+X_OFFSET,y+Y_OFFSET,length);		
}
#endif

		


