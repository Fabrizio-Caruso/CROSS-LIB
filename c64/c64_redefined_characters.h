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

#ifndef __C64_REDEFINED_CHARACTERS
#define __C64_REDEFINED_CHARACTERS

	#include <peekpoke.h>

	 Image PLAYER_IMAGE;
	 Image GHOST_IMAGE;
	 Image DEAD_GHOST_IMAGE;
	 Image INVINCIBLE_GHOST_IMAGE;
	 Image BOMB_IMAGE;
	 Image POWERUP_IMAGE;
	 Image MISSILE_IMAGE;
	 Image GUN_IMAGE;
	 
	 Image LEFT_ENEMY_MISSILE_IMAGE;
	 Image RIGHT_ENEMY_MISSILE_IMAGE;
	 
	 Image BUBBLE_IMAGE;

	Image EXTRA_POINTS_IMAGE;
	
	 extern char YSize; 
	 
	char powerUp_blink = 1;
	char gun_blink = 1;
	char extra_points_blink = 1;
	
	void INIT_GRAPHICS(void)
	{
		// Select video bank and set the character set pointer to point to $C800
		// where the redefined charset has been placed by the linker
		POKE(56576u,0);
		POKE(53272u,2);
		POKE(648,192);
	}
	 
	 
	void INIT_IMAGES(void)
	{		
		PLAYER_IMAGE._color = COLOR_GRAY3;
		INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
		POWERUP_IMAGE._color = POWER_UP_COLOR;
		GUN_IMAGE._color = GUN_COLOR;
		BOMB_IMAGE._color = COLOR_YELLOW;
		DEAD_GHOST_IMAGE._color = COLOR_YELLOW;
			
		GHOST_IMAGE._imageData = (char) 0x76;
		INVINCIBLE_GHOST_IMAGE._imageData = (char) 0x77;
		BOMB_IMAGE._imageData = (char) 0x78;
		PLAYER_IMAGE._imageData = '\'';
		POWERUP_IMAGE._imageData = (char) 0x7A;
		GUN_IMAGE._imageData = (char) 0x7B;
		MISSILE_IMAGE._imageData = (char) 0x7C;
		DEAD_GHOST_IMAGE._imageData = BOMB_IMAGE._imageData;

		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_BLUE;

		LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
		LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;		
		
		BUBBLE_IMAGE._imageData = '^';
		BUBBLE_IMAGE._color = COLOR_WHITE;
		
		EXTRA_POINTS_IMAGE._imageData = 'p';
		EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
	}
	
	void _draw(char x, char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (image->_color);
		cputc(image->_imageData); 
	};

	void _blink_powerUp_draw(char x, char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (image->_color);
		if(powerUp_blink) 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40,image->_imageData + image->_color );
			cputc(image->_imageData); 
			powerUp_blink=0;
		} 
		else 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32); 
			cputc(' '); 
			powerUp_blink=1;
		}	
	};
	
	void _blink_gun_draw(char x, char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (image->_color);
		if(gun_blink) 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40,image->_imageData + image->_color );
			cputc(image->_imageData); 
			gun_blink=0;
		} 
		else 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32); 
			cputc(' '); 
			gun_blink=1;
		}	
	};

	void _blink_extra_points_draw(char x, char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		(void) textcolor (image->_color);
		if(extra_points_blink) 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40,image->_imageData + image->_color );
			cputc(image->_imageData); 
			extra_points_blink=0;
		} 
		else 
		{
			//POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32); 
			cputc(' '); 
			extra_points_blink=1;
		}	
	};
	
	void _delete(char x, char y)
	{
		gotoxy(x+X_OFFSET,y+Y_OFFSET);cputc(' ');
	};
	
#endif // C64_REDEFINED_CHARACTERS