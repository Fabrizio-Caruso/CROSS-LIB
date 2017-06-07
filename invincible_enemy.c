#include "settings.h"
#include "invincible_enemy.h"

extern unsigned short level;
extern unsigned int loop;

int computeInvincibleSlowDown(void)
{
	if(loop<1000)
	{
		return 32000 - (level/2+1) * 1000 - loop/2;
	}
	else if(loop<11000)
	{
		return 32000 - level * 1000 - loop;
	}
	return 0; // You must die!
}

int computeInvincibleGhostCountTrigger(void)
{
	if(level<=7)
		return level/2 + 1;
	else
		return 5;
}

int computeInvincibleLoopTrigger(void)
{
	if(level==FINAL_LEVEL)
		return 50;
	else if (level==FINAL_LEVEL - 1)
		return 250;
	else if (level==FINAL_LEVEL - 2)
		return 500;
	else
		return 1000 - level*20;
}

