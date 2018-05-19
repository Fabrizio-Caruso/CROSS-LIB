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
 
#include "character.h"
#include "item.h"
#include "item.h"
#include "settings.h"
#include "enemy.h"

#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
#else
	#include <stdlib.h>
#endif

#include "display_macros.h"
#include "sound_macros.h" 
#include "sleep_macros.h"

#include "level.h"

extern unsigned char level;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image DEAD_GHOST_IMAGE;

#if !defined(TINY_GAME)
	extern Image INVINCIBLE_GHOST_IMAGE;
	extern Image MISSILE_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image GUN_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
#endif

extern Character player; 

#if !defined(TINY_GAME)
	extern Character invincibleGhost;

	extern Item powerUp;
	extern Item powerUp2;
	extern Item gun;
	extern Item extraPoints;

	extern Character missile;

#endif

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];


#if defined(FULL_GAME) 

	extern Item chase;
	extern Character chasingBullet;

	extern unsigned char innerVerticalWallX;
	extern unsigned char innerVerticalWallY;
	extern unsigned char innerVerticalWallLength;

	extern Image BUBBLE_IMAGE;

	extern Image FREEZE_IMAGE;
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image SUPER_IMAGE;
	extern Image CONFUSE_IMAGE;
	extern Image ZOMBIE_IMAGE;

	extern Character leftEnemyMissile;
	extern Character rightEnemyMissile;

	extern Item freeze;
	extern Item extraLife;
	extern Item invincibility;
	extern Item super;
	extern Item confuse;
	extern Item zombie;
	
	extern Character bubbles[BUBBLES_NUMBER];

	extern char bubbles_x[BUBBLES_NUMBER];
	
	extern char skullsKilled;
#endif


#if defined(FULL_GAME)
	void updateInnerWallVerticalData(void)
	{	
	unsigned char lvmod = level%5;
	if((lvmod==1)||(lvmod==0))
	{
		innerVerticalWallLength = 0;
	}
	else
	{
		#if defined(WIDE)
			innerVerticalWallLength = YSize-10+(lvmod-2)*2;
		#elif YSize<=12
			innerVerticalWallLength = 4;
		#else
			innerVerticalWallLength = YSize-14+(lvmod-2)*2;			
		#endif
	}

	innerVerticalWallX = (XSize>>1);
	innerVerticalWallY = (YSize>>1)-(innerVerticalWallLength>>1);
	}

	unsigned char oneMissileLevel(void)
	{
		return ((level%5)==3) || (level==5);
//		|| ((level%5==1) && (level>10));		
	}

	unsigned char rocketLevel(void)
	{
		return (level >= FIRST_BUBBLES_LEVEL) && ((level%5)==2 || (level%5)==3);
	}

	unsigned char missileLevel(void)
	{
		return level%5==4;
	}	

	unsigned char bossLevel(void)
	{
		return !(level%5);
	}
	
	unsigned char horizontalWallsLevel(void)
	{
		return ((level >= FIRST_HORIZONTAL_WALLS_LEVEL) && ((level%5==1) || (level%5==4)));
	}	

	void initializeAwayFromWall(Character * characterPtr, unsigned char x, unsigned char y, unsigned char status, Image *imagePtr)
	{
		do{
			initializeCharacter(characterPtr, x, y, status, imagePtr);
			relocateCharacter(characterPtr,bombs,4);
		} while(nearInnerWall(characterPtr));
	}

#endif 

#if defined(BETWEEN_LEVEL)

void _spiral_slow_down()
{
	unsigned char k;
	
	for(k=0;k<254;++k){};
}

void spiral(Character *characterPtr, unsigned char length)
{
	unsigned char i;
	unsigned char j;
	
	characterPtr->_x = XSize/2;
	characterPtr->_y = YSize/2;
	for(i=0;i<length;++i)
	{
		for(j=0;j<i/2;++j)
			{
				displayCharacter(characterPtr);		
				if(i&2)
				{
					++(*((unsigned char *) characterPtr + (i&1)));
				}
				else
				{
					--(*((unsigned char *) characterPtr + (i&1)));				
				}
				#if defined(SLOW_DOWN)
				_spiral_slow_down();	
				#endif
			}
	}
}

