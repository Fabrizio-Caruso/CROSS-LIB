#include "character.h"

void drawInnerVerticalWall(void);

unsigned short drawInnerVerticalWallForLevel(void);

void fillLevelWithCharacters(
						  Character * playerPtr, Character * powerUpPtr, 
						  Character * ghostPtr1, Character * ghostPtr2,
						  Character * ghostPtr3, Character * ghostPtr4,
						  Character * ghostPtr5, Character * ghostPtr6,
						  Character * ghostPtr7, Character * ghostPtr8,
						  Character * bombPtr1, Character * bombPtr2,
						  Character * bombPtr3, Character * bombPtr4,
						  Character * invincibleGhostPtr, 
						  Character * missilePtr, Character * gunPtr);
