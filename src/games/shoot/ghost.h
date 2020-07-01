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

void spawnGhost(Character *ghostPtr, uint8_t ghostIndex);

void ghostDies(Character * ghostPtr);

void checkBombsVsGhost(register Character * ghostPtr);

void checkBombsVsSkull(register Character * skullPtr);

void checkBombsVsGhosts(void);

void checkBombsVsSkulls(void);

uint8_t sameLocationAsAnyGhostLocation(uint8_t x, uint8_t y, Character *characterList, uint8_t length);

void decreaseGhostLevel(void);

void displayGhosts(void);

void displayBombs(void);

// uint8_t firstAlive(void);

#define playerReachedGhosts() (sameLocationAsAnyGhostLocation(player._x, player._y, ghosts, ghostsOnScreen)<ghostsOnScreen)

#endif // _GHOST_H

