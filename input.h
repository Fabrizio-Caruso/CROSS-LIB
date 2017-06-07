#ifndef _INPUT
#define _INPUT

#include "character.h"

void movePlayer(Character *playerPtr, char kbInput);

void movePlayerByJoystick(Character *playerPtr, unsigned char joyInput);

int computeInvincibleCountDown(void);

#endif // _INPUT