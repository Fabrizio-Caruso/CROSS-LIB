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
 
#ifndef _STRATEGY
#define _STRATEGY


#define X_MOVE 0
#define Y_MOVE 1

#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
#else
	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>
#endif

#include "character.h"

unsigned char move(Character* hunterPtr, Character* preyPtr, unsigned char offset);

void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr);
					
void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr);

#if defined(FULL_GAME)
	void moveTowardCharacter(Character *preyPtr, Character *hunterPtr, unsigned char strategy);
#elif !defined(TINY_GAME)
	void moveTowardCharacter(Character *hunterPtr, unsigned char strategy);
#else	
	void moveTowardCharacter(Character *hunterPtr);
#endif


void computeStrategy(void);

#if !defined(FULL_GAME)
	void chaseCharacter(unsigned short slowDown);
#else
	void chaseCharacter(Character *preyPtr, unsigned short slowDown);	
#endif
	
#endif // _STRATEGY

