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

#include <conio.h>
#include <peekpoke.h>
#include <atari.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../graphics.h"

#define GRAPHICS_MODE (1+16)
	
#define CHARS_BASE 8192

// unsigned short BASE_ADDR;

void redefine(unsigned char * loc, const char *new_char)
{
	unsigned char i;
	for(i=0;i<8;++i)
	{
		POKE(loc+i,new_char[i]);
	}
}


void INIT_GRAPHICS(void)
{	
	// unsigned short BASE_ADDR;

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
	static const char invincibility[8] =    { 24, 36, 24,  0,153,  0, 36,102};	
	static const char vertical_brick[8] =   { 24, 24, 24, 48, 24, 12, 24, 24};
	static const char horizontal_brick[8] = {  0,  0,  0,255,  0,  0,  0,  0};
	
	extern char _FONT_START__[];
	unsigned char *CHBAS = (unsigned char *)0x2f4;

	// Mode 12 with no last monochromatic lines (12+16)
	_graphics(GRAPHICS_MODE);
	
	_setcolor_low(0, TGI_COLOR_RED);
	_setcolor_low(1, TGI_COLOR_WHITE);
	_setcolor_low(2, TGI_COLOR_CYAN); 	
	_setcolor_low(3, TGI_COLOR_BROWN);
	_setcolor_low(4, TGI_COLOR_BLACK);

	memcpy(_FONT_START__, (void *)0xE000, 512);
	
	/* modify your font at _FONT_START__, etc, then set the new font: */		
		
	redefine(_FONT_START__+_PLAYER_DOWN*8, player_down);
	redefine(_FONT_START__+_PLAYER_UP*8, player_up);		
	redefine(_FONT_START__+_PLAYER_RIGHT*8, player_right);
	redefine(_FONT_START__+_PLAYER_LEFT*8, player_left);

	redefine(_FONT_START__+_GHOST*8, ghost);
	redefine(_FONT_START__+_POWERUP*8, powerUp);		
	redefine(_FONT_START__+_SKULL*8, invincible_ghost);
	redefine(_FONT_START__+_BOMB*8, bomb);	
				
	redefine(_FONT_START__+_BULLET*8, missile);
	redefine(_FONT_START__+_GUN*8, gun);
	
	redefine(_FONT_START__+_HORIZONTAL_BRICK*8, horizontal_brick);
	redefine(_FONT_START__+_VERTICAL_BRICK*8, vertical_brick);
	
	#if defined(FULL_GAME)
		redefine(_FONT_START__+_LEFT_HORIZONTAL_MISSILE*8, missile_left);
		redefine(_FONT_START__+_RIGHT_HORIZONTAL_MISSILE*8, missile_right);		
		redefine(_FONT_START__+_ROCKET*8, bubble);
		redefine(_FONT_START__+_INVINCIBILITY*8, invincibility);				
	#endif
	
	*CHBAS = ((int)_FONT_START__ >> 8);  /* enable the new font */	

	// {
		// unsigned char i;

		// gotoxy(0,0);
		// for(i=0;i<=254;i++)
		// {
			// POKE(48512u+i,i);
		// }
		// cgetc();
	// }
	

}
 
 