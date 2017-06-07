
#include "character.h"
#include "powerups.h"


extern unsigned short level;

int powerUpReached(Character * hunterPtr, Character* preyPtr)
{
	return(areCharctersAtSamePosition(hunterPtr, preyPtr));
}


void computePowerUp(int *coolDownDecreasePtr, int *powerUpInitialCoolDownPtr)
{
	*coolDownDecreasePtr = 200-(level/2-1)*10;
	*powerUpInitialCoolDownPtr = 200+(level/2-1)*10;
}

int computeGunInitialCoolDown(void)
{
	if(level<=5)
		return 600;
	else
		return 600 + level * 10;
}

