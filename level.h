#include "character.h"

void drawInnerVerticalWall(int XSize, int YSize);

unsigned short drawInnerVerticalWallForLevel(int XSize,int YSize, unsigned short level);

void fillLevelWithCharacters(int XSize, int YSize,
						  Character * playerPtr, Character * powerUpPtr, 
						  Character * ghostPtr1, Character * ghostPtr2,
						  Character * ghostPtr3, Character * ghostPtr4,
						  Character * ghostPtr5, Character * ghostPtr6,
						  Character * ghostPtr7, Character * ghostPtr8,
						  Character * bombPtr1, Character * bombPtr2,
						  Character * bombPtr3, Character * bombPtr4,
						  Character * invincibleGhostPtr, 
						  Character * missilePtr, Character * gunPtr);
