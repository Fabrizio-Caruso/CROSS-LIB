/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/
 
//#include <peekpoke.h>
#include "display_macros.h"

#include <string.h>

Image PLAYER_IMAGE;
Image GHOST_IMAGE;
Image DEAD_GHOST_IMAGE;
Image INVINCIBLE_GHOST_IMAGE;
Image BOMB_IMAGE;
Image POWERUP_IMAGE;
Image MISSILE_IMAGE;
Image GUN_IMAGE;

Image BUBBLE_IMAGE;

Image LEFT_ENEMY_MISSILE_IMAGE;
Image RIGHT_ENEMY_MISSILE_IMAGE;

Image EXTRA_POINTS_IMAGE;
Image EXTRA_LIFE_IMAGE;
Image INVINCIBILITY_IMAGE;

#include <stdio.h>

#define UDG_BASE 0xFF58

#if !defined(SPECTRUM_NATIVE_DIRECTIVES) && defined(SPECTRUM_UDG)
	#define UDG_N 13
	#include <stropts.h>
	unsigned char my_font[(128-32+UDG_N)*8];
	extern unsigned char font_8x8_rom[];
#endif

#if defined(SPECTRUM_NATIVE_DIRECTIVES)
	#include <graphics.h>
	#include <spectrum.h>
#else
	#include <arch/zx.h>

	#define PRINT_CLS      "\x0c"
	#define PRINT_INK_I    "\x10%c"
	#define PRINT_PAPER_P  "\x11%c"
	//#define PRINT_AT_CR    "\x16%c%c"

	#define INSIDE_COLOR  INK_BLACK
	#define OUTSIDE_COLOR INK_WHITE	
#endif


#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))

char powerUp_blink = 1;
char gun_blink = 1;
char extra_points_blink = 1;
char extra_life_blink = 1;
char invincibility_blink = 1;
char player_blink = 1;

#if defined(SPECTRUM_NATIVE_DIRECTIVES) && defined(SPECTRUM_UDG)
	void redefine(unsigned long loc, const unsigned char * data)
	{
		unsigned short i;
		for(i=0;i<8;++i)
		{
			POKE((unsigned short)(loc+i),data[i]);
		}
	}
#endif

void INIT_GRAPHICS(void)
{
	#if defined(SPECTRUM_NATIVE_DIRECTIVES) && defined(SPECTRUM_UDG)
		static const char player_down[8] =      { 24, 36, 24,102,153, 24, 36,102};
		static const char player_up[8] =        { 24, 60, 24,102,153, 24, 36,102};
		static const char player_right[8] =     { 24, 52, 25,118,152, 24, 20, 20};	
		static const char player_left[8] =      { 24, 44,152,110, 25, 24, 40, 40};
		static const char ghost[8] =            {129,126,165,129,129,189,129,126};
		static const char missile_right[8] =    {  0,  0, 15,252,252, 15,  0,  0};
		static const char missile_left[8] =     {  0,  0,240, 63, 63,240,  0,  0};
		static const char invincible_ghost[8] = { 60, 66,165,129, 90, 36, 36, 60};
		static const char gun[8] =              {  0,128,126,200,248,192,128,  0};
		static const char powerUp[8] =          {  0, 60, 54,223,231,122, 36, 24};
		static const char missile[8] =          {  0,  0,  8, 56, 28, 16,  0,  0};
		static const char bomb[8] =             { 60, 66,165,153,153,165, 66, 60};
		static const char bubble[8] =           { 24, 60, 60, 60,126, 90, 66, 66};
	#endif
	#if !defined(SPECTRUM_NATIVE_DIRECTIVES) && defined(SPECTRUM_UDG)
		static const char udg_definitions[] = { 
			 24, 36, 24,102,153, 24, 36,102, // player_down
			 24, 60, 24,102,153, 24, 36,102,
			 24, 52, 25,118,152, 24, 20, 20,	
			 24, 44,152,110, 25, 24, 40, 40,
			129,126,165,129,129,189,129,126,
			  0,  0, 15,252,252, 15,  0,  0,
			  0,  0,240, 63, 63,240,  0,  0,
			 60, 66,165,129, 90, 36, 36, 60,
			  0,128,126,200,248,192,128,  0,
			  0, 60, 54,223,231,122, 36, 24,
			  0,  0,  8, 56, 28, 16,  0,  0,
			 60, 66,165,153,153,165, 66, 60,
			 24, 60, 60, 60,126, 90, 66, 66
		};
	#endif	
		
	#if defined(SPECTRUM_32COL) && defined(SPECTRUM_NATIVE_DIRECTIVES)
		printf("\x1\x20");
	#endif
	
	#if defined(SPECTRUM_NATIVE_DIRECTIVES)
		clg();
		zx_border(0);
		zx_colour(PAPER_BLACK|INK_WHITE);
	#else
		zx_border(PAPER_BLACK);
		zx_cls(PAPER_BLACK);
		printf(PRINT_PAPER_P PRINT_INK_I PRINT_CLS, INSIDE_COLOR, INSIDE_COLOR);
	#endif	

	#if defined(SPECTRUM_NATIVE_DIRECTIVES) && defined(SPECTRUM_UDG)
		redefine(UDG_BASE,player_down); // 0x90
		// redefine(UDG_BASE+8,player_up);		// 0x91
		// redefine(UDG_BASE+8*2,player_right); //0x92
		// redefine(UDG_BASE+8*3,player_left); //0x93
		
		redefine(UDG_BASE+8*4,missile_right); //0x94
		redefine(UDG_BASE+8*5,missile_left); //0x95	
		
		redefine(UDG_BASE+8*6,invincible_ghost); //0x96
		redefine(UDG_BASE+8*7,gun); //0x97

		redefine(UDG_BASE+8*8,powerUp); // 0x98
		redefine(UDG_BASE+8*9,missile); //0x99

		redefine(UDG_BASE+8*10,bomb); //0xA0
		redefine(UDG_BASE+8*11,ghost); //0xA1
		redefine(UDG_BASE+8*12,bubble);	//0xA2
	#endif
	
	
	#if !defined(SPECTRUM_NATIVE_DIRECTIVES) && defined(SPECTRUM_UDG)
		memcpy(my_font, font_8x8_rom, (128-32)*8);	
		//memcpy(&font_8x8_rom[128*8], udg_definitions, UDG_N*8);
		memcpy(my_font+(128-32)*8, udg_definitions, UDG_N*8);
		ioctl(1, IOCTL_OTERM_FONT, (void*)(my_font - 256));
	#endif	
}

