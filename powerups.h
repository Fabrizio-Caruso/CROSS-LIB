#ifndef _POWERUPS
#define _POWERUPS


int powerUpReached(Character * hunterPtr, Character* preyPtr);

void computePowerUp(int *coolDownDecreasePtr, int *powerUpInitialCoolDownPtr);

int computeGunInitialCoolDown(void);

#endif // _POWERUPS