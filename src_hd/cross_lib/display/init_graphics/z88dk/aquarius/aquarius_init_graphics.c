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

#define _AQUARIUS_RED 16
#define _AQUARIUS_WHITE (16+32+64)
#define _AQUARIUS_CYAN (16+128)

#include "memory_mapped_graphics.h"
#include "display_macros.h"

#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(uint8_t*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))


void _XL_INIT_GRAPHICS(void)
{
	uint8_t i;
	
	POKE(COLOR_ADDR,7);
	
	for(i=0;i<251;++i)
	{
		POKE((uint16_t) (COLOR_ADDR+1+i),128);
		POKE((uint16_t) (COLOR_ADDR+250+i),128);
		POKE((uint16_t) (COLOR_ADDR+500+i),128);
		POKE((uint16_t) (COLOR_ADDR+749+i),128);
	}

    _setScreenColors();
}

