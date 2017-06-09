#ifndef _INPUT
#define _INPUT

#include "character.h"

void movePlayerByKeyboard(Character *playerPtr, char kbInput);

void movePlayerByJoystick(Character *playerPtr, unsigned char joyInput);

#endif // _INPUT