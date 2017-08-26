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
/* wazrranty.  In no event will the authors be held liable for any damages    */
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

#ifndef _VG5K_REDEFINED_CHARACTERS
#define _VG5K_REDEFINED_CHARACTERS

	#include<stdlib.h> 
	#include "character.h"
	
	#define POKE(loc,val) bpoke((loc),(val));
	
	#define VIDEO_MEMORY_BASE 0x4000
	
	extern Character bombs[BOMBS_NUMBER];
	
	Image PLAYER_IMAGE;
	Image GHOST_IMAGE;
	Image DEAD_GHOST_IMAGE;
	Image INVINCIBLE_GHOST_IMAGE;
	Image BOMB_IMAGE;
	Image POWERUP_IMAGE;
	Image MISSILE_IMAGE;
	Image GUN_IMAGE;

	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;
	Image PLAYER_UP;
	Image PLAYER_DOWN;

	
	extern unsigned char YSize; 
	extern unsigned char XSize;

	
	#define VG5K_BLACK 0
	#define VG5K_RED 1
	#define VG5K_GREEN 2
	#define VG5K_YELLOW 3
	#define VG5K_BLUE 4
	#define VG5K_VIOLET 5
	#define VG5K_CYAN 6
	#define VG5K_WHITE 7
	
	void INIT_IMAGES(void)
	{		
		
		PLAYER_IMAGE._color = VG5K_CYAN;		
		GHOST_IMAGE._color = VG5K_WHITE;
		MISSILE_IMAGE._color = VG5K_WHITE;
		INVINCIBLE_GHOST_IMAGE._color = VG5K_YELLOW;
		POWERUP_IMAGE._color = VG5K_GREEN;
		GUN_IMAGE._color = VG5K_VIOLET;
		BOMB_IMAGE._color = VG5K_RED;
		DEAD_GHOST_IMAGE._color = VG5K_RED;
			

		GHOST_IMAGE._imageData = 'o';
		INVINCIBLE_GHOST_IMAGE._imageData = '+';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '*';
		POWERUP_IMAGE._imageData = 'S';
		GUN_IMAGE._imageData = '!';
		MISSILE_IMAGE._imageData = '.';
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

		GHOST_IMAGE._color = VG5K_WHITE;
		MISSILE_IMAGE._color = VG5K_WHITE;
	}
	
	void INIT_GRAPHICS(void)
	{
		{
			unsigned char i;	
			for(i=0;i<24;++i)
			{
				POKE(VIDEO_MEMORY_BASE+80*i,32);
				POKE(VIDEO_MEMORY_BASE+1+80*i,1);
			}	
			POKE(0x47FD,0);
		}
	}

	void no_cursor(void)
	{	
		#asm
		jr clean_cursor
		ef9345:
			defb 0x04,0x20,0x82,0x29,0x00
		clean_cursor:
			ld hl,ef9345
			ld ix,$47FA
			call 0x00AD
		#endasm		
	}
	
	void CLEAR_SCREEN()
	{
		no_cursor();
		clrscr();
		INIT_GRAPHICS();
	}		

	void DRAW_HORIZONTAL_BORDER(unsigned char y)
	{
		unsigned char i;
		gotoxy(X_OFFSET+1,Y_OFFSET+1+y); 
		for(i=0;i<XSize;++i)
		{
			cputc('-');
		}
	}
	
	void DRAW_VERTICAL_BORDER(unsigned char x)
	{
		unsigned char i;
		for(i=2;i<YSize;++i) 
		{ 
			gotoxy(X_OFFSET+1+x,Y_OFFSET+i); 
			cputc('|'); 
		} 		
	}
	
	void DRAW_BORDERS(void)
	{ 
		unsigned char i; 
		DRAW_HORIZONTAL_BORDER(0);
		DRAW_HORIZONTAL_BORDER(YSize-1);	
		DRAW_VERTICAL_BORDER(0);
		DRAW_VERTICAL_BORDER(XSize-1);
	}	
	
	void DRAW_BOMBS(void) 
	{
		unsigned char i;
		for(i=0;i<BOMBS_NUMBER;++i)
		{
			 _draw_ch(bombs[i]._x, bombs[i]._y, BOMB_IMAGE._imageData, BOMB_IMAGE._color);
		}
	}	
	
	void _draw_ch(unsigned char x, unsigned char y, unsigned char ch, unsigned char col)
	{
		no_cursor();		
			
		{			
			int xy = 0;
			int chCol = 0;
			xy = ((y+8+Y_OFFSET-1)<<8) | (x+X_OFFSET);
			chCol = (ch<<8) | col;
			
			_draw_ch_aux(chCol,xy);
		}
	}

	int _draw_ch_aux(int chCol, int xy)
	{		
		#asm
		
		pop bc   ; bc = ret address
		pop hl   ; hl = int b
		pop de  ; de = int a

		push de    ; now restore stack
		push hl
		push bc
		
		ld ix,$47FA	
		
		call 0x0092	
		
		#endasm	
	}
	
	
	void _draw(unsigned char x,unsigned char y,Image * image) 
	{
		_draw_ch(x,y,image->_imageData, image->_color);
	}

	void _delete(unsigned char x, unsigned char y)  
	{
		_draw_ch(x,y,32, 0);	
	}
	

	
	void _blink_draw(unsigned char x,unsigned char y,Image * image, unsigned char *blinkCounter)
	{
		 _draw_ch(x,y, image->_imageData, image->_color);
		// if(*blinkCounter) 
		// {
			// _draw_ch(x,y,image->_imageData, image->_color);
			// *blinkCounter=0;
		// } 
		// else 
		// {
			// _draw_ch(x,y,32, 0);
			// *blinkCounter=1;
		// }
	}
	

	

	
#endif // _VG5K_REDEFINED_CHARACTERS