void INIT_IMAGES(void)
{		

	PLAYER_IMAGE._color = COLOR_CYAN;
	INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
	POWERUP_IMAGE._color = COLOR_MAGENTA;
	GUN_IMAGE._color = COLOR_MAGENTA;
	BOMB_IMAGE._color = COLOR_RED;
	DEAD_GHOST_IMAGE._color = COLOR_RED;
	EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
	EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;
	INVINCIBILITY_IMAGE._color = COLOR_YELLOW;			
		
	#if defined(SPECTRUM_NATIVE_DIRECTIVES) && defined(SPECTRUM_UDG)	
		GHOST_IMAGE._imageData = 139;
		INVINCIBLE_GHOST_IMAGE._imageData = 134;
		BOMB_IMAGE._imageData = 138;
		PLAYER_IMAGE._imageData = 128;
		POWERUP_IMAGE._imageData = 136;
		GUN_IMAGE._imageData = 135;
		MISSILE_IMAGE._imageData = 137;
		LEFT_ENEMY_MISSILE_IMAGE._imageData = 133;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = 132;
		BUBBLE_IMAGE._imageData = 140;
	#elif defined(SPECTRUM_UDG)
		GHOST_IMAGE._imageData = 139;
		INVINCIBLE_GHOST_IMAGE._imageData = 134;
		BOMB_IMAGE._imageData = 138;
		PLAYER_IMAGE._imageData = 128;
		POWERUP_IMAGE._imageData = 136;
		GUN_IMAGE._imageData = 135;
		MISSILE_IMAGE._imageData = 137;
		LEFT_ENEMY_MISSILE_IMAGE._imageData = 133;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = 132;
		BUBBLE_IMAGE._imageData = 140;	
	#else
		GHOST_IMAGE._imageData = 'o';
		INVINCIBLE_GHOST_IMAGE._imageData = '+';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '*';
		POWERUP_IMAGE._imageData = 'S';
		GUN_IMAGE._imageData = '!';
		MISSILE_IMAGE._imageData = '.';
		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		BUBBLE_IMAGE._imageData = '^';		
	#endif
	
	DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

	GHOST_IMAGE._color = COLOR_WHITE;
	MISSILE_IMAGE._color = COLOR_WHITE;

	LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;

	RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
	

	BUBBLE_IMAGE._color = COLOR_WHITE;
	
	EXTRA_POINTS_IMAGE._imageData = '$';
	
	EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
	INVINCIBILITY_IMAGE._imageData = '!';
}


void _draw_broken_wall(unsigned char x, unsigned char y)
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(COLOR_WHITE);
	cputc('X');
}

void _draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(image->_color);
	cputc(image->_imageData); 
}

void _delete(unsigned char x, unsigned char y)
{
	gotoxy(x+X_OFFSET,y+Y_OFFSET);
	cputc(' ');
}

void _blink_powerUp_draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(image->_color);
	if(powerUp_blink) 
	{
		cputc(image->_imageData); 
		powerUp_blink=0;
	} 
	else 
	{
		cputc(' '); 
		powerUp_blink=1;
	}	
}

void _blink_gun_draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(image->_color);
	if(gun_blink) 
	{
		cputc(image->_imageData); 
		gun_blink=0;
	} 
	else 
	{
		cputc(' '); 
		gun_blink=1;
	}	
}

void _blink_extra_points_draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(image->_color);
	if(extra_points_blink) 
	{
		cputc(image->_imageData); 
		extra_points_blink=0;
	} 
	else 
	{
		cputc(' '); 
		extra_points_blink=1;
	}	
}


void _blink_extra_life_draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(image->_color);
	if(extra_life_blink) 
	{
		cputc(image->_imageData); 
		extra_life_blink=0;
	} 
	else 
	{
		cputc(' '); 
		extra_life_blink=1;
	}	
}

void _blink_invincibility_draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(image->_color);
	if(invincibility_blink) 
	{
		cputc(image->_imageData); 
		invincibility_blink=0;
	} 
	else 
	{
		cputc(' '); 
		invincibility_blink=1;
	}	
}

void _blink_player_draw(unsigned char x, unsigned char y, Image * image) 
{
	gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
	SET_TEXT_COLOR(image->_color);
	if(player_blink) 
	{
		cputc(image->_imageData); 
		player_blink=0;
	} 
	else 
	{
		cputc(' '); 
		player_blink=1;
	}	
}

void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length) 
{ 
	unsigned char i; 
	
	SET_TEXT_COLOR(COLOR_WHITE);
	for(i=0;i<length;++i) 
	{ 
		gotoxy(x+X_OFFSET,y+Y_OFFSET+i);  printf("%c",'|'); 
	} 
}