#endif


void fillLevelWithCharacters(unsigned char nGhosts)
{
	unsigned char i;
	unsigned char j;
	unsigned char count = 0;
	
	#if defined(FULL_GAME)
		if(rocketLevel() || bossLevel())
		{
			for(i=0;i<BUBBLES_NUMBER;i++)
			{
				bubbles_x[i] = (unsigned char) (i+1)*(XSize/(BUBBLES_NUMBER+1));
				initializeCharacter(&bubbles[i],(unsigned char) bubbles_x[i],(unsigned char)(YSize-1),1,&BUBBLE_IMAGE);
				displayMissile(&bubbles[i]);
			}
		}

		if(bossLevel())
		{
			nGhosts = level>>2;
		}				
	#endif
	
	#if GHOSTS_NUMBER >= 9
		ROUND_NINE_GHOSTS();
	#elif GHOSTS_NUMBER>=7 
		#if !defined(TINY_GAME) || defined(ROUND_ENEMIES)
			ROUND_EIGHT_GHOSTS();	
		#else
			FLAT_EIGHT_GHOSTS();
		#endif
	#else
		FLAT_SIX_GHOSTS();
	#endif

	#if BOMBS_NUMBER==4
	{
		count = 0;
		for(i=1;i<=2;++i)
		{
			for(j=1;j<=2;++j)
			{
				#if defined(NO_RANDOM_LEVEL)
					initializeCharacter(&bombs[count],(unsigned char) ((XSize/3)*i), (unsigned char) ((YSize/3)*j),0,&BOMB_IMAGE);				
				#else
					#if defined(TINY_GAME)
						initializeCharacter(&bombs[count],(unsigned char) (((XSize/3)*i)-(unsigned char)(rand()&1)), 
														  (unsigned char) ((YSize/3)*j),0,&BOMB_IMAGE);								
					#else
						initializeCharacter(&bombs[count],(unsigned char) (((XSize/3)*i)-(unsigned char)(rand()&1)), 
														  (unsigned char) (((YSize/3)*j)+(unsigned char)(rand()&1)),0,&BOMB_IMAGE);														
					#endif
				#endif
				++count;
			}
		}
	}
	#elif BOMBS_NUMBER==3	
	{
		#if defined(NO_RANDOM_LEVEL)
			initializeCharacter(&bombs[0],(XSize>>1), ((YSize/3)<<1),0,&BOMB_IMAGE);
			initializeCharacter(&bombs[1],XSize/3, (YSize/3),0,&BOMB_IMAGE);
			initializeCharacter(&bombs[2],((XSize/3)<<1), (YSize/3),0,&BOMB_IMAGE);		
		#else
			unsigned char rnd = rand()&1;
			initializeCharacter(&bombs[0],XSize/3+rnd, (YSize/3)+rnd,0,&BOMB_IMAGE);
			initializeCharacter(&bombs[1],(XSize>>1)+rnd, ((YSize/3)*2)+rnd,0,&BOMB_IMAGE);
			initializeCharacter(&bombs[2],2*(XSize/3)+rnd, (YSize/3)-rnd,0,&BOMB_IMAGE);
		#endif
	}
	#elif BOMBS_NUMBER==2
		#if defined(NO_RANDOM_LEVEL)
			initializeCharacter(&bombs[0],(XSize>>1), ((YSize/3)),0,&BOMB_IMAGE);
			initializeCharacter(&bombs[1],(XSize>>1), ((YSize/3)<<1),0,&BOMB_IMAGE);		
		#else
			initializeCharacter(&bombs[0],(XSize>>1), ((YSize/3))+rand()%3,0,&BOMB_IMAGE);
			initializeCharacter(&bombs[1],(XSize>>1)-1+rand()%3, ((YSize/3)*2)-1+rand()%3,0,&BOMB_IMAGE);
		#endif
	#elif BOMBS_NUMBER==1
		#if defined(NO_RANDOM_LEVEL)
			initializeCharacter(&bombs[0],(XSize>>1), (YSize>>1),0,&BOMB_IMAGE);			
		#else
			initializeCharacter(&bombs[0],(XSize>>1)+rand()&1, (YSize>>1)+rand()&1,0,&BOMB_IMAGE);
		#endif
	#endif
	
	#if defined(FULL_GAME)
		initializeAwayFromWall(&(powerUp._character),(XSize>>1),(YSize>>1),1,&POWERUP_IMAGE);
		initializeAwayFromWall(&(powerUp2._character),(XSize>>1),(YSize>>1),0,&POWERUP_IMAGE);
		initializeAwayFromWall(&(freeze._character),(XSize>>1),(YSize>>1),0,&FREEZE_IMAGE);
		initializeAwayFromWall(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);
		initializeAwayFromWall(&(super._character), (XSize>>1), (YSize>>1), 0, &SUPER_IMAGE);
		initializeAwayFromWall(&(confuse._character), (XSize>>1), (YSize>>1), 0, &CONFUSE_IMAGE);		
		initializeAwayFromWall(&(zombie._character), (XSize>>1), (YSize>>1), 0, &ZOMBIE_IMAGE);	
		
		initializeAwayFromWall(&(gun._character),(XSize>>1), (YSize>>1), (bossLevel() ? 1 : 0), &GUN_IMAGE);

		do
		{
			initializeAwayFromWall(&player,(unsigned char) ((XSize>>1)+(rand()&1)),(unsigned char) ((YSize>>1)+(rand()&1)),1,&PLAYER_IMAGE);
		} while(player._y==(YSize>>1));
		
		initializeAwayFromWall(&(extraLife._character), (XSize>>1), (YSize>>1), 0, &EXTRA_LIFE_IMAGE);

		initializeAwayFromWall(&(invincibility._character), (XSize>>1), (YSize>>1), 0, &INVINCIBILITY_IMAGE);

		if(oneMissileLevel())
		{
			initializeCharacter(&rightEnemyMissile,         XSize-1,                      (YSize>>1), 1,&RIGHT_ENEMY_MISSILE_IMAGE);			
		}
		else if(missileLevel() || bossLevel())
		{	
			initializeCharacter(&rightEnemyMissile,         XSize-1,         ENEMY_MISSILE_OFFSET, 1,&RIGHT_ENEMY_MISSILE_IMAGE);
			initializeCharacter(&leftEnemyMissile,                0, YSize-1-ENEMY_MISSILE_OFFSET, 1,&LEFT_ENEMY_MISSILE_IMAGE);		
		}		
		
		initializeAwayFromWall(&(chase._character), (XSize>>1), (YSize>>1),0,&MISSILE_IMAGE);
		initializeCharacter(&chasingBullet, 0,0, 0, &MISSILE_IMAGE);
	#else
		#if !defined(TINY_GAME)
			initializeCharacter(&(powerUp._character),(XSize>>1),(YSize>>1),1,&POWERUP_IMAGE);
			initializeCharacter(&(powerUp2._character),(XSize>>1),(YSize>>1),0,&POWERUP_IMAGE);
			initializeCharacter(&(gun._character),(XSize>>1), (YSize>>1), 0, &GUN_IMAGE);	
			initializeCharacter(&(extraPoints._character), (XSize>>1), (YSize>>1), 0, &EXTRA_POINTS_IMAGE);	
		#endif
		
		#if defined(NO_RANDOM_LEVEL) || defined(TINY_GAME)
			initializeCharacter(&player,(unsigned char) ((XSize>>1)),(unsigned char) ((YSize>>1)),1,&PLAYER_IMAGE);			
		#else
			initializeCharacter(&player,(unsigned char) ((XSize>>1)+(unsigned char) (rand()&1)),
										(unsigned char) ((YSize>>1)+(unsigned char) (rand()&1)),1,&PLAYER_IMAGE);	
		#endif
	#endif
	#if !defined(TINY_GAME)
		displayPlayer(&player);
			
		initializeCharacter(&missile, 0, 0,0,&MISSILE_IMAGE);
		initializeCharacter(&invincibleGhost,XSize-2,YSize-2, 0, &INVINCIBLE_GHOST_IMAGE);
	#endif

}

