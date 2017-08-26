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
extern Image BUBBLE_IMAGE;
extern Image DEAD_GHOST_IMAGE;

extern Character invincibleGhost;
extern Character player; 
extern Character powerUp;
extern Character gun;
extern Character missile;


extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];

void fillLevelWithCharacters(unsigned char nGhosts)
{

	unsigned char i;
	unsigned char j;
	unsigned char count = 0;
	
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
					initializeCharacter(&ghosts[count],8-count,1,0,&DEAD_GHOST_IMAGE);
				}
				++count;
			}
		}
	}

	initializeCharacter(&player,XSize/2+rand()%4-2,YSize/2+rand()%4-2,1,&PLAYER_IMAGE);

	DRAW_PLAYER(player._x,player._y,player._imagePtr);

	
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

	initializeCharacter(&powerUp,XSize/2,YSize/2,1,&POWERUP_IMAGE);

	initializeCharacter(&missile, 0, 0,0,&MISSILE_IMAGE);
	
	initializeCharacter(&gun,XSize/2, YSize/2, 0, &GUN_IMAGE);

	initializeCharacter(&invincibleGhost,XSize-2,YSize-2, 0, &INVINCIBLE_GHOST_IMAGE);

}

