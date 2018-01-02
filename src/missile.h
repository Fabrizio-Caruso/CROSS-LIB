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
 
#ifndef _MISSILE
#define _MISSILE

#include "character.h"
#include "settings.h"

void checkMissileVsGhost(Character * missilePtr,
					     Character * ghostPtr);
	
void checkMissileVsGhosts(Character * missilePtr);
	
unsigned char setMissileInitialPosition(Character *missilePtr, Character *playerPtr,
							  unsigned short missileDirection);
	
void _moveMissile(Character * missilePtr, unsigned short missileDirection);
	
void moveMissile(Character * missilePtr, unsigned short missileDirection);

void restoreMissile(Character *missilePtr);

#if !defined(TINY_GAME)
	void reducePowerUpsCoolDowns(void);
#endif
	
#endif //_MISSILE

