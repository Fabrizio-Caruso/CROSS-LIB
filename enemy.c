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

int computeGhostSmartness(void)
{
	if(level<=4)
	{
		return level+3;
	}
	else
	{
		return 8;
	}
}

