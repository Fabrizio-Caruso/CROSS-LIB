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
 
#ifndef _DISPLAY_MACROS
#define _DISPLAY_MACROS

#include <conio.h>
#include "settings.h"

#include "patch/generic_conio_patch.h"
#if defined(__ATMOS__)
	#include "atmos/atmos_conio_patch.h"
#endif

// #if defined(__C64__)
	// #include "c64/c64_hardware_sprite.h"
// #endif



struct ImageStruct
{
	unsigned char _imageData;
	unsigned char _color;
};

typedef struct ImageStruct Image;

//
 
#if defined(__ATMOS__)
	#define X_OFFSET 2
	#define Y_OFFSET 3
#elif defined(__VIC20__) 
	#define X_OFFSET 0
	#define Y_OFFSET 2
#elif defined(__ATARI__) || defined(__ATARIXL__)
	#define X_OFFSET 0
	#define Y_OFFSET 0
#else
	#define X_OFFSET 0
	#define Y_OFFSET 3
#endif

#if defined(__NES__)
	#define GET_SCREEN_SIZE(x,y) {*x=32; *y=40;};
#elif  defined(__ATARI__) || defined(__ATARIXL__)
	#define GET_SCREEN_SIZE(x,y) {*x=20; *y=24;};
#else
	#define GET_SCREEN_SIZE(x,y) {screensize(x,y); *x-=X_OFFSET; *y-=Y_OFFSET;};
#endif

#if defined(__ATMOS__)
	#include<peekpoke.h>
	
	char powerUp_blink = 1;
	char gun_blink = 1;
	char extra_points_blink = 1;
	
	extern Image PLAYER_LEFT;
	extern Image PLAYER_RIGHT;
	extern Image PLAYER_UP;
	extern Image PLAYER_DOWN;
	
	#define DRAW_BROKEN_WALL(x,y) {gotoxy(x+X_OFFSET,(y+Y_OFFSET)); cputc('X' + 128);};
	
	void DRAW_PLAYER(char x, char y, Image * image) 
	{
		gotoxy(x+X_OFFSET,(y+Y_OFFSET)); 
		
		POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, image->_imageData + image->_color);
	};
		
	#define DRAW_GHOST(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, image->_imageData + image->_color);};
	
	#define DRAW_INVINCIBLE_GHOST(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, image->_imageData + image->_color);};

	#define DRAW_BOMB(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, image->_imageData + image->_color);};

	// #define DRAW_POWERUP(x,y,image) {gotoxy((x+X_OFFSET,(y+Y_OFFSET)); cputc(image->_imageData + image->_color);};
	void DRAW_POWERUP(char x, char y, Image * image) 
	{
		//gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		if(powerUp_blink) 
		{
			POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40,image->_imageData + image->_color );
			//cputc(image->_imageData + image->_color); 
			powerUp_blink=0;
		} 
		else 
		{
			POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32); 
			powerUp_blink=1;
		}
	};
	
	
	// #define DRAW_GUN(x,y,image) {gotoxy((x+X_OFFSET,(y+Y_OFFSET)); cputc(image->_imageData + image->_color);};
	void DRAW_GUN(char x, char y, Image * image) 
	{
		//gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		if(gun_blink) 
		{
			POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40,image->_imageData + image->_color );
			//cputc(image->_imageData + image->_color); 
			gun_blink=0;
		} 
		else 
		{
			POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);
			gun_blink=1;
		}
	};
	
	// #define DRAW_GUN(x,y,image) {gotoxy((x+X_OFFSET,(y+Y_OFFSET)); cputc(image->_imageData + image->_color);};
	void DRAW_EXTRA_POINTS(char x, char y, Image * image) 
	{
		//gotoxy((x+X_OFFSET),(y+Y_OFFSET)); 
		if(extra_points_blink) 
		{
			POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40,image->_imageData + image->_color );
			//cputc(image->_imageData + image->_color); 
			extra_points_blink=0;
		} 
		else 
		{
			POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);
			extra_points_blink=1;
		}
	};	
	
	#define DRAW_MISSILE(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, image->_imageData + image->_color);};

	#define DRAW_BOMBS() \
	{ \
		unsigned char i; \
		for(i=0;i<BOMBS_NUMBER;++i) \
		{ \
			DRAW_BOMB(bombs[i]->_x, bombs[i]->_y, bombs[i]->_imagePtr); \
		} \
	}

	#define DELETE_PLAYER(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);};

	#define DELETE_GHOST(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);};

	#define DELETE_INVINCIBLE_GHOST(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);};

	#define DELETE_BOMB(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);};

	#define DELETE_POWERUP(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);};

	#define DELETE_GUN(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);};

	#define DELETE_MISSILE(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);};
	
	#define DELETE_EXTRA_POINTS(x,y,image) {POKE(0xBB80+(x+X_OFFSET)+(y+Y_OFFSET)*40, 32);};
	
	#define PRINT(x,y,str) {gotoxy(x+X_OFFSET,y+Y_OFFSET); cputs(str); };

	#define PRINTF(x,y,...) {gotoxy(x+X_OFFSET,y+Y_OFFSET); cprintf(##__VA_ARGS__); };

	#define DRAW_BORDERS() \
	{ \
		unsigned char i; \
		gotoxy(0+X_OFFSET,0+Y_OFFSET); \
		cputc (CH_ULCORNER+128); \
		for(i=0;i<38-1;++i) \
		{ \
			cputc('-' + 128); \
		} \
		gotoxy(38-1+X_OFFSET,0+Y_OFFSET); \
		cputc (CH_URCORNER+128); \
		for(i=0;i<28-X_OFFSET-Y_OFFSET;++i) \
		{ \
			gotoxy(0+X_OFFSET,1+i+Y_OFFSET); \
			cputc('|'+128); \
		} \
		gotoxy(0+X_OFFSET,28-1); \
		cputc (CH_LLCORNER+128); \
		for(i=0;i<38-1;++i) \
		{ \
			cputc('-' + 128); \
		} \
		gotoxy(38-1+X_OFFSET, 28-1); \
		cputc (CH_LRCORNER+128); \
		for(i=0;i<28-X_OFFSET-Y_OFFSET;++i) \
		{ \
			gotoxy(38-1+X_OFFSET,1+i+Y_OFFSET); \
			cputc('|'+128); \
		} \
	} 

	
	#define DRAW_VERTICAL_LINE(x,y,length) \
	{ \
		unsigned char i; \
		for(i=0;i<length;++i) \
		{ \
			POKE(0xBB80+(x+X_OFFSET)+(y+i+Y_OFFSET)*40,'|'+128); \
		} \
	}
			
	#define SHOW_LEFT() {player._imagePtr = &PLAYER_LEFT; }
	#define SHOW_RIGHT() {player._imagePtr = &PLAYER_RIGHT; }
	#define SHOW_UP() {player._imagePtr = &PLAYER_UP; }
	#define SHOW_DOWN() {player._imagePtr = &PLAYER_DOWN; }
