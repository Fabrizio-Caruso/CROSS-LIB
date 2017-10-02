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
#include <conio.h>
#include "cpcrslib.h"

// #define _DRAW 	cputc(image->_imageData);
// #define _DELETE cputc(' '); 

extern unsigned char XSize;

// #define CPC_BLUE 2
// #define CPC_RED 4
// #define CPC_YELLOW 0
// #define CPC_CYAN 3


#define CPC_RED 3
#define CPC_CYAN 1
#define CPC_BLUE 4
#define CPC_YELLOW 2 


#include "../display_macros.h"

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;

#if defined(FULL_GAME)
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image BUBBLE_IMAGE;

	extern Image EXTRA_POINTS_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;	
	
	extern Image BROKEN_WALL_IMAGE;
#endif

#if defined(REDEFINED_CHARS)
Image PLAYER_DOWN;
Image PLAYER_UP;
Image PLAYER_RIGHT;
Image PLAYER_LEFT;
#endif

char space_str[2] = {' ', '\0'};
char vertical_brick_str[2] = {'|', '\0' };
char horizontal_brick_str[2] = {'-', '\0'};
char broken_wall_str[2] = {'X', '\0'};

char char_list[13*2] = 
{ 
'*', '\0', 
'o', '\0', 
'x', '\0', 
'+', '\0', 
'.', '\0', 
's', '\0', 
'!', '\0', 
'$', '\0', 
'v', '\0', 
'*', '\0', 
'>', '\0', 
'<', '\0', 
'^', '\0'
};

#define _PLAYER 0
#define _GHOST 2
#define _BOMB 4
#define _INVINCIBLE_GHOST 6
#define _MISSILE 8
#define _POWERUP 10
#define _GUN 12
#define _EXTRA_POINTS 14
#define _INVINCIBILITY 16
#define _EXTRA_LIFE 18
#define _LEFT_MISSILE 20
#define _RIGHT_MISSILE 22
#define _BUBBLE 24

void INIT_GRAPHICS(void)
{
    //cpc_DisableFirmware();          //Now, I don't gonna use any firmware routine so I modify interrupts jump to nothing
    // cpc_ClrScr();                           //fills scr with ink 0
    // cpc_SetMode(1);                         //hardware call to set mode 1	
	
	// WAIT_PRESS();
	// cpc_ClrScr();	
	

	// draw(172,0,172,40);	
	//cpc_DisableFirmware();
	// cpc_SetColour(16,20); //background
	// cpc_SetColour(0,20); //border
	// cpc_SetColour(1,10); //
	
	// cpc_SetMode(0);
	// cpc_SetInkGphStr(0,0);
	// cpc_SetInkGphStr(2,2);
	// cpc_SetInkGphStr(1,8);
	
	// cpc_PrintGphStrXY("GAME;OF;LIFE",50,10);
	// cpc_PrintGphStrXY("ARTABURU;2009",49,19);
	
	// cpc_EnableFirmware();
	
	// WAIT_PRESS();
}

