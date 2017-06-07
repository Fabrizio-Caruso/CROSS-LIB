#include "missile.h"

extern unsigned long points;
extern unsigned short ghostCount;

void checkMissileVsGhost(Character * missilePtr,
						 Character * ghostPtr)
{
	if(ghostPtr->_alive && 
	areCharctersAtSamePosition(missilePtr, ghostPtr))
	{
		gotoxy(ghostPtr->_x,ghostPtr->_y);
		die(ghostPtr); 
		points+=GHOST_VS_MISSILE;
		--ghostCount;
	}
}
	
void checkMissileVsGhosts(Character * missilePtr,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8)
{
	checkMissileVsGhost(missilePtr, ghostPtr1);
	checkMissileVsGhost(missilePtr, ghostPtr2);
	checkMissileVsGhost(missilePtr, ghostPtr3);
	checkMissileVsGhost(missilePtr, ghostPtr4);
	checkMissileVsGhost(missilePtr, ghostPtr5);
	checkMissileVsGhost(missilePtr, ghostPtr6);
	checkMissileVsGhost(missilePtr, ghostPtr7);
	checkMissileVsGhost(missilePtr, ghostPtr8);
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
		missilePtr->_ch = '.';
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
	deleteCharacter(missilePtr);
	missilePtr->_x = newX;
	missilePtr->_y = newY;
	if(wallReached(missilePtr))
	{
		die(missilePtr);
		missilePtr->_ch = '.';
	}
	else
	{
		displayCharacter(missilePtr);
	}
}

void restoreMissile(Character *missilePtr)
{
	missilePtr->_x = 0; missilePtr->_y = 0; missilePtr->_ch = '.';
}
