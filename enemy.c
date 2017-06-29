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

extern Character* ghosts[GHOSTS_NUMBER];

unsigned int computeGhostSlowDown(void)
{
	if(ghostLevel<1000)
		return 28000-level*400-ghostLevel*32;
	else
		return 0;
}

void decreaseGhostLevel(void)
{
	if(ghostLevel>ghostLevelDecrease)
		ghostLevel-=ghostLevelDecrease;
	else
		ghostLevel=0;
}

void displayGhosts(void)
{
	char i;

	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		DRAW_GHOST(ghosts[i]->_x, ghosts[i]->_y, ghosts[i]->_imagePtr);
	}
}
