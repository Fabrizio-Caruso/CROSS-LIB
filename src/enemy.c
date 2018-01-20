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


#include "enemy.h"
#include "settings.h"

extern unsigned short ghostLevel;
extern unsigned char level;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];

unsigned short computeGhostSlowDown(void)
{
	if(ghostLevel<MAX_GHOST_LEVEL)
	{
		return INITIAL_ENEMY_SLOWDOWN-level*200-ghostLevel*16;
	}
	return ENEMY_MIN_SLOWDOWN;
}

#if !defined(TINY_GAME)
	void decreaseGhostLevel(void)
	{
		if(ghostLevel>GHOST_LEVEL_DECREASE)
			ghostLevel-=GHOST_LEVEL_DECREASE;
		else
			ghostLevel=0;
	}
#endif

void displayGhosts(void)
{
	unsigned char i;

	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		// displayGhost(&ghosts[i]);
		DRAW_GHOST(ghosts[i]._x, ghosts[i]._y, ghosts[i]._imagePtr);
	}
}

void displayBombs(void)
{
	unsigned char i;

	for(i=0;i<BOMBS_NUMBER;++i)
	{
		displayGhost(&bombs[i]);
		//DRAW_BOMB(bombs[i]._x, bombs[i]._y, bombs[i]._imagePtr);
	}
}	

#if defined(FULL_GAME)
	unsigned char firstAlive(void)
	{
		unsigned char i;
		
		i=0;
		while(i<GHOSTS_NUMBER)
		{
			if(ghosts[i]._status)
				return i;
			++i;
		}
		return GHOSTS_NUMBER;
	}
#endif
