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

#include "character.h"

// Required by horizontal missile
#if defined(FULL_GAME) 
	unsigned char moveCharacter(Character* hunterPtr, Character* preyPtr, unsigned char offset);
#else
	unsigned char moveCharacter(Character* hunterPtr, unsigned char offset);
#endif

#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
	void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr);
	void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr);
#endif

#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
	void moveTowardCharacter(Character *preyPtr, Character *hunterPtr, unsigned char strategy);
#elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
	void moveTowardCharacter(Character *preyPtr, Character *hunterPtr);
#else	
	void moveTowardCharacter(Character *hunterPtr);
#endif

#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
	void computeStrategy(void);
#endif

#if defined(FULL_GAME)
	void chaseCharacter(Character *preyPtr, unsigned short slowDown);	
#else
	void chaseCharacter(unsigned short slowDown);	
#endif
	
#endif // _STRATEGY

