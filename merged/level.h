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
 
#ifndef _LEVEL
#define _LEVEL


#include "character.h"

void fillLevelWithCharacters(unsigned char nGhosts);

#if defined(FULL_GAME)
	void updateInnerWallVerticalData(void);

	unsigned char rocketLevel(void);

	unsigned char missileLevel(void);	

	unsigned char bossLevel(void);
	
	void initializeAwayFromWall(Character * characterPtr, unsigned char x, unsigned char y, unsigned char status, Image *imagePtr);

#endif 



#endif // _LEVEL
