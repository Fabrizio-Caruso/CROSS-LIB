/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 
 
#if !defined(TINY_GAME) 

#include "settings.h"
 
#include "bullet.h"
#include "level.h"
#include "text.h"
#include "character.h"
#include "item.h"
#include "ghost.h"
#include "strategy.h"



extern unsigned short points;
extern unsigned char ghostCount;
extern Character ghosts[GHOSTS_NUMBER];
#if !defined(NO_DEAD_GHOSTS)
extern Image DEAD_GHOST_IMAGE;
#else
extern Image SKULL_IMAGE;	
#endif
extern unsigned char level;

extern Item extraPoints;

extern Character skull;
extern unsigned char skullHits;
extern unsigned char skullAlive;
extern unsigned char playerFire;
extern unsigned char guns;
extern unsigned char playerDirection;
extern unsigned char bulletDirection;

extern Character bullet;
extern Character player;


	
#if defined(FULL_GAME) 
	extern Item freeze;
	extern Item extraLife;
	extern Item invincibility;
	
	extern Character chasingBullet;
	
	extern Character leftEnemyMissile;
	extern Character rightEnemyMissile;
	extern unsigned char rockets_x[ROCKETS_NUMBER];
	extern Character rockets[ROCKETS_NUMBER];
	extern unsigned char dead_rockets;
	extern unsigned char missileBasesDestroyed;

	extern Character *chasedEnemyPtr;	
#endif

#if defined(FULL_GAME)
	void handle_chasing_bullet(void)
	{
		if(chasingBullet._status)
		{
			deleteMissile(&chasingBullet);
			#if !defined(SIMPLE_STRATEGY)
				moveTowardCharacter(chasedEnemyPtr, &chasingBullet, 4);
			#else
				moveTowardCharacter(chasedEnemyPtr, &chasingBullet);				
			#endif
			displayMissile(&chasingBullet);
			checkBullet(&chasingBullet);
		}	
	}
#endif
	
void handle_bullet(void)
{
	// Check if player has fired the gun
	if(playerFire && bullet._status==0 && guns>0)
	{
		SHOOT_SOUND();
		--guns;
		printGunsStats();
		bulletDirection = playerDirection;
		bullet._status = setBulletInitialPosition(&bullet, &player);
		playerFire = 0;
		displayMissile(&bullet);
		checkBullet(&bullet);		
	}
	
	// Move bullet if fired
	if(bullet._status==1)
	{
		moveBullet(&bullet);
		checkBullet(&bullet);
	}
}


void checkBullet(Character *bulletPtr)
{
	checkBulletVsSkull(bulletPtr);
	checkBulletVsGhosts(bulletPtr);
}

void checkBulletVsGhost(Character * bulletPtr,
						 Character * ghostPtr)
{
	if(ghostPtr->_status && 
	   areCharctersAtSamePosition(bulletPtr, ghostPtr))
	{
		points+=GHOST_VS_MISSILE;
		#if !defined(NO_DEAD_GHOSTS)
			ghostPtr->_imagePtr = (Image *)&DEAD_GHOST_IMAGE;
		#else
			ghostPtr->_imagePtr = (Image *)&SKULL_IMAGE;			
		#endif
		ghostDies(ghostPtr);
		die(bulletPtr);
	}
}
	
void checkBulletVsGhosts(Character * bulletPtr)
{
	unsigned char i = 0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		if(ghosts[i]._status)
		{
			checkBulletVsGhost(bulletPtr, &ghosts[i]);
		}
	};
}

void checkBulletVsSkull(register Character *bulletPtr)
{
	if(skull._status && 
	   areCharctersAtSamePosition(bulletPtr, &skull))
	{
		PING_SOUND();
		die(bulletPtr);
		deleteMissile(bulletPtr);
		bulletPtr->_x = 0; bulletPtr->_y = 0;
		++skullHits;
		decreaseGhostLevel();
		reducePowerUpsCoolDowns();
		
		if(skullHits>=MIN_SKULL_HITS)
		{
			skull._status = 0;
			deleteSkull(&skull);
			skull._x=XSize-2; 
			skull._y=YSize-2;
			skullAlive = 0;
			EXPLOSION_SOUND();
			points+=SKULL_POINTS;
			displayStats();
		}
		else
		{
			displaySkull(&skull);
		}
	}	
}


void _moveBullet(register Character *bulletPtr)
{
	deleteMissile(bulletPtr);
	switch(bulletDirection)
	{
		case RIGHT:
			++bulletPtr->_x;
		break;
		case DOWN:
			++bulletPtr->_y;
		break;
		case UP:
			--bulletPtr->_y;
		break;
		case LEFT:
			--bulletPtr->_x;
		break;
	}	
}

unsigned char setBulletInitialPosition(Character *bulletPtr, Character *playerPtr)
{
	bulletPtr->_x = playerPtr->_x; 
	bulletPtr->_y = playerPtr->_y;
	_moveBullet(bulletPtr);
	if(wallReached(bulletPtr))
	{
		die(bulletPtr);
		deleteMissile(bulletPtr);
		#if defined(FULL_GAME)
			DRAW_BROKEN_WALL(bulletPtr->_x, bulletPtr->_y);
		#endif
		return 0;
	}
	return 1;
}

#if defined(FULL_GAME)
	void destroyEnemyMissile(Character * enemyMissilePtr)
	{
		enemyMissilePtr->_status = 0;
		EXPLOSION_SOUND();
		deleteMissile(enemyMissilePtr);
		points+=HORIZONTAL_MISSILE_BONUS;
		displayStats();				
		++dead_rockets;
		reducePowerUpsCoolDowns();		
	}
#endif

void moveBullet(register Character * bulletPtr)
{
	_moveBullet(bulletPtr);
	if(wallReached(bulletPtr) && bulletPtr->_status)
	{
		die(bulletPtr);
		deleteMissile(bulletPtr);
		#if defined(FULL_GAME)
			DRAW_BROKEN_WALL(bulletPtr->_x, bulletPtr->_y);
			
			if(oneMissileLevel())
			{
				if(bulletPtr->_x==XSize-1 && bulletPtr->_y==YSize/2 && rightEnemyMissile._status)
				{
					destroyEnemyMissile(&rightEnemyMissile);
				}
			}				
			else if(missileLevel() || bossLevel())
			{
				if(bulletPtr->_x==XSize-1 && bulletPtr->_y==HORIZONTAL_MISSILE_OFFSET && rightEnemyMissile._status)
				{
					destroyEnemyMissile(&rightEnemyMissile);	
				}
				else if(bulletPtr->_x==0 && bulletPtr->_y==YSize-1-HORIZONTAL_MISSILE_OFFSET && leftEnemyMissile._status)
				{
					destroyEnemyMissile(&leftEnemyMissile);	
				}
			}
			if((rocketLevel() || bossLevel()) && bulletPtr->_y==YSize-1)
			{
				unsigned char i;
				for(i=0;i<ROCKETS_NUMBER;++i)
				{
					if(bulletPtr->_x==rockets_x[i] && rockets[i]._status)
					{
						rockets[i]._status = 0;
						++dead_rockets;
						EXPLOSION_SOUND();
						deleteMissile(&rockets[i]);
						points+=VERTICAL_MISSILE_BONUS;
						displayStats();		
					}
				}
			}			
		#endif		
	}
	else
	{
		displayMissile(bulletPtr);
	}
}

#endif

