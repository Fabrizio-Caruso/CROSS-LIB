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
 
#ifndef _GHOST_H
#define _GHOST_H

#include "character.h"

uint16_t computeGhostSlowDown(void);

#if !defined(TINY_GAME)
	void decreaseGhostLevel(void);
#endif

#if !defined(TINY_GAME)
void displayBombs(void);
#else
    #define displayBombs() \
    do \
    { \
        uint8_t i; \
        \
        for(i=0;i<BOMBS_NUMBER;++i) \
        { \
            displayBomb(&bombs[i]); \
        } \
    } while(0)
#endif

#if defined(FULL_GAME)
	uint8_t firstAlive(void);
#endif

#endif // _GHOST_H

