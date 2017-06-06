#ifndef _MISSILE
#define _MISSILE

#include "character.h"
#include "settings.h"

void checkMissileVsGhost(Character * missilePtr,
					     Character * ghostPtr, unsigned long * pointsPtr, unsigned short * ghostCountPtr);
	
void checkMissileVsGhosts(Character * missilePtr,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8, 
						unsigned long * pointsPtr, unsigned short * ghostCountPtr);
	
int setMissileInitialPosition(int XSize, int YSize, Character *missilePtr, Character *playerPtr,
							  unsigned short missileDirection);
	
void moveMissile(int XSize, int YSize, Character * missilePtr, unsigned short missileDirection);

void restoreMissile(Character *missilePtr);

#endif _MISSILE