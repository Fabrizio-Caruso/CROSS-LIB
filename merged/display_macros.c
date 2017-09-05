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
 
#include "display_macros.h"
#if defined(__VG5K__)
	#include "vg5k/vg5k_graphics.h"
#elif defined(__C64__) && defined(REDEFINED_CHARS)
	#include "c64/c64_redefined_characters.h"
#elif defined(__ATMOS__) && defined(REDEFINED_CHARS)
	#include "atmos/atmos_redefined_characters.h"
#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)
	#include "atari/atari_mode1_graphics.h"
#elif (defined(__C16__)   || defined(__PLUS4__))   && defined(REDEFINED_CHARS)
	#include "c264/c264_graphics.h"
#elif defined(__C128__) && defined(C128_80COL_VIDEO_MODE)
	#include "c128/c128_80col_graphics.h"
#elif defined(__SPECTRUM__) && defined(REDEFINED_CHARS)
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
	
	#if defined(FULL_GAME)
		// #if defined(PLAYER_IMAGES)
			// Image PLAYER_RIGHT;
			// Image PLAYER_LEFT;
			// Image PLAYER_UP;
			// Image PLAYER_DOWN;
		// #endif
	
		Image LEFT_ENEMY_MISSILE_IMAGE;
		Image RIGHT_ENEMY_MISSILE_IMAGE;

		Image BUBBLE_IMAGE;

		Image EXTRA_POINTS_IMAGE;
		Image EXTRA_LIFE_IMAGE;
		Image INVINCIBILITY_IMAGE;	
	#endif
 
	void INIT_GRAPHICS() {}
	
	void INIT_IMAGES(void)
	{		

		#if defined(__VIC20__)
			PLAYER_IMAGE._color = COLOR_YELLOW;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_BLUE;
			GUN_IMAGE._color = COLOR_BLUE;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;	
		#elif defined(__ATARI__) || defined(__ATARIXL__)
			PLAYER_IMAGE._color = COLOR_WHITE;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
			POWERUP_IMAGE._color = COLOR_GRAY1;
			GUN_IMAGE._color = COLOR_GRAY1;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;
		#elif defined(__C16__)
			PLAYER_IMAGE._color = COLOR_CYAN;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
			POWERUP_IMAGE._color = COLOR_YELLOW;
			GUN_IMAGE._color = COLOR_YELLOW;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;				
		#else
			PLAYER_IMAGE._color = COLOR_WHITE;
			INVINCIBLE_GHOST_IMAGE._color = COLOR_WHITE;
			POWERUP_IMAGE._color = COLOR_WHITE;
			GUN_IMAGE._color = COLOR_WHITE;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;		
		#endif
			

		GHOST_IMAGE._imageData = 'o';
		INVINCIBLE_GHOST_IMAGE._imageData = '+';
		BOMB_IMAGE._imageData = 'X';
		PLAYER_IMAGE._imageData = '*';
		POWERUP_IMAGE._imageData = 'S';
		GUN_IMAGE._imageData = '!';
		MISSILE_IMAGE._imageData = '.';
		
		// Monochromatic vs color case
		#if defined(__CPC__) || defined(__MSX__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__PET__) || defined(__CBM610__)
			DEAD_GHOST_IMAGE._imageData = '_';
		#else
			DEAD_GHOST_IMAGE._imageData = 'o';
		#endif

		GHOST_IMAGE._color = COLOR_WHITE;
		MISSILE_IMAGE._color = COLOR_WHITE;
		
		#if defined(FULL_GAME)
			LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
			LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
			RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
			RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;	
			
			BUBBLE_IMAGE._imageData = '^';
			BUBBLE_IMAGE._color = COLOR_WHITE;
			
			EXTRA_POINTS_IMAGE._imageData = '$';
			
			EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
			INVINCIBILITY_IMAGE._imageData = 'V';		
		#endif
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

	void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char * blink_counter) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		SET_TEXT_COLOR(image->_color);
		if(*blink_counter) 
		{
			cputc(image->_imageData); 
			*blink_counter=0;
		} 
		else 
		{
			cputc(' '); 
			*blink_counter=1;
		}	
	}
#endif
