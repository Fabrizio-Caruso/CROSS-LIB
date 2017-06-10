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
			DRAW(ghosts[i]->_x,ghosts[i]->_y,'X');
			SET_TEXT_COLOR(TEXT_COLOR);
		}
		else
		{
			SET_TEXT_COLOR(COLOR_BLACK);
			DRAW(ghosts[i]->_x,ghosts[i]->_y,'O');
			SET_TEXT_COLOR(TEXT_COLOR);
		}
	}
}