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

#if defined(__C64__)
	#include "c64/c64_redefined_characters.h"
#elif defined(__ATMOS__)
	#include "atmos/atmos_redefined_characters.h"
#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)
	#include "atari/atari_mode1_graphics.h"
#elif defined(__NES__)
	#include "nes/nes_graphics.h"
#elif defined(__C16__) || defined(__PLUS4__)
	#include "c264/c264_graphics.h"
#elif defined(__C128__) && defined(C128_80COL_VIDEO_MODE)
	#include "c128/c128_80col_graphics.h"
#elif defined(__SPECTRUM__)
	#include "spectrum/spectrum_graphics.h"
#elif defined(__MSX__)
	#include "msx/msx_graphics.h"
#else
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

	char powerUp_blink = 1;
	char gun_blink = 1;
	char extra_points_blink = 1;
	char extra_life_blink = 1;
	char invincibility_blink = 1;
	char player_blink = 1;
	
	void INIT_GRAPHICS(void)
	{

	}
 
	void INIT_IMAGES(void)
	{		
		#if defined(__PET__) || defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__)
			PLAYER_IMAGE._color = COLOR_WHITE;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
			POWERUP_IMAGE._color = COLOR_WHITE;
			GUN_IMAGE._color = COLOR_WHITE;
			BOMB_IMAGE._color = COLOR_WHITE;
			DEAD_GHOST_IMAGE._color = COLOR_WHITE;
			EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
			EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
			INVINCIBILITY_IMAGE._color = COLOR_WHITE;
		#elif defined(__VIC20__)
			PLAYER_IMAGE._color = COLOR_YELLOW;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_BLUE;
			GUN_IMAGE._color = COLOR_BLUE;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;	
			EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;	
			EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;	
			INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
		#elif defined(__C128__) 
			PLAYER_IMAGE._color = COLOR_CYAN;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
			POWERUP_IMAGE._color = COLOR_YELLOW;
			GUN_IMAGE._color = COLOR_BROWN;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;
			EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
			EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;	
			INVINCIBILITY_IMAGE._color = COLOR_YELLOW;
		#elif defined(__ATARI__) || defined(__ATARIXL__)
			PLAYER_IMAGE._color = COLOR_WHITE;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
			POWERUP_IMAGE._color = COLOR_GRAY1;
			GUN_IMAGE._color = COLOR_GRAY1;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;
			EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;
			EXTRA_LIFE_IMAGE._color = COLOR_YELLOW;	
			INVINCIBILITY_IMAGE._color = COLOR_WHITE;
		#else
			PLAYER_IMAGE._color = COLOR_WHITE;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
			POWERUP_IMAGE._color = COLOR_WHITE;
			GUN_IMAGE._color = COLOR_WHITE;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;
			EXTRA_POINTS_IMAGE._color = COLOR_WHITE;
			EXTRA_LIFE_IMAGE._color = COLOR_WHITE;
			INVINCIBILITY_IMAGE._color = COLOR_WHITE;			
		#endif
			

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

#endif
