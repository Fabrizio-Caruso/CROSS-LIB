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
#include "settings.h"

#include <stdlib.h>

#include "display_macros.h"
#include "sound_macros.h" 
#include "sleep_macros.h"

#include "level.h"

extern unsigned char level;
extern unsigned char XSize;
extern unsigned char YSize;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;
extern Image DEAD_GHOST_IMAGE;


extern Character invincibleGhost;
extern Character player; 
extern Character powerUp;
extern Character gun;
extern Character missile;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];


#if defined(FULL_GAME)
	extern unsigned char innerVerticalWallX;
	extern unsigned char innerVerticalWallY;
	extern unsigned char innerVerticalWallLength;

	extern Image BUBBLE_IMAGE;

	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;

	extern Image EXTRA_POINTS_IMAGE;
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;

	extern Character leftEnemyMissile;
	extern Character rightEnemyMissile;
	extern Character extraPoints;
	extern Character extraLife;
	extern Character invincibility;
	
	extern Character bubbles[BUBBLES_NUMBER];

	extern char bubbles_x[BUBBLES_NUMBER];
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
			innerVerticalWallLength = YSize-12+(lvmod-2)*2;
		#else
			innerVerticalWallLength = YSize-12+(lvmod-2)*2-4;			
		#endif
	}

	innerVerticalWallX = XSize / 2;
	innerVerticalWallY = YSize/2-(innerVerticalWallLength/2);
	}


	unsigned char rocketLevel(void)
	{
		return level >= FIRST_BUBBLES_LEVEL && ((level%5)==2 || (level%5)==3);
	}

	unsigned char missileLevel(void)
	{
		return level%5==4;
	}	

	unsigned char bossLevel(void)
	{
		return level%5==0;
	}

	void initializeAwayFromWall(Character * characterPtr, unsigned char x, unsigned char y, unsigned char status, Image *imagePtr)
	{
		do{
			initializeCharacter(characterPtr, x, y, status, imagePtr);
			relocateCharacter(characterPtr,bombs,4);
		} while(nearInnerWall(characterPtr));
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
				initializeCharacter(&bubbles[i],bubbles_x[i],YSize-1,1,&BUBBLE_IMAGE);
				DRAW_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);
			}
		}

		if(bossLevel())
		{
			nGhosts = level/8;
		}		
		
	#endif
	
	for(i=0;i<3;++i)
	{
		for(j=0;j<3;++j)
		{
			if(!((i==1) && (j==1)))
			{
				if(nGhosts>count)
				{
					initializeCharacter(&ghosts[count],XSize/6+j*2*(XSize/6),YSize/6+i*2*(YSize/6)+i,1,&GHOST_IMAGE);
				}
				else
				{
					initializeCharacter(&ghosts[count],GHOSTS_NUMBER-count,1,0,&DEAD_GHOST_IMAGE);
				}
				++count;
			}
		}
	}
	#if GHOSTS_NUMBER>8
		if(nGhosts>=9)
		{
			initializeCharacter(&ghosts[count],XSize-3,YSize-3,1,&GHOST_IMAGE);
		}
		else
		{
			initializeCharacter(&ghosts[count],1,1,0,&DEAD_GHOST_IMAGE);
		}
	#endif


	#if BOMBS_NUMBER==4
	{
		count = 0;
		for(i=1;i<=2;++i)
		{
			for(j=1;j<=2;++j)
			{
				initializeCharacter(&bombs[count],(XSize/3)*i-1+rand()%3, (YSize/3)*j-1+rand()%3,0,&BOMB_IMAGE);
				++count;
			}
		}
	}
	#elif BOMBS_NUMBER==3	
	{
		unsigned char rnd = rand()%4;
		initializeCharacter(&bombs[0],XSize/3-2+rnd, ((YSize/3))-1+rnd,0,&BOMB_IMAGE);

		initializeCharacter(&bombs[1],XSize/3-2-rnd, ((YSize/3)*2)-1+rnd,0,&BOMB_IMAGE);

		initializeCharacter(&bombs[2],2*(XSize/3)-2+rnd, (YSize/2)-1-rnd,0,&BOMB_IMAGE);
	}
	#elif BOMBS_NUMBER==2
		initializeCharacter(&bombs[0],XSize/2-3+rand()%7, ((YSize/3))-1+rand()%3,0,&BOMB_IMAGE);

		initializeCharacter(&bombs[1],XSize/2-3+rand()%7, ((YSize/3)*2)-1+rand()%3,0,&BOMB_IMAGE);
	#elif BOMBS_NUMBER==1
		initializeCharacter(&bombs[0],XSize/2-3+rand()%7, ((YSize/2))-1+rand()%3,0,&BOMB_IMAGE);
	#endif

	
	#if defined(FULL_GAME)
		initializeAwayFromWall(&powerUp,XSize/2,YSize/2,1,&POWERUP_IMAGE);
		
		if(bossLevel())
		{
			initializeAwayFromWall(&gun,XSize/2, YSize/2, 1, &GUN_IMAGE);
		}
		else
		{
			initializeAwayFromWall(&gun,XSize/2, YSize/2, 0, &GUN_IMAGE);
		}
		
		initializeAwayFromWall(&player,XSize/2+rand()%4-2,YSize/2+rand()%4-2,1,&PLAYER_IMAGE);
		
		initializeAwayFromWall(&extraPoints, XSize/2, YSize/2, 0, &EXTRA_POINTS_IMAGE);
		
		initializeAwayFromWall(&extraLife, XSize/2, YSize/2, 0, &EXTRA_LIFE_IMAGE);

		initializeAwayFromWall(&invincibility, XSize/2, YSize/2, 0, &INVINCIBILITY_IMAGE);

		if(missileLevel() || bossLevel())
		{	
			initializeCharacter(&rightEnemyMissile,         XSize-1,         ENEMY_MISSILE_OFFSET, 1,&RIGHT_ENEMY_MISSILE_IMAGE);
			initializeCharacter(&leftEnemyMissile,                0, YSize-1-ENEMY_MISSILE_OFFSET, 1,&LEFT_ENEMY_MISSILE_IMAGE);		
		}		
		
	#else
		initializeCharacter(&powerUp,XSize/2,YSize/2,1,&POWERUP_IMAGE);
		
		initializeCharacter(&gun,XSize/2, YSize/2, 0, &GUN_IMAGE);		
		
		initializeCharacter(&player,XSize/2+rand()%4-2,YSize/2+rand()%4-2,1,&PLAYER_IMAGE);	
	#endif
	DRAW_PLAYER(player._x,player._y,player._imagePtr);
		
	initializeCharacter(&missile, 0, 0,0,&MISSILE_IMAGE);

	initializeCharacter(&invincibleGhost,XSize-2,YSize-2, 0, &INVINCIBLE_GHOST_IMAGE);
	
}