#elif defined(__ATARI__) || defined(__ATARIXL__)
	
	#define DRAW_BROKEN_WALL(x,y) {gotoxy((x+X_OFFSET),(y+Y_OFFSET)); cputc('X');};

	#define DRAW_PLAYER(x,y,image)  {_draw(x,y,image);};
	
	#define DRAW_GHOST(x,y,image)  {_draw(x,y,image);};
	
	#define DRAW_INVINCIBLE_GHOST(x,y,image) {_draw(x,y,image);};
	
	#define DRAW_BOMB(x,y,image)  {_draw(x,y,image);};
	
	#define DRAW_POWERUP(x,y,image) {_blink_powerUp_draw(x,y,image);};
	
	#define DRAW_GUN(x,y,image) {_blink_gun_draw(x,y,image);};

	#define DRAW_EXTRA_POINTS(x,y,image) {_blink_extra_points_draw(x,y,image);};
	
	void _draw(char x, char y, Image * image);
	void _blink_powerUp_draw(char x, char y, Image * image);
	void _blink_gun_draw(char x, char y, Image * image);
	void _blink_extra_points_draw(char x, char y, Image * image);	
	
	#define DRAW_MISSILE(x,y,image)  {_draw(x,y,image);};
	
	#define DRAW_BOMBS() \
	{ \
		unsigned char i = 0; \
		for(;i<BOMBS_NUMBER;++i) \
		{ \
			DRAW_BOMB(bombs[i]->_x, bombs[i]->_y, bombs[i]->_imagePtr); \
		} \
	}

	#define DELETE_PLAYER(x,y,image) {_delete(x,y);};

	#define DELETE_GHOST(x,y,image)  {_delete(x,y);};

	#define DELETE_INVINCIBLE_GHOST(x,y,image)  {_delete(x,y);};

	#define DELETE_BOMB(x,y,image)  {_delete(x,y);};

	#define DELETE_POWERUP(x,y,image)  {_delete(x,y);};

	#define DELETE_GUN(x,y,image)  {_delete(x,y);};
	
	#define DELETE_EXTRA_POINTS(x,y,image)  {_delete(x,y);};
	
	#define DELETE_MISSILE(x,y,image) {_delete(x,y);};

	void _delete(char x,char y);

	#define PRINT(x,y,str) \
	{ \
		if((y+Y_OFFSET)%2==1) \
		{ \
			gotoxy(x+20+X_OFFSET,(y+Y_OFFSET)/2); \
		} \
		else \
		{ \
			gotoxy(x+X_OFFSET, (y+Y_OFFSET)/2); \
		} \
		cputs(str); \
	};

	#define PRINTF(x,y,...)  \
	{ \
		if((y+Y_OFFSET)%2==1) \
		{ \
			gotoxy(x+20+X_OFFSET,(y+Y_OFFSET)/2); \
		} \
		else \
		{ \
			gotoxy(x+X_OFFSET, (y+Y_OFFSET)/2); \
		} \
		cprintf(##__VA_ARGS__); \
	};
	//{gotoxy(x+X_OFFSET,y+Y_OFFSET); cprintf(##__VA_ARGS__); };


	//{(void) textcolor (COLOR_WHITE);cvlinexy (x+X_OFFSET,y+Y_OFFSET,length);};
	#define DRAW_VERTICAL_LINE(x,y,length) \
	{ \
		unsigned char i; \
		for(i=0;i<length;i++) \
		{ \
			if((y+Y_OFFSET+i)%2==1) \
			{ \
				gotoxy(x+20+X_OFFSET,(y+Y_OFFSET+i)/2); \
			} \
			else \
			{ \
				gotoxy(x+X_OFFSET,(y+Y_OFFSET+i)/2); \
			} \
			cputc('a'+20+12); \
		} \
	}
	
	#define DRAW_HORIZONTAL_LINE(x,y,length) \
	{ \
		unsigned char i; \
		if((y+Y_OFFSET)%2==1) \
		{ \
			for(i=0;i<length;++i) \
			{ \
				gotoxy(x+20+X_OFFSET+i,(y+Y_OFFSET)/2); \
				cputc('-'+64+32); \
			} \
		} \
		else \
		{ \
			for(i=0;i<length;++i) \
			{ \
				gotoxy(x+X_OFFSET+i,(y+Y_OFFSET)/2); \
				cputc('-'+64+32); \
			} \
		} \
	}

	#define DRAW_BORDERS()\
	{ \
		SET_TEXT_COLOR(TEXT_COLOR); \
		gotoxy(0+X_OFFSET,0+Y_OFFSET); \
		//cputc (CH_ULCORNER);\
		cputc ('X');\
		DRAW_HORIZONTAL_LINE (1+X_OFFSET,0+Y_OFFSET, XSize-2);\
		//cputc (CH_URCORNER);\
		cputc ('X');\
		DRAW_VERTICAL_LINE(0+X_OFFSET, 1+Y_OFFSET, YSize - 2);\
		//cputc (CH_LLCORNER);\
		gotoxy(0+20,(YSize-1)/2); \
		cputc ('X'); \
		DRAW_HORIZONTAL_LINE (1+X_OFFSET,YSize-1,XSize-2);\
		//cputc (CH_LRCORNER);\
		cputc ('X');\
		DRAW_VERTICAL_LINE(XSize - 1, 1+Y_OFFSET, YSize - 2); \
	}

	
	
	#if defined(__C16__) || defined(__PLUS4__) || defined(__C64__)
		#define SHOW_LEFT() {player._imagePtr = &PLAYER_LEFT; }
		#define SHOW_RIGHT() {player._imagePtr = &PLAYER_RIGHT; }
		#define SHOW_UP() {player._imagePtr = &PLAYER_UP; }
		#define SHOW_DOWN() {player._imagePtr = &PLAYER_DOWN; }
	#else
		#define SHOW_LEFT() {}
		#define SHOW_RIGHT() {}
		#define SHOW_UP() {}
		#define SHOW_DOWN() {}	
	#endif
#else
	#if defined(__C16__) || defined(__PLUS4__) || defined(__C64__)
		extern Image PLAYER_LEFT;
		extern Image PLAYER_RIGHT;
		extern Image PLAYER_UP;
		extern Image PLAYER_DOWN;
	#endif

	
	#define DRAW_BROKEN_WALL(x,y) {gotoxy((x+X_OFFSET),(y+Y_OFFSET)); cputc('X');};

	#define DRAW_PLAYER(x,y,image)  {_draw(x,y,image);};
	
	#define DRAW_GHOST(x,y,image)  {_draw(x,y,image);};
	
	#define DRAW_INVINCIBLE_GHOST(x,y,image) {_draw(x,y,image);};
	
	#define DRAW_BOMB(x,y,image)  {_draw(x,y,image);};
	
	#define DRAW_POWERUP(x,y,image) {_blink_powerUp_draw(x,y,image);};
	
	#define DRAW_GUN(x,y,image) {_blink_gun_draw(x,y,image);};

	#define DRAW_EXTRA_POINTS(x,y,image) {_blink_extra_points_draw(x,y,image);};
	
	void _draw(char x, char y, Image * image);
	void _blink_powerUp_draw(char x, char y, Image * image);
	void _blink_gun_draw(char x, char y, Image * image);
	void _blink_extra_points_draw(char x, char y, Image * image);	
	
	#define DRAW_MISSILE(x,y,image)  {_draw(x,y,image);};
	
	#define DRAW_BOMBS() \
	{ \
		unsigned char i = 0; \
		for(;i<BOMBS_NUMBER;++i) \
		{ \
			DRAW_BOMB(bombs[i]->_x, bombs[i]->_y, bombs[i]->_imagePtr); \
		} \
	}

	#define DELETE_PLAYER(x,y,image) {_delete(x,y);};

	#define DELETE_GHOST(x,y,image)  {_delete(x,y);};

	#define DELETE_INVINCIBLE_GHOST(x,y,image)  {_delete(x,y);};

	#define DELETE_BOMB(x,y,image)  {_delete(x,y);};

	#define DELETE_POWERUP(x,y,image)  {_delete(x,y);};

	#define DELETE_GUN(x,y,image)  {_delete(x,y);};
	
	#define DELETE_EXTRA_POINTS(x,y,image)  {_delete(x,y);};
	
	#define DELETE_MISSILE(x,y,image) {_delete(x,y);};

	void _delete(char x,char y);

	#define PRINT(x,y,str) {gotoxy(x+X_OFFSET,y+Y_OFFSET); cputs(str); };

	#define PRINTF(x,y,...) {gotoxy(x+X_OFFSET,y+Y_OFFSET); cprintf(##__VA_ARGS__); };

	#define DRAW_BORDERS()\
	{ \
		SET_TEXT_COLOR(TEXT_COLOR); \
		gotoxy(0+X_OFFSET,0+Y_OFFSET); \
		cputc (CH_ULCORNER);\
		chline (XSize-2);\
		cputc (CH_URCORNER);\
		cvlinexy (0+X_OFFSET, 1+Y_OFFSET, YSize - 2);\
		cputc (CH_LLCORNER);\
		chline (XSize-2);\
		cputc (CH_LRCORNER);\
		cvlinexy (XSize - 1, 1+Y_OFFSET, YSize - 2); \
	}

	#define DRAW_VERTICAL_LINE(x,y,length) {(void) textcolor (COLOR_WHITE);cvlinexy (x+X_OFFSET,y+Y_OFFSET,length);};

	#if defined(__C16__) || defined(__PLUS4__) || defined(__C64__)
		#define SHOW_LEFT() {player._imagePtr = &PLAYER_LEFT; }
		#define SHOW_RIGHT() {player._imagePtr = &PLAYER_RIGHT; }
		#define SHOW_UP() {player._imagePtr = &PLAYER_UP; }
		#define SHOW_DOWN() {player._imagePtr = &PLAYER_DOWN; }
	#else
		#define SHOW_LEFT() {}
		#define SHOW_RIGHT() {}
		#define SHOW_UP() {}
		#define SHOW_DOWN() {}	
	#endif
#endif

#define CLEAR_SCREEN() clrscr();


	
#define SET_TEXT_COLOR(c) (void) textcolor (c);

#define SET_BORDER_COLOR(c) (void) bordercolor (c);

#define SET_BACKGROUND_COLOR(c) (void) bgcolor (c);

void INIT_IMAGES(void);

void INIT_GRAPHICS(void);
	
#endif // _DISPLAY_MACROS