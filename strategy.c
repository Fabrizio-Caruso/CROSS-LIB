
#include "settings.h"
#include "character.h"
#include "strategy.h"


extern unsigned short level;
extern unsigned short ghostCount;
extern unsigned int invincibleSlowDown;

void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
{
	if(hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}
	else if(hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
}


void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
{
    if(hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	else if(hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}
}

void chaseCharacter(Character *hunterPtr, Character *preyPtr, int slowDown)
{
	if(hunterPtr->_status)
	{
		if(rand()>slowDown)
		{
			if(rand()%2) // Select blind chase strategy
				{
					blindChaseCharacterXStrategy(hunterPtr, preyPtr);
				}
				else
				{
					blindChaseCharacterYStrategy(hunterPtr, preyPtr);
				}
		}
	}
}

void chasePlayer(Character ** ghosts, 
                 Character* preyPtr, 
                 int slowDown)
{
	char i=0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		chaseCharacter(ghosts[i], preyPtr, slowDown);
		displayCharacter(ghosts[i]);
	}
}
