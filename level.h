#ifndef _LEVEL
#define _LEVEL


#include "character.h"

void drawInnerVerticalWall(void);

unsigned short drawInnerVerticalWallForLevel(void);

void fillLevelWithCharacters(
						  Character * playerPtr, Character * powerUpPtr, 
						  Character **ghosts,
						  Character **bombs,
						  Character * invincibleGhostPtr, 
						  Character * missilePtr, Character * gunPtr);

						  
#endif // _LEVEL