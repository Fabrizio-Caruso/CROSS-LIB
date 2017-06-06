#include "missile.h"

void checkMissileVsGhost(Character * missilePtr,
					   Character * ghostPtr, 
					   unsigned long * pointsPtr, unsigned short * ghostCountPtr)
{
	if(ghostPtr->_alive && 
	areCharctersAtSamePosition(missilePtr, ghostPtr))
	{
		gotoxy(ghostPtr->_x,ghostPtr->_y);
		die(ghostPtr); 
		(*pointsPtr)+=GHOST_VS_MISSILE;
		--(*ghostCountPtr);
	}
}
	
void checkMissileVsGhosts(Character * missilePtr,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8,
						unsigned long * pointsPtr, unsigned short * ghostCountPtr)
{
	checkMissileVsGhost(missilePtr, ghostPtr1, pointsPtr, ghostCountPtr);
	checkMissileVsGhost(missilePtr, ghostPtr2, pointsPtr, ghostCountPtr);
	checkMissileVsGhost(missilePtr, ghostPtr3, pointsPtr, ghostCountPtr);
	checkMissileVsGhost(missilePtr, ghostPtr4, pointsPtr, ghostCountPtr);
	checkMissileVsGhost(missilePtr, ghostPtr5, pointsPtr, ghostCountPtr);
	checkMissileVsGhost(missilePtr, ghostPtr6, pointsPtr, ghostCountPtr);
	checkMissileVsGhost(missilePtr, ghostPtr7, pointsPtr, ghostCountPtr);
	checkMissileVsGhost(missilePtr, ghostPtr8, pointsPtr, ghostCountPtr);
}
	
int setMissileInitialPosition(int XSize, int YSize, Character *missilePtr, Character *playerPtr,
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
	if(wallReached(XSize,YSize, missilePtr))
	{
		die(missilePtr);
		missilePtr->_ch = '.';
		return 0;
	}
	return 1;
}
	
void moveMissile(int XSize, int YSize, Character * missilePtr, unsigned short missileDirection)
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
	if(wallReached(XSize,YSize, missilePtr))
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
