/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
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

#include "display_macros.h"

#if defined(__VZ__) || defined(__WINCMOC__) || defined(__CMOC__) || defined(__OSIC1P__)
	#define VERTICAL_BRICK '#'
#else
	#define VERTICAL_BRICK '|'
#endif

#if defined(FULL_GAME)
	unsigned char playerBlink;	
#endif

#if !defined(REDEFINED_CHARS)
	Image PLAYER_IMAGE;
#endif

Image GHOST_IMAGE;
Image DEAD_GHOST_IMAGE;
Image BOMB_IMAGE;

#if !defined(TINY_GAME)
	Image POWERUP_IMAGE;
	Image MISSILE_IMAGE;
	Image GUN_IMAGE;
	Image EXTRA_POINTS_IMAGE;
	
	Image INVINCIBLE_GHOST_IMAGE;	
#endif

#if defined(FULL_GAME)
	Image LEFT_ENEMY_MISSILE_IMAGE;
	Image RIGHT_ENEMY_MISSILE_IMAGE;

	Image BUBBLE_IMAGE;

	Image FREEZE_IMAGE;	
	
	Image EXTRA_LIFE_IMAGE;
	Image INVINCIBILITY_IMAGE;	
	
	Image CHASE_IMAGE;
	
	Image SUPER_IMAGE;
	Image CONFUSE_IMAGE;
	Image ZOMBIE_IMAGE;
	
	Image BROKEN_WALL_IMAGE;
#endif


#if defined(__VG5K__)
	//
#elif defined(__C64__) && defined(REDEFINED_CHARS)
	// 
#elif defined(__ATMOS__) && defined(REDEFINED_CHARS)
	// 
#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1) && defined(REDEFINED_CHARS)
	// 
#elif (defined(__C16__)   || defined(__PLUS4__))   && defined(REDEFINED_CHARS)
	// 
// #elif defined(__C128__) && defined(C128_80COL_VIDEO_MODE)
	// 
#elif defined(__SPECTRUM__)
	//	
#elif defined(__MSX__) && defined(REDEFINED_CHARS)
	// 
#elif defined(__MSX__) && !defined(MSX_MODE1)
	//
#elif defined(__SVI__) && defined(MSX_MODE0)
	//
#elif defined(__CPC__)
	//
#elif defined(__AQUARIUS__) && defined(TINY_GAME)
	//	
#elif defined(__NES__) && defined(NES_COLOR)
	//	
#elif defined(__ZX80__) || defined(__ZX81__) || defined(__ACE__) || defined(__LAMBDA__)
	//
#elif defined(__ATARI_LYNX__)
	//
#elif defined(__GAMATE__)
	//	
#elif defined(__VIC20__) && defined(REDEFINED_CHARS)
	//
#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED)
	//
