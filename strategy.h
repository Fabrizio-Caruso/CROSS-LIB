#ifndef _STRATEGY
#define _STRATEGY

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "character.h"

void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr);
					
void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr);

void chaseCharacter(Character *hunterPtr, Character *preyPtr, int slowDown);

void chasePlayer(Character ** ghosts, 
                 Character* preyPtr, int slowDown);

#endif // _STRATEGY