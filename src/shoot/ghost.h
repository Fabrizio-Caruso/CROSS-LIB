/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
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

struct GhostStruct
{
    Character _character;
    
    uint8_t type;
};

typedef struct GhostStruct Ghost;

void initializeGhost(Ghost *ghostPtr, uint8_t x, uint8_t y, uint8_t status, uint8_t type, Image *imagePtr);

void spawnGhost(Ghost *ghostPtr, uint8_t ghostIndex);

void ghostDies(Ghost * ghostPtr);

void checkBombsVsGhost(register Ghost * ghostPtr);

void checkBombsVsSkull(register Character * skullPtr);

void checkBombsVsGhosts(void);

void checkBombsVsSkulls(void);

uint8_t sameLocationAsAnyGhostLocation(uint8_t x, uint8_t y, Ghost *characterList, uint8_t length);

#if !defined(TINY_GAME)
    void decreaseGhostLevel(void);
#endif

void displayGhosts(void);

void displayBombs(void);

#if defined(FULL_GAME)
    uint8_t firstAlive(void);
#endif

#define playerReachedGhosts() (sameLocationAsAnyGhostLocation(player._x, player._y, ghosts, GHOSTS_NUMBER)<GHOSTS_NUMBER)

#endif // _GHOST_H

