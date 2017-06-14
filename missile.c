/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/
 

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
		ghost_die(ghostPtr); 
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
		DELETE_MISSILE(missilePtr->_x,missilePtr->_y,misslePtr->_imagePtr);
		DRAW_BROKEN_WALL(missilePtr);
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
	DELETE_MISSILE(missilePtr->_x,missilePtr->_y,missilePtr->_imagePtr);
	missilePtr->_x = newX;
	missilePtr->_y = newY;
	if(wallReached(missilePtr))
	{
		die(missilePtr);
		DELETE_MISSILE(missilePtr->_x,missilePtr->_y,misslePtr->_imagePtr);
		DRAW_BROKEN_WALL(missilePtr);
	}
	else
	{
		DRAW_MISSILE(missilePtr->_x, missilePtr->_y, missilePtr->_imagePtr);
	}
}

void restoreMissile(Character *missilePtr)
{
	missilePtr->_x = 0; missilePtr->_y = 0;
}