#elif (defined(__WINCMOC__) && defined(__CMOC__)) && defined(REDEFINED_CHARS)
	//
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	//
#else
	void INIT_GRAPHICS() 
	{
		#if defined(__VIC20__) && defined(TINY_GAME)
			#include<peekpoke.h>
			POKE(646,1);
			POKE(36879L,9);
		#elif defined(__C128__) && defined(C128_80COL_VIDEO_MODE)
			#include <c128.h>
			
			fast();
			videomode(80);		
		#endif	

		
	}
	
	void INIT_IMAGES(void)
	{		
		#if defined(COLOR)
			PLAYER_IMAGE._color = COLOR_CYAN;
			BOMB_IMAGE._color = COLOR_RED;
			DEAD_GHOST_IMAGE._color = COLOR_RED;

			#if !defined(TINY_GAME)
				INVINCIBLE_GHOST_IMAGE._color = COLOR_YELLOW;
				POWERUP_IMAGE._color = COLOR_GREEN;
				GUN_IMAGE._color = COLOR_BLUE;
				EXTRA_POINTS_IMAGE._color = COLOR_YELLOW;			
			#endif

			#if defined(FULL_GAME)
				FREEZE_IMAGE._color = COLOR_CYAN;
				EXTRA_LIFE_IMAGE._color = COLOR_BLUE;
				INVINCIBILITY_IMAGE._color = COLOR_YELLOW;	
				SUPER_IMAGE._color = COLOR_RED;
				CONFUSE_IMAGE._color = COLOR_RED;
				ZOMBIE_IMAGE._color = COLOR_RED;
			#endif		
		#endif
		
		#if defined(__WINCMOC__) || defined(__CMOC__)
			BOMB_IMAGE._imageData = 'x';	
			GHOST_IMAGE._imageData = 'O';				
		#else
			BOMB_IMAGE._imageData = 'X';
			GHOST_IMAGE._imageData = 'o';			
		#endif
		
		#if !defined(__CREATIVISION__)
			PLAYER_IMAGE._imageData = '*';			
		#else
			PLAYER_IMAGE._imageData = 'I';
		#endif
		
		#if !defined(TINY_GAME)
			INVINCIBLE_GHOST_IMAGE._imageData = '+';
			EXTRA_POINTS_IMAGE._imageData = '$';			
			#if defined(__WINCMOC__) || defined(__CMOC__)
				POWERUP_IMAGE._imageData = 's';	
				GUN_IMAGE._imageData = '!';				
			#else
				POWERUP_IMAGE._imageData = 'S';
				GUN_IMAGE._imageData = '!';			
			#endif

			MISSILE_IMAGE._imageData = '.';
		#endif
		
		#if defined(__PET__) || defined(__CBM610__) || defined(__ATARI__) || defined(__ATARIXL__) 			
			DEAD_GHOST_IMAGE._imageData = 'O';
		#elif defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__GAMATE__)
			DEAD_GHOST_IMAGE._imageData = '#';
		#elif defined(__CREATIVISION__) || defined(__ZX81__) || defined(__LAMBDA__) || defined(__ZX80__) || defined(__ACE__) 
			DEAD_GHOST_IMAGE._imageData = 'x';	
		#elif defined(__VZ__) || defined(__WINCMOC__) || defined(__CMOC__)
			DEAD_GHOST_IMAGE._imageData = '#';				
		#elif defined(NO_COLOR)
			DEAD_GHOST_IMAGE._imageData = '#';		 
		#else
			DEAD_GHOST_IMAGE._imageData = 'o';
		#endif

		#if defined(COLOR)
			#if !defined(__GAMATE__)
				GHOST_IMAGE._color = COLOR_WHITE;
				#if !defined(TINY_GAME)
					MISSILE_IMAGE._color = COLOR_WHITE;
				#endif
			#else
				GHOST_IMAGE._color = COLOR_YELLOW;
				#if !defined(TINY_GAME)
					MISSILE_IMAGE._color = COLOR_YELLOW;
				#endif
			#endif
		#endif
		
		#if defined(FULL_GAME)
			#if !defined(NO_COLOR)
				LEFT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
				RIGHT_ENEMY_MISSILE_IMAGE._color = COLOR_WHITE;
				BUBBLE_IMAGE._color = COLOR_WHITE;	
				BROKEN_WALL_IMAGE._color = COLOR_RED;		
			#endif
			LEFT_ENEMY_MISSILE_IMAGE._imageData = '>';
			RIGHT_ENEMY_MISSILE_IMAGE._imageData = '<';
		
			BUBBLE_IMAGE._imageData = '^';
			#if defined(__WINCMOC__) || defined(__CMOC__)	
				FREEZE_IMAGE._imageData = 'f';								
				EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
				INVINCIBILITY_IMAGE._imageData = 'v';	
				SUPER_IMAGE._imageData = 'h';
				CONFUSE_IMAGE._imageData = 'c';
				ZOMBIE_IMAGE._imageData = 'z';
			#else
				FREEZE_IMAGE._imageData = 'F';												
				EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
				INVINCIBILITY_IMAGE._imageData = 'V';	
				SUPER_IMAGE._imageData = 'H';	
				CONFUSE_IMAGE._imageData = 'C';
				ZOMBIE_IMAGE._imageData = 'Z';
			#endif
			BROKEN_WALL_IMAGE._imageData = 'X';
		#endif
	}

	#if defined(FULL_GAME)
		void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
		{
			gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
			#if !defined(NO_COLOR)
				SET_TEXT_COLOR(BROKEN_WALL_IMAGE._color);
			#endif
			cputc(BROKEN_WALL_IMAGE._imageData); 		
		}
	#endif

	void _draw(unsigned char x, unsigned char y, Image * image) 
	{
		gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		#if !defined(NO_COLOR)
			SET_TEXT_COLOR(image->_color);
		#endif
		cputc(image->_imageData); 
	}
	
	void _delete(unsigned char x, unsigned char y)
	{
		gotoxy(x+X_OFFSET,y+Y_OFFSET);
		cputc(' ');
	}

	#if !defined(TINY_GAME)
		void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char * blink_counter) 
		{
			gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
			#if !defined(NO_COLOR)
				SET_TEXT_COLOR(image->_color);
			#endif
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

	#if !defined(TINY_GAME)
		void DRAW_VERTICAL_LINE(unsigned char x, unsigned char y, unsigned char length)
			{ 
				unsigned char i; 

				for(i=0;i<length;++i) 
				{ 
					gotoxy(X_OFFSET+x,Y_OFFSET+y+i);
					cputc(VERTICAL_BRICK);
				} 
			}
			
		void DRAW_HORIZONTAL_LINE(unsigned char x, unsigned char y, unsigned char length)
			{ 
				unsigned char i; 

				gotoxy(X_OFFSET+x,Y_OFFSET+y); 
				for(i=0;i<length;++i)
				{
					cputc('-');
				}
			}
	#endif

#endif

	

