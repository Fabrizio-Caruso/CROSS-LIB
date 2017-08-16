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
#include "sound_macros.h"
#include "display_macros.h"
#include "level.h"
#include "text.h"
#include "powerUps.h"

extern unsigned long points;
extern unsigned char ghostCount;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bubbles[BUBBLES_NUMBER];

extern unsigned char bubbles_x[BUBBLES_NUMBER];

extern Image DEAD_GHOST_IMAGE;
extern unsigned char level;

extern unsigned short extraPointsCoolDown;
extern unsigned short invincibilityCoolDown;
extern unsigned short extraLifeCoolDown;

extern unsigned char dead_bubbles;

void checkMissileVsGhost(Character * missilePtr,
						 Character * ghostPtr)
{
	if(ghostPtr->_status && 
	   areCharctersAtSamePosition(missilePtr, ghostPtr))
	{
		EXPLOSION_SOUND();
		ghostPtr->_imagePtr = &DEAD_GHOST_IMAGE;
		die(ghostPtr); 
		points+=GHOST_VS_MISSILE;
		displayStats();		
		--ghostCount;
		printGhostCountStats();
	}
}
	
void checkMissileVsGhosts(Character * missilePtr)
{
	unsigned char i = 0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		checkMissileVsGhost(missilePtr, &ghosts[i]);
	};
}
	
unsigned char setMissileInitialPosition(Character *missilePtr, Character *playerPtr,
							  unsigned short missileDirection, Character *rightEnemyMissilePtr, Character *leftEnemyMissilePtr)
{
	unsigned char newX = playerPtr->_x; 
	unsigned char newY = playerPtr->_y;
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
		DRAW_BROKEN_WALL(missilePtr->_x, missilePtr->_y);
		if(missilePtr->_x==1 && missilePtr->_y==4)
		{
			rightEnemyMissilePtr->_status = 0;
			EXPLOSION_SOUND();
			DELETE_MISSILE(rightEnemyMissilePtr->_x,rightEnemyMissilePtr->_y,rightEnemyMissilePtr->_imagePtr);
		}
		else if(missilePtr->_x==XSize-1 && missilePtr->_y==YSize-4)
		{
			leftEnemyMissilePtr->_status = 0;
			EXPLOSION_SOUND();
			DELETE_MISSILE(leftEnemyMissilePtr->_x,leftEnemyMissilePtr->_y,leftEnemyMissilePtr->_imagePtr);
		}
		return 0;
	}
	return 1;
}
	
void moveMissile(Character * missilePtr, unsigned short missileDirection, Character *rightEnemyMissilePtr, Character *leftEnemyMissilePtr)
{
	unsigned char newX = missilePtr->_x; 
	unsigned char newY = missilePtr->_y;
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
		DRAW_BROKEN_WALL(missilePtr->_x,missilePtr->_y);
		if(missileLevel() || bossLevel())
		{
			if(missilePtr->_x==XSize-1 && missilePtr->_y==4 && rightEnemyMissilePtr->_status)
			{
				rightEnemyMissilePtr->_status = 0;
				EXPLOSION_SOUND();
				DELETE_MISSILE(rightEnemyMissilePtr->_x,rightEnemyMissilePtr->_y,rightEnemyMissilePtr->_imagePtr);
				points+=HORIZONTAL_MISSILE_BONUS;
				displayStats();				
				// extraPointsCoolDown/=2;
				// invincibilityCoolDown/=2;
				// extraLifeCoolDown/=2;
				reducePowerUpsCoolDowns();		
			}
			else if(missilePtr->_x==0 && missilePtr->_y==YSize-4 && leftEnemyMissilePtr->_status)
			{
				leftEnemyMissilePtr->_status = 0;
				EXPLOSION_SOUND();
				DELETE_MISSILE(leftEnemyMissilePtr->_x,leftEnemyMissilePtr->_y,leftEnemyMissilePtr->_imagePtr);
				points+=HORIZONTAL_MISSILE_BONUS;
				displayStats();				
				// extraPointsCoolDown/=2;
				// invincibilityCoolDown/=2;
				// extraLifeCoolDown/=2;
				reducePowerUpsCoolDowns();		
			}
		}
		if((rocketLevel() || bossLevel()) && missilePtr->_y==YSize-1)
		{
			unsigned char i;
			for(i=0;i<BUBBLES_NUMBER;++i)
			{
				if(missilePtr->_x==bubbles_x[i] && bubbles[i]._status)
				{
					bubbles[i]._status = 0;
					++dead_bubbles;
					EXPLOSION_SOUND();
					DELETE_MISSILE(bubbles[i]._x,bubbles[i]._y,bubbles[i]._imagePtr);
					DELETE_MISSILE(leftEnemyMissilePtr->_x,leftEnemyMissilePtr->_y,leftEnemyMissilePtr->_imagePtr);
					points+=VERTICAL_MISSILE_BONUS;
					displayStats();					
					if(dead_bubbles==BUBBLES_NUMBER)
					{
						// extraPointsCoolDown/=4;
						// invincibilityCoolDown/=4;
						// extraLifeCoolDown/=4;
						reducePowerUpsCoolDowns();
						reducePowerUpsCoolDowns();		
						TICK_SOUND();
					}
				}
			}
		}
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
