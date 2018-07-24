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

#include "../../../graphics_settings.h"

#include "../../../graphics_data/8x8_chars.h"

#define GRAPHICS_MODE (1+16)

#include "../../../redefine_characters/udg_map.h"	

void set_udg(void)
{	

	unsigned char i;
	
	extern char _FONT_START__[];
	unsigned char *CHBAS = (unsigned char *)0x2f4;

	memcpy(_FONT_START__, (void *)0xE000, 512);
	
	/* modify your font at _FONT_START__, etc, then set the new font: */		

	
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i)
	{
		memcpy(_FONT_START__ + (redefine_map[i].ascii)*8, redefine_map[i].bitmap, 8);
	}
	
	*CHBAS = ((int)_FONT_START__ >> 8);  /* enable the new font */		
}
 
void INIT_GRAPHICS(void)
{
	// Mode 12 with no last monochromatic lines (12+16)
	_graphics(GRAPHICS_MODE);
	
	_setcolor_low(0, TGI_COLOR_RED);
	_setcolor_low(1, TGI_COLOR_WHITE);
	_setcolor_low(2, TGI_COLOR_CYAN); 	
	_setcolor_low(3, TGI_COLOR_BROWN);
	_setcolor_low(4, TGI_COLOR_BLACK);

	set_udg();
}


