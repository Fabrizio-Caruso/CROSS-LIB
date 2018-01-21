/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
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
 
#ifndef _CHARACTER
#define _CHARACTER

#include "display_macros.h"

struct CharacterStruct
{
	// character coordinates
	unsigned char _x;
	unsigned char _y;
	
	// _status decides whether the character is active
	unsigned char _status;
	
	Image* _imagePtr;
};

typedef struct CharacterStruct Character;


#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
#else
	#include <stdlib.h>
#endif

#include "settings.h"

#include "display_macros.h"

#define playerReached(preyPtr) sameLocationAsAnyLocation((preyPtr)->_x, (preyPtr)->_y, ghosts, GHOSTS_NUMBER)

#define  playerReachedBombs(preyPtr) sameLocationAsAnyLocation((preyPtr)->_x, (preyPtr)->_y, bombs, BOMBS_NUMBER)

#define areCharctersAtSamePosition(lhs, rhs)isCharacterAtLocation((lhs)->_x, (lhs)->_y,rhs)

extern unsigned short points;

extern unsigned char ghostCount;

#if defined(FULL_GAME)
	unsigned char playerKilledBy(Character *enemyPtr);
#endif

void displayCharacter(Character * characterPtr);

void deleteCharacter(Character * characterPtr);

#define displayPlayer(characterPtr) displayCharacter(characterPtr);
#define displayGhost(characterPtr) displayCharacter(characterPtr);
#define displayBomb(characterPtr) displayCharacter(characterPtr);
#define displayMissile(characterPtr) displayCharacter(characterPtr);
#define displayInvincibleGhost(characterPtr) displayCharacter(characterPtr);

#define deletePlayer(characterPtr) deleteCharacter(characterPtr);
#define deleteGhost(characterPtr) deleteCharacter(characterPtr);
#define deleteBomb(characterPtr) deleteCharacter(characterPtr);
#define deleteMissile(characterPtr) deleteCharacter(characterPtr);
#define deleteInvincibleGhost(characterPtr) deleteCharacter(characterPtr);
#define deletePowerUp(characterPtr) deleteCharacter(characterPtr);

void initializeCharacter(Character* characterPtr, unsigned char x, unsigned char y, unsigned char status, Image * imagePtr);

void setCharacterPosition(Character* characterPtr, unsigned char x, unsigned char y);

unsigned char isCharacterAtLocation(unsigned char x, unsigned char y, Character * characterPtr);

unsigned char wallReached(Character *characterPtr);

void die(Character * playerPtr);

void playerDies(void);

void checkBombsVsGhost(Character * ghostPtr);
						
void checkBombsVsGhosts(void);
						
unsigned char safeLocation(unsigned char x, unsigned char y, Character *danger, unsigned char dangerSize);

void relocateCharacter(Character * characterPtr, Character *danger, unsigned char dangerSize);

void ghostDies(Character * ghostPtr);

unsigned char sameLocationAsAnyLocation(unsigned char x, unsigned char y, Character *characterList, unsigned char length);

#if defined(FULL_GAME)
	unsigned char innerWallReached(Character *characterPtr);
	unsigned char horizontalWallsReached(Character *characterPtr);
	unsigned char nearInnerWall(Character *characterPtr);
#endif

#endif // _CHARACTER
