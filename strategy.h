#ifndef _STRATEGY
#define _STRATEGY

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include <unistd.h>

#include "character.h"

void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr);
					
void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr);

void blindChaseCharacterMaxStrategy(Character* hunterPtr, Character* preyPtr);

void blindChaseCharacter(Character* hunterPtr, Character* preyPtr);



void chaseCharacterXAvoidBombStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7);


void chaseCharacterYAvoidBombStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7);
					

void chaseCharacterXYStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, 
					int ghostSmartness);
					
void chaseCharacterMaxAndXStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7,
					int ghostSmartness);

void chaseCharacterMaxAndYStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7,
					int ghostSmartness);

void chaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7,
					int ghostSmartness);
					
void chaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7,
					int ghostSmartness);


void chaseCharacterXStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8,
					int ghostSmartness, int ghostSlowDown);			 

void chaseCharacterYStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8,
					int ghostSmartness, int ghostSlowDown);
					
void chaseCharacterIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8,
					int ghostSmartness, int ghostSlowDown);
					
void chaseCharacterMaxAndXStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8,
					int ghostSmartness, int ghostSlowDown);

void chaseCharacterMaxAndYStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8,
					int ghostSmartness, int ghostSlowDown);

void chasePlayer(Character * ghostPtr1, Character * ghostPtr2, 
                 Character * ghostPtr3, Character * ghostPtr4,
				 Character * ghostPtr5, Character * ghostPtr6, 
                 Character * ghostPtr7, Character * ghostPtr8, 
                 Character* preyPtr, 
                 Character* bombPtr1, Character* bombPtr2,
				 Character* bombPtr3, Character* bombPtr4,
				 int ghostSmartness, int ghostSlowDown);

#endif // _STRATEGY