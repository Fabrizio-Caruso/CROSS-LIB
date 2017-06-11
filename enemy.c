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
 

#include "enemy.h"

extern unsigned int ghostLevel;
extern unsigned short level;
extern unsigned int ghostLevelDecrease;

unsigned int computeGhostSlowDown(void)
{
	if(ghostLevel<500)
	{
	   return 32000-ghostLevel*10-level*200;
	}
	else if(ghostLevel<1000)
	{
	   return 32000-ghostLevel*15-level*200;
	}
	else if(ghostLevel<1200)
	{
	   return 32000-ghostLevel*20-level*200;
	}
	else
	   return 0; // You must die!
}


void decreaseGhostLevel(void)
{
	if(ghostLevel>ghostLevelDecrease)
		ghostLevel-=ghostLevelDecrease;
	else
		ghostLevel=0;
}

void displayGhosts(Character ** ghosts)
{
	char i;

	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if(!(ghosts[i]->_alive))
		{	
			SET_TEXT_COLOR(COLOR_RED);
			DRAW(ghosts[i]); // TODO: Dead ghost should be displayed differently
			SET_TEXT_COLOR(TEXT_COLOR);
		}
		else
		{
			SET_TEXT_COLOR(COLOR_BLACK);
			DRAW(ghosts[i]); // TODO: 
			SET_TEXT_COLOR(TEXT_COLOR);
		}
	}
}