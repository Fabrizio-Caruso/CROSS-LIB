#ifndef _MISSILE
#define _MISSILE

#include "character.h"
#include "settings.h"

void checkMissileVsGhost(Character * missilePtr,
					     Character * ghostPtr);
	
void checkMissileVsGhosts(Character * missilePtr,
						  Character ** ghosts);
	
int setMissileInitialPosition(Character *missilePtr, Character *playerPtr,
							  unsigned short missileDirection);
	
void moveMissile(Character * missilePtr, unsigned short missileDirection);

void restoreMissile(Character *missilePtr);

#endif _MISSILE