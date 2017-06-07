#ifndef _POWERUPS
#define _POWERUPS


int powerUpReached(Character * hunterPtr, Character* preyPtr);

void computePowerUp(unsigned int *coolDownDecreasePtr, unsigned int *powerUpInitialCoolDownPtr);

int computeGunInitialCoolDown(void);

#endif // _POWERUPS