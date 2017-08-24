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
	
	#define POKE(loc,val) bpoke((loc),(val));
	
	#define VIDEO_MEMORY_BASE 0x4000

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

	Image LEFT_ENEMY_MISSILE_IMAGE;
	Image RIGHT_ENEMY_MISSILE_IMAGE;

	Image BUBBLE_IMAGE;

	Image EXTRA_POINTS_IMAGE;
	Image EXTRA_LIFE_IMAGE;
	Image INVINCIBILITY_IMAGE;
	
	extern unsigned char YSize; 

	void INIT_IMAGES(void)
	{		
		
		PLAYER_IMAGE._color = 1;		
		GHOST_IMAGE._color = 1;
		MISSILE_IMAGE._color = 1;
		INVINCIBLE_GHOST_IMAGE._color = 1;
		POWERUP_IMAGE._color = 1;
		GUN_IMAGE._color = 1;
		BOMB_IMAGE._color = 1;
		DEAD_GHOST_IMAGE._color = 1;
			

		GHOST_IMAGE._imageData = 'o';
		INVINCIBLE_GHOST_IMAGE._imageData = '+';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '*';
		POWERUP_IMAGE._imageData = 'S';
		GUN_IMAGE._imageData = '!';
		MISSILE_IMAGE._imageData = '.';
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_WHITE;

		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
		
		BUBBLE_IMAGE._imageData = '^';
		BUBBLE_IMAGE._color = COLOR_WHITE;
		
		EXTRA_POINTS_IMAGE._imageData = '$';
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = 'V';

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
		no_cursor();
	}

	void no_cursor(void)
	{
		//TODO: Fix this to disable cursor
		#asm
			_ef9345:
				defb 0x04,0x20,0x82,0x29,0x00		
				pop bc
				pop hl
				pop de
				push de
				push hl
				push bc
				ld hl,_ef9345
				call 0x00AD		
		#endasm		
	}
	
	int _draw_ch(unsigned char x, unsigned char y, unsigned char ch, unsigned char col)
	{
		//no_cursor();
		gotoxy(x,y);
		cputc(ch);
		// int xy = 0;
		// int chCol = 0;
		// xy = ((y+8+Y_OFFSET)<<8) | (x+X_OFFSET);
		// chCol = (ch<<8) | col;
		// no_cursor();
		// return _draw_ch_aux(chCol,xy);
	}

	int _draw_ch_aux(int chCol, int xy)
	{
		// #asm
		// di
		
		// pop bc   ; bc = ret address
		// pop hl   ; hl = int b
		// pop de  ; de = int a

		// push de    ; now restore stack
		// push hl
		// push bc
		
		// call 0x0092	
		
		// ei
		// #endasm
		
		// #asm
		// di
		// pop bc   ; bc = ret address
		// pop hl   ; hl = int b
		// pop de  ; de = int a

		// push de    ; now restore stack
		// push hl
		// push bc   	
		
		// call 0x0092 
		// ei
		// #endasm
		
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
		if(*blinkCounter) 
		{
			_draw_ch(x,y,image->_imageData, image->_color);
			*blinkCounter=0;
		} 
		else 
		{
			_draw_ch(x,y,32, 0);
			*blinkCounter=1;
		}
	}
	

	

	
#endif // _VG5K_REDEFINED_CHARACTERS