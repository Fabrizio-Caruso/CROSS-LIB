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

#if defined(FULL_GAME) && !defined(NO_BLINKING)
	unsigned char playerBlink;	
#endif

#if !defined(REDEFINED_CHARS)
	Image PLAYER_IMAGE;
#endif

#if defined(NO_MESSAGE)
	#include "text.h"
	
	extern short highScore;
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
#elif defined(__ENTERPRISE__)
	//
#elif defined(__SUPERVISION__)
	//
#elif defined(__EG2K__) || defined(__TRS80__)
	//
#elif defined(Z88DK_SPRITES)
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
		#elif defined(__SMS__)
		
			unsigned char pal1[] = {0x2A, 0x3F, 0x0F, 0x00, 0x01, 0x02, 0x03, 0x17,
									0x15, 0x35, 0x1D, 0x3D, 0x17, 0x37, 0x1F, 0x03};
			unsigned char pal2[] = {0x2A, 0x3F, 0x0F, 0x00, 0x10, 0x20, 0x30, 0x35,
									0x15, 0x35, 0x1D, 0x3D, 0x17, 0x10, 0x20, 0x03};			
			clear_vram();
			load_tiles(standard_font, 0, 255, 1);
			load_palette(pal1, 0, 16);
			load_palette(pal2, 16, 16);
			set_vdp_reg(VDP_REG_FLAGS1, VDP_REG_FLAGS1_BIT7 | VDP_REG_FLAGS1_SCREEN);

			// #define VDP_REG_HBL_COUNTER 0x8A
			// load_palette(pal1, 0, 16); 
			// load_palette(pal2, 16, 16); 
			// set_vdp_reg(VDP_REG_FLAGS1, VDP_REG_FLAGS1_BIT7 | VDP_REG_FLAGS1_SCREEN); 							
		#elif defined(__GCC__)
            #include <ncurses.h>
			
			noecho();
			cbreak();
		#endif	
	}
	
	void INIT_IMAGES(void)
	{		
		#if !defined(NO_COLOR)
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

		#if !defined(NO_COLOR)
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
		gotoxy((x+X_OFFSET),(y+Y_OFFSET));
		cputc(' ');
	}

	#if !defined(TINY_GAME) && !defined(NO_BLINKING)
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
			
		#if !defined(NO_BORDERS)
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

	#if defined(NO_MESSAGE)
		void highScoreScreen(void)
		{
			gotoxy(1,0); printf("05u0", highScore);
		}
	#endif	
	

//
	# if defined(Z88DK_PUTC4X6)
		void PRINT(unsigned char x, unsigned char y, char * str)
		{
			unsigned char i = 0;
			while(str[i]!='\0')
			{
				gotoxy(x+i,y);
				cputc(str[i++]);
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
				gotoxy(x+i,y);
				cputc((digits[5-i])+48);
			}
		}	

		void print_02u(unsigned char x, unsigned char y, unsigned short val)
		{
			gotoxy(x,y);
			cputc(((unsigned char) val)/10+48);
			gotoxy((x+1),y);
			cputc(((unsigned char) val)%10+48);
		}	


		void print_u(unsigned char x, unsigned char y, unsigned short val)
		{
			gotoxy(x,y);
			cputc(val+48);
			// POKE(BASE_ADDR+x+y*((unsigned short)XSize), (unsigned char) (val+48));
		}

		void print_level(unsigned short val)
		{
			PRINT(XSize/2-4,YSize/2,"level");
			print_u(XSize/2+2, YSize/2, val);
		}
		
		void print_score(x,y,str,val)
		{
			print_05u0(XSize/2+2, YSize/2, val);
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
			else if(str[0]=='B')
			{
				print_score(x,y,"BONUS:",val);
			}
			else if(str[0]=='b')
			{
				print_score(x,y,"bonus:",val);
			}	
			else if(str[0]=='S')
			{
				print_score(x,y,"SCORE:",val);
			}
			else if(str[0]=='s')
			{
				print_score(x,y,"score:",val);
			}		
			else if(str[0]=='H')
			{
				print_score(x,y,"HIGH SCORE:",val);
			}
			else if(str[0]=='h')
			{
				print_score(x,y,"high score:",val);
			}		
			else
			{
				print_level(val);
			}
		}
	#endif
//	
	
	
#endif



