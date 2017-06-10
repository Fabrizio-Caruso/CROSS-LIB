#include "missile.h"
#include "settings.h"

extern unsigned long points;
extern unsigned short ghostCount;

void checkMissileVsGhost(Character * missilePtr,
						 Character * ghostPtr)
{
	if(ghostPtr->_alive && 
	areCharctersAtSamePosition(missilePtr, ghostPtr))
	{
		die(ghostPtr); 
		points+=GHOST_VS_MISSILE;
		--ghostCount;
	}
}
	
void checkMissileVsGhosts(Character * missilePtr,
						Character ** ghosts)
{
	int i = 0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		checkMissileVsGhost(missilePtr, ghosts[i]);
	}
}
	
int setMissileInitialPosition(Character *missilePtr, Character *playerPtr,
							  unsigned short missileDirection)
{
	int newX = playerPtr->_x; 
	int newY = playerPtr->_y;
	switch(missileDirection)
		{
			case RIGHT:
				++newX;
			break;
			case DOWN:
				++newY;
			break;
			case UP:
				--newY;
			break;
			case LEFT:
				--newX;
			break;
		}
	 
	missilePtr->_x = newX;
	missilePtr->_y = newY;	
	if(wallReached(missilePtr))
	{
		die(missilePtr);
		return 0;
	}
	return 1;
}
	
void moveMissile(Character * missilePtr, unsigned short missileDirection)
{
	int newX = missilePtr->_x; 
	int newY = missilePtr->_y;
	switch(missileDirection)
	{
		case RIGHT:
			++newX;
		break;
		case DOWN:
			++newY;
		break;
		case UP:
			--newY;
		break;
		case LEFT:
			--newX;
		break;
	}
	DELETE_CHARACTER(missilePtr);
	missilePtr->_x = newX;
	missilePtr->_y = newY;
	if(wallReached(missilePtr))
	{
		die(missilePtr);
	}
	else
	{
		DRAW_MISSILE(missilePtr);
	}
}

void restoreMissile(Character *missilePtr)
{
	missilePtr->_x = 0; missilePtr->_y = 0;
}
