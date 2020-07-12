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
 
#ifndef _STRATEGY
#define _STRATEGY


#define X_MOVE 0
#define Y_MOVE 1


#include "character.h"

uint8_t innerWallBetween(Character *hunterPtr);

// Required by horizontal missile
uint8_t moveCharacter(uint8_t *hunterOffsetPtr, uint8_t *preyOffsetPtr);

void blindChaseCharacterXStrategy(Character* hunterPtr);
void blindChaseCharacterYStrategy(Character* hunterPtr);


void verticalWallMoveTowardCharacter(register Character *hunterPtr, uint8_t strategy);
void horizontalWallMoveTowardCharacter(register Character *hunterPtr, uint8_t strategy);


void skullMoveTowardCharacter(Character *hunterPtr, uint8_t strategy);


#if !defined(SIMPLE_STRATEGY)
    void computeStrategy(void);
#endif


void chaseCharacter(void);    

    
#endif // _STRATEGY