void INIT_IMAGES(void)
{		
	PLAYER_IMAGE._color = CPC_YELLOW;
	INVINCIBLE_GHOST_IMAGE._color = CPC_YELLOW;
	POWERUP_IMAGE._color = CPC_YELLOW;
	GUN_IMAGE._color = CPC_YELLOW;
	BOMB_IMAGE._color = CPC_RED;
	DEAD_GHOST_IMAGE._color = CPC_RED;
		
	GHOST_IMAGE._imageData = _GHOST;
	INVINCIBLE_GHOST_IMAGE._imageData = _INVINCIBLE_GHOST;
	BOMB_IMAGE._imageData = _BOMB;
	
	PLAYER_IMAGE._imageData = _PLAYER;
	
	#if defined(REDEFINED_CHARS)
		PLAYER_DOWN._imageData = _PLAYER;
		PLAYER_UP._imageData = _PLAYER;
		PLAYER_RIGHT._imageData = _PLAYER;
		PLAYER_LEFT._imageData = _PLAYER;
		PLAYER_DOWN._color = PLAYER_IMAGE._color;
		PLAYER_UP._color = PLAYER_IMAGE._color;	
		PLAYER_RIGHT._color = PLAYER_IMAGE._color;
		PLAYER_LEFT._color = PLAYER_IMAGE._color;
	#endif
	
	POWERUP_IMAGE._imageData = _POWERUP;
	GUN_IMAGE._imageData = _GUN;
	MISSILE_IMAGE._imageData = _MISSILE;

	DEAD_GHOST_IMAGE._imageData = _GHOST;

	GHOST_IMAGE._color = CPC_CYAN;
	MISSILE_IMAGE._color = CPC_CYAN;

	#if defined(FULL_GAME)
		LEFT_ENEMY_MISSILE_IMAGE._imageData = _LEFT_MISSILE;
		LEFT_ENEMY_MISSILE_IMAGE._color = CPC_CYAN;
		RIGHT_ENEMY_MISSILE_IMAGE._imageData = _RIGHT_MISSILE;
		RIGHT_ENEMY_MISSILE_IMAGE._color = CPC_CYAN;	
		
		BUBBLE_IMAGE._imageData = _BUBBLE;
		BUBBLE_IMAGE._color = CPC_CYAN;
		
		EXTRA_POINTS_IMAGE._imageData = _EXTRA_POINTS;
		
		EXTRA_LIFE_IMAGE._imageData = PLAYER_IMAGE._imageData;
		INVINCIBILITY_IMAGE._imageData = _INVINCIBILITY;
		
		EXTRA_POINTS_IMAGE._color = CPC_YELLOW;
		EXTRA_LIFE_IMAGE._color = CPC_YELLOW;
		INVINCIBILITY_IMAGE._color = CPC_YELLOW;			
	#endif
}

#if defined(FULL_GAME)
	void DRAW_BROKEN_WALL(unsigned char x, unsigned char y)
	{
		cpc_PrintGphStrStdXY(CPC_BLUE,broken_wall_str,(x+X_OFFSET)*2,(y+Y_OFFSET)*8);			
	}
#endif
	
void _draw(unsigned char x, unsigned char y, Image * image) 
{
    cpc_PrintGphStrStdXY(image->_color,char_list+image->_imageData,(x+X_OFFSET)*2,(y+Y_OFFSET)*8);	
}

void _delete(unsigned char x, unsigned char y)
{	
    cpc_PrintGphStrStdXY(CPC_BLUE,space_str,(x+X_OFFSET)*2,(y+Y_OFFSET)*8);	
}

void _blink_draw(unsigned char x, unsigned char y, Image * image, unsigned char *blinkCounter) 
{
	char str[2];	

	if(*blinkCounter) 
	{
		cpc_PrintGphStrStdXY(image->_color,char_list+image->_imageData,(x+X_OFFSET)*2,(y+Y_OFFSET)*8);	
		*blinkCounter=0;
	} 
	else 
	{
		cpc_PrintGphStrStdXY(1,space_str,(x+X_OFFSET)*2,(y+Y_OFFSET)*8);
		*blinkCounter=1;
	}	
}

void DRAW_VERTICAL_LINE(unsigned char x,unsigned char y, unsigned char length)
{ 
	unsigned char i;

	for(i=0;i<length;++i)
	{
		cpc_PrintGphStrStdXY(CPC_CYAN,vertical_brick_str,(x+X_OFFSET)*2,(y+i+Y_OFFSET)*8);			
	}	
}

void DRAW_HORIZONTAL_LINE(unsigned char x,unsigned char y, unsigned char length)
{
	unsigned char i;

	for(i=0;i<length;++i)
	{
		cpc_PrintGphStrStdXY(CPC_CYAN,horizontal_brick_str,(x+i+X_OFFSET)*2,(y+Y_OFFSET)*8);			
	}
}




